#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRACK_HEIGHT 20
#define TRACK_WIDTH 10

#define WALL 1
#define PATH 0
#define START 2
#define END 3

#define MAX_V 4

typedef struct {
    int map[TRACK_HEIGHT][TRACK_WIDTH];
    int* start;
    int* end;
} Track;

typedef struct {
    int* pos;
    int* v;
} Kart;

typedef struct{
    int x;
    int y;
    int vx;
    int vy;
} State;

typedef struct Step Step;

struct Step{
    int t;
    Step* nextStep;
    State eState;
    int eAction;
    int eReward;
};

void generateTrack(Track* track, int start_x, int start_y, int end_x, int end_y){
    for(int i = 0; i < TRACK_HEIGHT; i++){
        for(int j = 0; j < TRACK_WIDTH; j++){
            if(i == 0 || j == 0 || i == TRACK_HEIGHT - 1 || j == TRACK_WIDTH - 1){
                track -> map[i][j] = WALL;
            } else {
                track -> map[i][j] = PATH;
            }
        }
    }

    if(start_x == 0 || start_x == TRACK_WIDTH-1 || start_y == 0 || start_y == TRACK_HEIGHT-1){
        track -> map[start_y][start_x] = START;
        track -> start = malloc(sizeof(int)*2);
        track -> start[0] = start_x; track -> start[1] = start_y;
    } else {
        return;
    }

    if(end_x == 0 || end_x == TRACK_WIDTH-1 || end_y == 0 || end_y == TRACK_HEIGHT-1){
        track -> map[end_y][end_x] = END;
        track -> end = malloc(sizeof(int)*2);
        track -> end[0] = end_x; track -> end[1] = end_y;
    } else {
        return;
    }

    for(int x = 4; x < TRACK_HEIGHT-1; x++){track -> map[x][1] = WALL;}
    for(int x = 7; x < TRACK_HEIGHT-1; x++){track -> map[x][2] = WALL;}
    for(int x = 10; x < TRACK_HEIGHT-1; x++){track -> map[x][3] = WALL;}
    for(int x = 14; x < TRACK_HEIGHT-1; x++){track -> map[x][4] = WALL;}
    for(int x = 17; x < TRACK_WIDTH-1; x++){track -> map[x][5] = WALL;}
    for(int x = 2; x > 0; x--){track -> map[x][6] = WALL;}
    for(int x = 4; x > 0; x--){track -> map[x][7] = WALL;}
    for(int x = 12; x > 0; x--){track -> map[x][8] = WALL;}
}

void showTrack(int track[TRACK_HEIGHT][TRACK_WIDTH]){
    for(int i = 0; i < TRACK_HEIGHT; i++){
        for(int j = 0; j < TRACK_WIDTH; j++){
            printf("%d ",track[i][j]);
        }
        printf("\n");
    }
}

int getIndex(int h, int w, int v_x, int v_y) {
    return ((((h * TRACK_WIDTH + w) * (MAX_V + 1) + v_x) * (MAX_V + 1) + v_y) * 9);
}

int weightedActionChoice(float* actions, int nA){
    float max = -1;
    int i = 0;
    float* weightedActions = malloc(sizeof(float)*nA);

    for(i = 0; i < nA; i++){
        weightedActions[i] = actions[i] * (float)(i+1);
        if(weightedActions[i] > max){
            max = weightedActions[i];
        };
    }

    float r = ((float)rand() / (float)RAND_MAX) * max;

    float cumulative = 0;
    for (int i = 0; i < nA; i++) {
        cumulative = weightedActions[i];
        if (r <= cumulative) {
            return i;
        }
    }

    return nA - 1;
}


float* createRandomPolicy(){
    int size = TRACK_HEIGHT*TRACK_WIDTH*(MAX_V+1)*(MAX_V+1)*9;
    float* pol = malloc(sizeof(float)*size);

    for(int i = 0; i < size; i++){
        pol[i] = 1.0f/9.0f;
    }

    return pol;
}

