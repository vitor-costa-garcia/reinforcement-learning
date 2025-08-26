#include <iostream>
#include <vector>

#define HEIGHT 12
#define WIDTH 12

#define NSTATES HEIGHT*WIDTH
#define NACTIONS 4

#define PATH  0
#define WALL  1
#define AGENT 2
#define START 3
#define END 4

#define LR 0.1
#define DF 0.95
#define EPS 0.1
#define KPL 0.0001 //k for dynaQ+

using qTable = std::vector<std::vector<std::vector<float>>>;
using sTable = std::vector<std::vector<std::vector<bool>>>;
using mTable = std::vector<std::vector<std::vector<std::vector<float>>>>;

class DynaMaze{
private:
    unsigned int width = 0;
    unsigned int height = 0;
    std::vector<std::vector<int>> maze;
    std::vector<int> startPos;
    std::vector<int> endPos;

public:
    std::vector<int> agentPosition;

    DynaMaze(unsigned int height, unsigned int width, unsigned int startX , unsigned int startY, unsigned int endX , unsigned int endY){
        this->width = width;
        this->height = height;

        maze = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
        agentPosition = std::vector<int>(2, 0);
        startPos.push_back(startX);
        startPos.push_back(startY);
        endPos.push_back(endX);
        endPos.push_back(endY);
    }

