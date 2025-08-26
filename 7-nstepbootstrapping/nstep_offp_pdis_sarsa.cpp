#include <iostream>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <queue>

//Exercise 7.10 -> Devise a samll off-policy prediction problem and use it to show that the off-policy
//learning algorithm using (7.13) and (7.2) is more data efficient than the simpler algorithm using (7.1) and
//(7.9).

// Off policy n-step SARSA using per-decision sampling
// Simple enviroment: Agent has to take a key in a given block to open the exit
// For every step taken, a reward of -1 is given to the agent.
// When the agent picks up the key, it recieves a +5 reward, then cannot recieve that reward anymore.

#define WIDTH 15
#define HEIGHT 7
#define NACTIONS 4 //UP DOWN LEFT RIGHT

#define START_X 0
#define START_Y 3
#define KEY_X 6
#define KEY_Y 1
#define EXIT_X 14
#define EXIT_Y 6

#define N 7
#define EPSILON 0.05
#define STEPSIZE 0.01
#define DISCOUNT 1
#define TEMPERATURE 1 // Has to be different than 0 for this implementation.
                      // Between 0 and 1 -> greedy/deterministic
                      // More than 1 -> uniform/random

#define INFINITY std::numeric_limits<float>::infinity();
#define _USE_MATH_DEFINES


struct State{
    int x;
    int y;
    bool hasKey;
    bool isTerminal
};

struct Timestep{
    State s; //State
    int a;   //Action
    float r; //Reward
};

float**** initializeQTable(){
    float**** qTable = new float*** [WIDTH];
    for(int i = 0; i < WIDTH; i++){ //width WIDTH
        qTable[i] = new float** [HEIGHT];
        for(int j = 0; j < HEIGHT; j++){ //height HEIGHT
            qTable[i][j] = new float* [2];
            for(int k = 0; k < 2; k++){ //has_key (binary)
                qTable[i][j][k] = new float [NACTIONS];
                for(int z = 0; z < NACTIONS; z++){
                    qTable[i][j][k][z] = 0.0;
                }
            }   
        }
    }
    return qTable;
}

float*** initializeVTable(){
    float*** vTable = new float*** [WIDTH];
    for(int i = 0; i < WIDTH; i++){ //width WIDTH
        vTable[i] = new float* [HEIGHT];
        for(int j = 0; j < HEIGHT; j++){ //height HEIGHT
            vTable[i][j] = new float[2];
            for(int k = 0; k < 2; k++){ //has_key (binary)
                vTable[i][j][k] = 0.0;
            }   
        }
    }
    return vTable;
}

int getActionSoftmaxPol(*float qTableRow){
    float sum_e_qval = 0;

    for(int i = 0; i < NACTIONS; i++){
        sum_e_qval += pow(M_E, qTableRow[i]/TEMPERATURE);
    }

    float cumulativeProb = 0;
    float randomChoice = (rand()%10000)/10000;
    for(int i = 0; i < NACTIONS; i++){
        cumulativeProb += pow(M_E, qTableRow[i]/TEMPERATURE) / sum_e_qval;
        if(randomChoice < cumulativeProb){
            return i;
        }
    }
}

int getActionEGreedyPol(float* qTableRow){
    if(rand()%100 < EPSILON*100){ //Random action
        return rand()%NACTIONS;
    } else {                      //Greedy action
        int bestAction = -1;
        float bestQValue = -INFINITY;

        for(int i = 0; i < NACTIONS; i++){
            if(qTableRow[i] > bestQValue){
                bestQValue = qTableRow[i];
                bestAction = i;
            }
        }

        return i;
    }
}

int getActionGreedyPol(float* qTableRow){
    int bestAction = -1;
    float bestQValue = -INFINITY;

    for(int i = 0; i < NACTIONS; i++){
        if(qTableRow[i] > bestQValue){
            bestQValue = qTableRow[i];
            bestAction = i;
        }
    }

    return i;
}

Timestep* walk(Timestep* currentState, float**** qTable, int policy){
    int availableActions[][2] = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };
    int actionTaken;
    float* qTableRow = &(qTable[currentState->State.x][currentState->State.y][currentState->State.hasKey]);

    switch(policy){
    case 0:
        actionTaken = getActionGreedyPol(qTableRow);
        break;

    case 1:
        actionTaken = getActionEGreedyPol(qTableRow);
        break;

    case 2:
        actionTaken = getActionSoftmaxPol(qTableRow);
        break;

    default:
        std::cout << "Invalid policy.\n";
        return;
    }

    currentState -> a = actionTaken;
    currentState -> r = -1;

    int new_x = currentState -> State.x + availableActions[actionTaken][0];
    int new_y = currentState -> State.y + availableActions[actionTaken][1];

    Timestep* next_ts = createTimestep(currentState->State.x, currentState->State.y);

    if(!(new_x < 0 || new_x >= WIDTH || new_y < 0 || new_y >= HEIGHT)){ //Checking for invalid actions
        next_ts->State.x = new_x;
        next_ts->State.y = new_y;
    }

    if(next_ts->x == KEY_X && next_ts->y == KEY_Y && !currentState->State.hasKey){
        currentState->r = 5;
        next_ts->State.hasKey = true;
    } else if(next_ts->x == EXIT_X && next_ts->y == EXIT_Y && currentState->State.hasKey){
        currentState->r = 0;
        next_ts->State.isTerminal = true;
    }

    return new_ts;
}

