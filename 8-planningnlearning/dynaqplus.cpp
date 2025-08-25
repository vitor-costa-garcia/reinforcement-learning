#include <iostream>
#include <vector>

#define HEIGHT 10
#define WIDTH 10

#define NSTATES HEIGHT*WIDTH
#define NACTIONS 4

#define PATH  0
#define WALL  1
#define AGENT 2

class DynaMaze{
private:
    unsigned int width = 0;
    unsigned int height = 0;
    std::vector<int> agentPosition;
    std::vector<std::vector<int>> maze;

public:
    DynaMaze(unsigned int height, unsigned int width){
        this->width = width;
        this->height = height;

        maze = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
        agentPosition = std::vector<int>(2, 0);
    }

    void displayMaze(){
        if(width != 0 and height != 0){
            for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    std::cout << maze[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    void placeAgent(unsigned int i, unsigned int j){
        if(i > 0 && i < width && j > 0 && j < height){
            agentPosition[0] = i;
            agentPosition[1] = j;
            return;
        }
        std::cout << "Invalid position.\n";
    }

    void moveAgent(int action){
        int moves[][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

        agentPosition[0] += moves[action][0];
        agentPosition[1] += moves[action][1];

        if(agentPosition[0] < 0){
            agentPosition[0] = 0;
            return;
        }
        if(agentPosition[0] >= height){
            agentPosition[0] = height-1;
            return;
        }
        if(agentPosition[1] < 0){
            agentPosition[1] = 0;
            return;
        }
        if(agentPosition[1] >= width){
            agentPosition[1] = width-1;
        } 
    }
};

std::vector<std::vector<std::vector<int>>> allocSAPairs(){
    return std::vector<std::vector<int>>(height, std::vector<int>(width, std::vector<int>(NACTIONS, 0)));
}

int main(){
    DynaMaze maze(4,5);
    maze.displayMaze();
    std::vector<std::vector<std::vector<int>>> qT = allocSAPairs();
    // std::vector<std::vector<std::vector<int>>> model = allocSAPairs();
}