    void displayMaze(){
        if(width != 0 && height != 0){
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    if(i == agentPosition[0] && j == agentPosition[1])
                        std::cout << AGENT << " ";
                    else if(i == startPos[0] && j == startPos[1])
                        std::cout << START << " ";
                    else if(i == endPos[0] && j == endPos[1])
                        std::cout << END << " ";
                    else
                        std::cout << maze[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    void placeWall(unsigned int i, unsigned int j){
        if(i < height && j < width){
            maze[i][j] = WALL;
            return;
        }
        std::cout << "Invalid position.\n";
    }

    void placePath(unsigned int i, unsigned int j){
        if(i < height && j < width){
            maze[i][j] = PATH;
            return;
        }
        std::cout << "Invalid position.\n";
    }

    void placeAgent(unsigned int i, unsigned int j){
        if(i < height && j < width){
            agentPosition[0] = i;
            agentPosition[1] = j;
            return;
        }
        std::cout << "Invalid position.\n";
    }

    void resetAgent(){
        agentPosition[0] = startPos[0];
        agentPosition[1] = startPos[1];
    };

    bool mazeEnded(){
        if(agentPosition[0] == endPos[0] && agentPosition[1] == endPos[1]){
            return true;
        }
        return false;
    }

    float moveAgent(int action){
        int moves[][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

        int newI = agentPosition[0] + moves[action][0];
        int newJ = agentPosition[1] + moves[action][1];

        if(newI >= 0 && newI < height && newJ >= 0 && newJ < width
           && maze[newI][newJ] != WALL){
            agentPosition[0] = newI;
            agentPosition[1] = newJ;

            if(mazeEnded()){
                return 100;
            }
        }
        return -1;
    }

};

int getAction(std::vector<int> pos, qTable qT, int policy){
    switch(policy){
        case 0: {
            if(rand()%100 < EPS*100){
                return rand()%NACTIONS;
            }
        }
        case 1: { //fall to complete egreedy algorithm 
            int bestAction = -1;
            double bestQ = -999999;
            bool draw = false;

            for(int i = 0; i < NACTIONS; i++){
                if(qT[pos[0]][pos[1]][i] == bestQ){
                    draw = true;
                }
                if(qT[pos[0]][pos[1]][i] > bestQ){
                    bestQ = qT[pos[0]][pos[1]][i];
                    bestAction = i;
                    draw = false;
                }
            }

            if(draw){
                return rand()%NACTIONS;
            }

            return bestAction;
        }
    }
}

void printQTable(qTable qT){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            for(int a = 0; a < NACTIONS; a++){
                std::cout << qT[i][j][a] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

qTable allocQTable(){
    return std::vector<
            std::vector<
             std::vector<float>
             >>(HEIGHT, std::vector<std::vector<float>>(
                WIDTH, std::vector<float>(
                NACTIONS, 0)));
}

sTable allocSeenTable(){
    return std::vector<
            std::vector<
             std::vector<bool>
             >>(HEIGHT, std::vector<std::vector<bool>>(
                WIDTH, std::vector<bool>(
                NACTIONS, false)));
}

mTable allocModel(){
    return std::vector<
            std::vector<
             std::vector<
              std::vector<float>
              >>>(HEIGHT, std::vector<std::vector<std::vector<float>>>(
                  WIDTH, std::vector<std::vector<float>>(
                  NACTIONS, std::vector<float>(4, 0))));
}

void tabularDynaQ(DynaMaze maze, qTable& qT, mTable& model, int n, int episodes, bool showInfo){
    int oldS[2] = {0, 0};
    std::vector<std::vector<float>> seenSA = std::vector<std::vector<float>>(0);
    sTable seenTable = allocSeenTable();

    for(int episode = 0; episode < episodes; episode++){
        maze.resetAgent(); //S -> start state
        int timestep = 0;
        while(1==1){
            if(showInfo){std::cout << "(" << maze.agentPosition[0] << " " << maze.agentPosition[1] << ")";};
            int action = getAction(maze.agentPosition, qT, 0); //A -> egreedy
            oldS[0] = maze.agentPosition[0];
            oldS[1] = maze.agentPosition[1];// saving S
            float reward = maze.moveAgent(action); // moving agent and recieving reward

            float greedyQS = qT[maze.agentPosition[0]][maze.agentPosition[1]][getAction(maze.agentPosition, qT, 1)];// maxaQ(s,a)

            qT[oldS[0]][oldS[1]][action] += LR*(reward + (DF * greedyQS) - qT[oldS[0]][oldS[1]][action]); //update rule

            model[oldS[0]][oldS[1]][action][0] = reward; // index 0->reward
            model[oldS[0]][oldS[1]][action][1] = maze.agentPosition[0]; // index 1->next state
            model[oldS[0]][oldS[1]][action][2] = maze.agentPosition[1]; // index 2->next state

            std::vector<float> obsSA = std::vector<float>(4, 0);
            obsSA[0] = oldS[0];
            obsSA[1] = oldS[1];
            obsSA[2] = action;
            obsSA[3] = timestep;

            if(!seenTable[obsSA[0]][obsSA[1]][obsSA[2]]){
                seenSA.push_back(obsSA);
                seenTable[obsSA[0]][obsSA[1]][obsSA[2]] = true;
            } else {
                for(int i = 0; i < seenSA.size(); i++){
                    if(seenSA[i][0] == obsSA[0] && seenSA[i][1] == obsSA[1] && seenSA[i][2] == obsSA[2]){
                        seenSA[i][3] = timestep;
                        break;
                    }
                }
            }

            for(int i = 0; i < n; i++){
                int rObsSA = rand()%seenSA.size();
                int sX = seenSA[rObsSA][0];
                int sY = seenSA[rObsSA][1];
                int sA = seenSA[rObsSA][2];
                int sT = seenSA[rObsSA][3];

                float nsR = model[sX][sY][sA][0] + KPL*(timestep - sT);
                int nsX = model[sX][sY][sA][1];
                int nsY = model[sX][sY][sA][2];

                std::vector<int> nextPos = std::vector<int>(2, 0);

                nextPos[0] = nsX;
                nextPos[1] = nsY; 

                greedyQS = qT[nsX][nsY][getAction(nextPos, qT, 1)];

                qT[sX][sY][sA] += LR*(nsR + (DF * greedyQS) - qT[sX][sY][sA]); //update rule

            }

            timestep ++;

            if(maze.mazeEnded()){
                std::cout<<"Ended.\n";
                break;
            }
        }
    }
}

int main(){
    DynaMaze maze(12, 12, 0, 0, 11, 0);
    qTable qT = allocQTable();
    mTable model = allocModel();

    for(int i = 0; i < WIDTH-1; i++){
        maze.placeWall(6, i);
    }

    maze.displayMaze();

    tabularDynaQ(maze, qT, model, 50, 500, true);

    maze.placeWall(6, WIDTH-1);
    maze.placePath(6, 0);

    maze.displayMaze();

    tabularDynaQ(maze, qT, model, 50, 500, true);
}