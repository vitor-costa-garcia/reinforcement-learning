#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define INFINITE 999999999

#define WIDTH 20
#define HEIGHT 15
#define NACTIONS 8

#define DISCOUNT 1
#define STEPSIZE 0.01

#define N 10

typedef struct{
	int x;
	int y;
} State;

typedef struct{
	State state;
	int action;
	float reward;
} Timestep;

int min_int(int a, int b) {
    return (a < b) ? a : b;
}

void walk(int* pos, int wind[WIDTH], int action){
	int actions[][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

	//Normal action moving
	pos[0] += actions[action][0];
	pos[1] += actions[action][1];

	if(pos[0]<0 || pos[0]>=WIDTH || pos[1]<0 || pos[1]>=HEIGHT){ //INVALID ACTIONS DOESNT MOVE AGENT
		pos[0] -= actions[action][0];
		pos[1] -= actions[action][1];
		return;
	};

	//Wind going north with 70% chance (Stochastic part)
	if(rand() % 10 < 7){
		pos[1] -= wind[pos[0]];
	}

	if(pos[1] < 0){
		pos[1]=0;
	}
}

float*** initializeQTable(){
	float*** qTable = malloc(sizeof(float**)*WIDTH);
	for(int i = 0; i < WIDTH; i++){
		qTable[i] = malloc(sizeof(float*)*HEIGHT);
		for(int j = 0; j < HEIGHT; j++){
			qTable[i][j] = malloc(sizeof(float)*NACTIONS);
			for(int k = 0; k < NACTIONS; k++){
				qTable[i][j][k] = 0;
			}
		}
	}

	return qTable;
}

void freeQTable(float*** qTable){
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			free(qTable[i][j]);
		}
		free(qTable[i]);
	}
};

int getGreedyAction(float* qTableRow){
	int bestAction = -1;
	float bestActionQValue = -INFINITE;

	for(int i = 0; i < NACTIONS; i++){
		if(qTableRow[i] > bestActionQValue){
			bestAction = i;
			bestActionQValue = qTableRow[bestAction];
		}
	}

	return bestAction;
}

int eGreedyPolicyAction(float epsilon, int* pos, float*** qTable){
	if(rand()%100 < epsilon*100){
		return rand()%NACTIONS;
	} else {
		float* qTableRow = qTable[pos[0]][pos[1]];
		return getGreedyAction(qTableRow);
	}
}

void saveTimestep(Timestep* timestep, int n, int x, int y, int a, int r){
	for(int i = n-1; i > 0; i--){
		timestep[i].state.x = timestep[i-1].state.x;
		timestep[i].state.y = timestep[i-1].state.y;
		timestep[i].action = timestep[i-1].action;
		timestep[i].reward = timestep[i-1].reward;
	}

	timestep[0].state.x = x;
	timestep[0].state.y = y;
	timestep[0].action = a;
	timestep[0].reward = r;
};


void runEpisode_NStepSarsa(float*** qTable){
	Timestep* lastTimestep = malloc(sizeof(Timestep)*N);
	for(int i = 0; i < N; i++){
		lastTimestep[i].state.x = 0;
		lastTimestep[i].state.y = 0;
		lastTimestep[i].action = 0;
		lastTimestep[i].reward = 0;
	}

	int t = 0;
	int T = INFINITE;
	int t_updt = 0;

	int wind[WIDTH] = {0,0,0,0,0,1,1,0,0,3,2,2,1,1,1,2,3,0,0,0};

	float g = 0;

	int startPos[2] = {2, 7};
	int endPos[2] = {16, 7};

	int* currentPos = malloc(sizeof(int)*2);
	currentPos[0] = startPos[0]; currentPos[1] = startPos[1];

	bool terminal = false;

	//printf("Start episode!");
	while(t_updt != T - 1){
		//printf("t:%d | T:%d | t_updt:%d | currx: %d | curry: %d\n", t, T, t_updt, currentPos[0], currentPos[1]);
		if(t < T){
			if(!terminal){
				int prevX = currentPos[0]; int prevY = currentPos[1];
				int action = eGreedyPolicyAction(0.1, currentPos, qTable);
				float reward = -1;
				walk(currentPos, wind, action);
				if(currentPos[0] == endPos[0] && currentPos[1] == endPos[1]){
					T = t + 1;
					terminal = true;
					reward = 0;
					//printf("Terminal State!\n");
				}
				saveTimestep(lastTimestep, N, prevX, prevY, action, reward);
			}
		}
		t_updt = t - N + 1;
		//if(terminal){
		//	printf("%d\n", T - t_updt);
		//}
		if(t_updt >= 0){
			g = 0;
			for(int i = t_updt + 1; i < min_int(t_updt + N, T); i++){
				//printf("index_forloop:%d | reward:%f\n",(N-1)-(i-t_updt-1)-1, lastTimestep[(N-1)-(i-t_updt-1)-1].reward);
				g += pow(DISCOUNT, i-t_updt-1)*lastTimestep[(N-1)-(i-t_updt-1)-1].reward; //truncated return
			}
			if(t_updt + N < T){
				//printf("YES\n");
				g += pow(DISCOUNT, N)*qTable[lastTimestep[0].state.x][lastTimestep[0].state.y][lastTimestep[0].action];
			}
			//printf("g: %f\n", g);
			int i_u = (min_int(t_updt + N, T) - t_updt) - 1;
			qTable[lastTimestep[i_u].state.x][lastTimestep[i_u].state.y][lastTimestep[i_u].action] += 
			STEPSIZE*(g - qTable[lastTimestep[i_u].state.x][lastTimestep[i_u].state.y][lastTimestep[i_u].action]);
		}
		t++;
	}

	free(lastTimestep);
	free(currentPos);
}

void printStateVal(float*** qTable, int endpos[2]){
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			float* qTableRow = qTable[j][i];
			float maxQVal = -INFINITE;
			int maxAct = -1;
			for(int k = 0; k < NACTIONS; k++){
				if(qTableRow[k] > maxQVal){
					maxQVal = qTableRow[k];
					maxAct = k;
				}
			}
			if(i==endpos[1] && j == endpos[0]){
				printf("[E] ");
			} else {
				printf("[%d] ", maxAct);
				//printf("[%.1f] ", maxQVal);
			}
		}
		printf("\n");
	}
}

int main(){
	srand(time(NULL));
	float*** qTable = initializeQTable();
	int episodes = 100000;

	for(int i = 0; i < episodes; i++)
		runEpisode_NStepSarsa(qTable);

	int endpos[2] = {16, 7};
	printStateVal(qTable, endpos);

	freeQTable(qTable);
	free(qTable);
}