float* zeroInvalidActions(Kart kart, float* actions, int nA){
    int available_actions[][2] = {{-1, -1},{0, -1},{1, -1},{-1, 0},{0, 0},{1, 0},{-1, 1},{0, 1},{1, 1}};

    float* fixedActionsProb = malloc(sizeof(float)*9);
    for(int i = 0; i < nA; i++){
        fixedActionsProb[i] = actions[i];
        if((available_actions[i][0] + kart.v[0]) == 0 && (available_actions[i][1] + kart.v[1]) == 0){
            fixedActionsProb[i] = 0;
        }
        if((available_actions[i][0] + kart.v[0]) > 4){
            fixedActionsProb[i] = 0;
        }
        if((available_actions[i][1] + kart.v[1]) > 4){
            fixedActionsProb[i] = 0;
        }
        if((available_actions[i][0] + kart.v[0]) < -4){
            fixedActionsProb[i] = 0;
        }
        if((available_actions[i][1] + kart.v[1]) < -4){
            fixedActionsProb[i] = 0;
        }
    }

    return fixedActionsProb;
};

Step* createStep(State state, int action, int reward, int t, Step* oldStep){
    Step* newStep = malloc(sizeof(Step));
    newStep -> t = t;
    newStep -> nextStep = oldStep;

    //S(t)
    newStep -> eState.x = state.x;
    newStep -> eState.y = state.y;
    newStep -> eState.vx = state.vx;
    newStep -> eState.vy = state.vy;

    //A(t)
    newStep -> eAction = action;

    //R(t+1)
    newStep -> eReward = reward;

    return newStep;
}

Step* runEpisode(Track track, float* policy){
    int available_actions[][2] = {{-1, -1},{0, -1},{1, -1},{-1, 0},{0, 0},{1, 0},{-1, 1},{0, 1},{1, 1}};
    Kart kart;

    //Initialize initial values
    kart.pos = malloc(sizeof(int)*2);
    kart.v = malloc(sizeof(int)*2);

    kart.pos[0] = track.start[0];
    kart.pos[1] = track.start[1];
    kart.v[0] = 0;
    kart.v[1] = 0;

    int counter = 0;
    int actionI;

    Step* episode = NULL;
    State oldState;

    //Grab Policy action
    //Returns an array[9] with probabilities for each action
    while(track.map[kart.pos[1]][kart.pos[0]] != 3){
        float* actions = &policy[getIndex(kart.pos[1], kart.pos[0], kart.v[0], kart.v[1])];

        actionI = weightedActionChoice(zeroInvalidActions(kart, actions, 9), 9);

        //Save old state
        oldState.x = kart.pos[0]; oldState.y = kart.pos[1]; oldState.vx = kart.v[0]; oldState.vy = kart.v[1];

        //Accelerating 
        kart.v[0] += available_actions[actionI][0];
        kart.v[1] += available_actions[actionI][1];

        //Checking for velocity roof
        if(kart.v[0] > MAX_V) kart.v[0] = MAX_V;
        if(kart.v[1] > MAX_V) kart.v[1] = MAX_V;

        if(kart.v[0] < -MAX_V) kart.v[0] = -MAX_V;
        if(kart.v[1] < -MAX_V) kart.v[1] = -MAX_V;

        //Moving
        kart.pos[0] += kart.v[0];
        kart.pos[1] += kart.v[1];

        //Case kart hits a wall or goes out of the map, returns to start and v = 0
        if((track.map[kart.pos[1]][kart.pos[0]] == 1) || 
           (kart.pos[0] < 0 || kart.pos[0] >= TRACK_WIDTH) || (kart.pos[1] < 0 || kart.pos[1] >= TRACK_HEIGHT)
        ){
            kart.pos[0] = track.start[0];
            kart.pos[1] = track.start[1];
            kart.v[0] = 0;
            kart.v[1] = 0;
        }

        //If not reached finishline
        counter++;
        if((track.map[kart.pos[1]][kart.pos[0]] != 3)){
            episode = createStep(oldState, actionI, -1, counter, episode);
        }
    }

    //When while loop ends, episode is terminated
    episode = createStep(oldState, actionI, 0, counter, episode);
    return episode;
}

int main(){
    srand(time(NULL));

    Track track;
    generateTrack(&track, 0, 1, 8, 19);
    showTrack(track.map);
    float* randomPolicy = createRandomPolicy();

    int runs = 1;

    for(int i = 0; i < runs; i++){
        Step* episode = runEpisode(track, randomPolicy);
        while(episode){
            printf("x:%d, y:%d, vx:%d, vy:%d, action:%d, reward:%d, timestep:%d\n", episode->eState.x,
                                                                                    episode->eState.y,
                                                                                    episode->eState.vx,
                                                                                    episode->eState.vy,
                                                                                    episode->eAction,
                                                                                    episode->eReward,
                                                                                    episode->t);
            episode = episode -> nextStep;
        }
    }
}