Timestep* createTimestep(int x, int y){
    Timestep* timeStep = new Timestep;

    timeStep -> State.x = START_X;
    timeStep -> State.y = START_Y;
    timeStep -> State.hasKey = false;
    timeStep -> State.isTerminal = false;
    timeStep -> a = -1;
    timeStep -> r = 0;

    return timeStep;
}

void addTimestepVector(std::vector<Timestep*>* episode, Timestep* timestep){
    episode->push_back(timestep);
    if(episode->size() > N){
        episode->erase(episode->begin());
    }
}

float getPolicyActionStateProb(int policy, Timestep* timestep, float**** qTable){
    switch(policy){
    case 0://greedy
        int bestAction = -1;
        float bestQValue = -INFINITY;

        for(int i = 0; i < NACTIONS; i++){
            float qval = qTable[timestep->State.x][timestep->State.y][timestep->State.hasKey][i];
            if(qval > bestQValue){
                bestQValue = qval;
                bestAction = i;
            }
        }

        if(timestep->a == bestAction){
            return 1;
        }
        return 0;

    case 1://egreedy
        int bestAction = -1;
        float bestQValue = -INFINITY;

        for(int i = 0; i < NACTIONS; i++){
            float qval = qTable[timestep->State.x][timestep->State.y][timestep->State.hasKey][i];
            if(qval > bestQValue){
                bestQValue = qval;
                bestAction = i;
            }
        }

        if(timestep->a == bestAction){
            return 1-EPSILON;
        }
        return EPSILON;
    
    case 2://softmax
        float sum_e_qval = 0;

        for(int i = 0; i < NACTIONS; i++){
            float qval = qTable[timestep->State.x][timestep->State.y][timestep->State.hasKey][i];
            sum_e_qval += pow(M_E, qval/TEMPERATURE);
        }

        float actQVal = qTable[timestep->State.x][timestep->State.y][timestep->State.hasKey][timestep->a];
        return pow(M_E, actQVal/TEMPERATURE) / sum_e_qval;
    
    default:
        std::cout << "Invalid policy.\n";
        return;
    }
}

float getImpSampRatio(int pBehavior, int pTarget, float**** qTable, int h, std::vector<Timestep*>* episode){
    float pth = 1.0;
    for(int i = 0; i < h; i++){ //vector t+n-1 <-> t
        Timestep* ts = episode.at(N-1-i);
        pth *= (getPolicyActionStateProb(pTarget,ts,qTable)/getPolicyActionStateProb(pBehavior,ts,qTable));
    }

    return pth;
}

void updateStateValue(float**** qTable, float*** vTable, std::vector<Timestep*>* episode, int i,
                      int pTarget, int pBehavior){
    if(i < n-1){ //doesnt update terminal states
        Timestep* ts = episode.at(i); //t+n -> i=
        Timestep* tsPrev = episode.at(i+1);
        float* tV = vTable[ts->State.x][ts->State.y][ts->State.hasKey];
        float* tPrevV = vTable[ts->State.x][ts->State.y][ts->State.hasKey];

        tV = tPrevV + STEPSIZE*getI
        mpSampRatio(pBehavior, pTarget, qTable, i, episode)
    }
}


void runEpisode(float**** qTable, int runs){
    int pBehavior = 1;//0- greedy //1- e-greddy //2- softmax
    int pTarget = 0;
    for(int run = 0; i < runs; i++){
        std::vector<Timestep*> nstepBuffer;

        currTimestep = createTimestep(START_X, START_Y);

        int T = INFINITY;
        int t = 0;
        int t_updt;

        while(t_updt != T - 1){
            if(t < T){
                Timestep* nextTimestep = walk(currTimestep, qTable, pUsed)

                if(nextTimestep->State.isTerminal){
                    T = t + 1;
                } else {
                    addTimestepVector(currentTimestep);
                }
            }
            t_updt = t - N + 1
            if(t >= 0){
                // Per decision importance sample truncated return will be used
            }

            t++;
        }
    }
}

int main{


}