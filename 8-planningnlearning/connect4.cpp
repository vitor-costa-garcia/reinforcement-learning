#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <array>
#include <vector>

using namespace std;

#define NROLLOUT 50000

#define ROWS 6
#define COLS 7

#define BLUE  0
#define RED   1
#define EMPTY 2

#define CONNECT 4

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
}

bool isIn(vector<int> v, int n){
    int l = 0;
    int h = v.size() - 1;

    while (l <= h) {
        int m = l + (h - l) / 2; // correct midpoint

        if (v[m] == n) {
            return true; // found
        } else if (v[m] > n) {
            h = m - 1;
        } else {
            l = m + 1;
        }
    }

    return false; // not found
}

int getMax(array<int, COLS> action_values){
    int bestAction = -1;
    int bestVal = -9999999;
    for(int i = 0; i < COLS; i++){
        if(action_values[i] > bestVal){
            bestVal = action_values[i];
            bestAction = i;
        }
    }
    return bestAction;
}

class Connect4{
private:
    array<array<int, ROWS>, COLS> board {};

public:
    Connect4(){
        for(int i = 0; i < COLS; i++){
            board[i].fill(EMPTY);
        }
    }

    vector<int> legal_moves(){
        vector<int> legal_moves;
        for(int i = 0; i < COLS; i++){
            if(board[i][0] == 2){
                legal_moves.push_back(i);
            }
        }

        return legal_moves;
    }

    void show(){
        for(int i = 0; i < ROWS; i++){
            cout << '|';
            for(int j = 0; j < COLS; j++){
                switch(board[j][i]){
                case 0:
                    cout << "\x1B[34mO\x1B[0m" << '|';
                    break;
                case 1:
                    cout << "\033[31mO\033[0m" << '|';
                    break;
                case 2:
                    cout << " |";
                    break;
                }
            }
            cout << '\n';
        }
        for(int i = 1; i <= COLS; i++){
            cout << '|'<< i;
        }
        cout << "|\n";
    }

    void play(int column, int color){
        vector<int> legal_m = legal_moves();
        if(isIn(legal_m, column)){
            for(int i = ROWS-1; i >= 0; i--){
                if(board[column][i] == EMPTY){
                    board[column][i] = color;
                    return;
                }
            }
        } else {
            cout << "invalid move! : " << column << "\n";
        }
        return;
    }

    int checkWin(int color){
        for(int j = ROWS - 1; j > ROWS - CONNECT; j--){
            for(int i = 0; i < COLS - (CONNECT - 1); i++){
                bool win = true;
                for(int k = 0; k < CONNECT; k++){
                    if(board[i+k][j-k] != color){
                        win = false;
                        break;
                    }
                }

                if(win) return 1;
            }
        }

        for(int j = ROWS - 1; j > ROWS - CONNECT; j--){
            for(int i = 0; i < COLS - (CONNECT - 1); i++){
                bool win = true;
                for(int k = 0; k < CONNECT; k++){
                    if(board[(CONNECT - 1)+i-k][j-k] != color){
                        win = false;
                        break;
                    }
                }
                if(win) return 1;
            }
        }

        for(int j = 0; j < ROWS; j++){
            for(int i = 0; i <= COLS - CONNECT; i++){
                bool win = true;
                for(int k = 0; k < CONNECT; k++){
                    if(board[i+k][j] != color){
                        win = false;
                        break;
                    }
                }
                if(win) return 1;
            }
        }

        for(int j = 5; j > ROWS - CONNECT; j--){
            for(int i = 0; i < COLS; i++){
                bool win = true;
                for(int k = 0; k < CONNECT; k++){
                    if(board[i][j-k] != color){
                        win = false;
                        break;
                    }
                }
                if(win) return 1;
            }
        }

        return 0;
    }

    bool checkDraw(){
        vector<int> legal_m = legal_moves();
        if(legal_m.size() == 0){
            return 1;
        }
        return 0;
    }

    int runRollout(int action, int color){
        array<array<int, ROWS>, COLS> currentBoard = board;
        int reward = -2;

        play(action, color);

        int currentPlayer = (color+1)%2;

        while(reward == -2){
            vector<int> legal_m = legal_moves();
            int randomAction = legal_m[rand()%legal_m.size()];
            play(randomAction, currentPlayer);
            currentPlayer = (currentPlayer+1)%2;

            if(checkWin(color)){
                reward = 1;
            }
            if(checkWin((color+1)%2)){
                reward = -1;
            }
            if(checkDraw()){
                reward = 0;
            }
        }

        board = currentBoard;
        return reward;
    }

    void runGame(){
        int action;
        int playerColor;
        int gameWinner = 0;

        cout << "Choose color: 0 BLUE | 1 RED\n";
        cin >> playerColor;

        int currentColor = 0;

        while(!gameWinner){
            if(checkWin(playerColor)){
                gameWinner = 1; //PLAYER
                break;
            }
            if(checkWin((playerColor+1)%2)){
                gameWinner = 2; //BOT
                break;
            }
            if(checkDraw()){
                gameWinner = 3; //DRAW
                break;
            }

            if(currentColor == playerColor){
                show();
                vector<int> legal_m = legal_moves();
                cout << "Moves available: ";
                for(int i = 0; i < legal_m.size(); i++){
                    cout << legal_m[i]+1 << ' ';
                }
                cout << '\n';

                cin >> action;

                play(action-1, playerColor);
            } else {
                array<int, COLS> action_values {};
                vector<int> legal_m = legal_moves();
                for(int i = 0; i < COLS; i++){
                    if(!isIn(legal_m, i)){
                        action_values[i] = -999999;
                    }
                }
                int legal_m_size = legal_m.size();
                cout << "Thinking...\n";
                for(int i = 0; i < NROLLOUT; i++){
                    int startAction = legal_m[rand()%legal_m_size];
                    action_values[startAction] += runRollout(startAction, (playerColor+1)%2);
                }
                int bestAction = getMax(action_values);
                cout << "Bot played column " << bestAction+1 << '\n';
                play(bestAction, (playerColor+1)%2);
            }

            currentColor = (currentColor+1)%2; 
        }

        show();
        switch(gameWinner){
        case 1:
            cout << "You won!";
            break;
        case 2:
            cout << "You lost!";
            break;
        case 3:
            cout << "Draw!";
            break;
        }
    }
};

int main(){
    enableANSI();

    Connect4 game;
    game.runGame();
};