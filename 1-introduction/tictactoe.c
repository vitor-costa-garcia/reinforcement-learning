#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Environment dynamics
Win -> R = 1 Winn must be rewarded
Loss -> R = -1 Loss must be penalized 
Draw -> R = 0 Draw must not be the same as a loss so is a game where he cant win anymore, it prioritizes a draw instead of a loss
Non terminal state -> R = 0;
Off-policy Q-learning

State will be represented as an 1D array of length 9
2  -> X
1 -> O
0 -> Empty
*/

#define TABLE_SIZE 1009

int encodeBoard(int* board){
	unsigned int hash = 0;
	for(int i = 0; i < 9; i++){
		hash = hash*7 + board[i];
	}
	return hash;
}

float* hashFunction(float* policy, int* board){
	return policy[encodeBoard(board)%TABLE_SIZE];
}

float** initializeQTable(){
	float** qTable = malloc(sizeof(float*)*TABLE_SIZE);
	for(int i = 0; i < TABLE_SIZE; i++){

		qTable[i] = malloc(sizeof(float)*9);

		for(int j = 0; j < 9; j++){
			qTable[i][j] = 0.0;
		}

	}

	return qTable;
}

int getCurrentBestAction(float* qTableRow){
	int currentBestAction = 0;
	float currentBestActionQValue = qTableRow[0];

	for(int i = 1; i < 9; i++){
		if(qTableRow[i] > currentBestActionQValue){
			currentBestActionQValue = qTableRow[i];
			currentBestAction = i;
		}
	}

	return currentBestAction;
}

void printBoard(int* board){
	for(int i = 0; i < 5; i+=3){
		printf("%d|%d|%d\n", board[i], board[i+1], board[i+2]);
		printf("-----\n");
	}
	printf("%d|%d|%d\n", board[6], board[7], board[8]);
}

void copyBoard(int* board1, int* board2){ //Copy content from board1 to board2
	for(int i = 0; i < 9; i++){
		board2[i] = board1[i];
	}
}

void reflect(int* board){
	int* newboard = malloc(sizeof(int)*9);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			newboard[i+(j*3)] = board[2-i+(j*3)];
		}
	}
	copyBoard(newboard, board);
	free(newboard);
};

void rot90(int* board){
	int* newboard = malloc(sizeof(int)*9);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			newboard[j+(3-i-1)*3] = board[(j*3)+i];
		}
	}
	copyBoard(newboard, board);
	free(newboard);
};

bool compareBoard(int* board1, int* board2){ //Is board1 lower than board2?
	for(int i = 0; i < 9; i++){
		if(board1[i]<board2[i]){
			return true;
		}else if(board1[i]>board2[i]){
			return false;
		}
	}
	return false; //equal boards
};

void cannonical_state(int* board){
	int* currboard = malloc(sizeof(int)*9);
	int* tboard = malloc(sizeof(int)*9);;
	copyBoard(board, currboard);
	copyBoard(board, tboard);

	for(int i = 0; i < 4; i++){
		rot90(currboard);
		if(compareBoard(currboard, tboard)){
			copyBoard(currboard, tboard);
		}

		
		reflect(currboard);
		if(compareBoard(currboard, tboard)){
			copyBoard(currboard, tboard);
		}
	}
	copyBoard(tboard, board);
	free(tboard);
	free(currboard);
};

bool checkBoardWin(int* board, int marker){
	for(int i = 0; i < 3; i++){
		if(board[0+(i*3)]==marker &&
		   board[1+(i*3)]==marker &&
		   board[2+(i*3)]==marker){
			return true;
		}

		if(board[0+i]==marker &&
		   board[3+i]==marker &&
		   board[6+i]==marker){
			return true;
		}
	}

	if(board[0]==marker && board[4]==marker && board[8]==marker){
		return true;
	}

	if(board[2]==marker && board[4]==marker && board[6]==marker){
		return true;
	}

	return false;
}

void playAction(int* board, int position, int marker){
	//Marker 1-X Marker 0-O
	board[position] = marker;
}

int* initializeBoard(){
	int* board = malloc(sizeof(int)*9);
	for(int i = 0; i < 9; i++){board[i]=0;}
	return board;
}

void runEpisode(float** qTable, float stepsize, float discount){
	int* board = initializeBoard();
	int turn = (rand()%2)+1; 
	while(checkBoardWin(board, 1) && checkBoardWin(board, 2)){
		
	}
}


int main(){
	int* board = initializeBoard();
	playAction(board, 0, 1);
	playAction(board, 1, 2);
	playAction(board, 4, 1);
	playAction(board, 2, 2);
	playAction(board, 8, 1);

	printBoard(board);
	cannonical_state(board);
	printBoard(board);
}










