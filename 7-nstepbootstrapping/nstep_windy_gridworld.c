#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 15
#define NACTIONS 8

void walk(int* pos, int* wind, int action){
	int actions = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

	//Normal action moving
	pos[0] += actions[action][0];
	pos[1] += actions[action][1];

	if(pos[0]<0 || pos[0]>=WIDTH || pos[1]<0 || pos[1]>=HEIGHT){ //INVALID ACTIONS DOESNT MOVE AGENT
		pos[0] -= actions[action][0];
		pos[1] -= actions[action][1];
		return;
	};

	//Wind going north with 70% chance (Stochastic part)
	if(rand() % 10 < 7 && ( pos[1] - wind[pos[0]] ) >= 0){
		pos[1] -= wind[pos[0]];
	}
}

float*** initializeQTable(){
	float*** qTable = malloc(sizeof(float**)*WIDTH);
	for(int i = 0; i < WIDTH; i++){
		qTable[i] = malloc(sizeof(float*)*HEIGHT);
		for(int j = 0; j < HEIGHT; j++){
			qTable[i][j] = malloc(sizeof(float)*NACTION);
			for(int k = 0; k < NACTION; k++){
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
	int bestAction = 0;
	float bestActionQValue = 0;

	for(int i = 1; i < NACTION; i++){
		if(qTableRow[i] > bestActionQValue){
			bestAction = i;
			bestActionQValue = qTableRow[bestAction];
		}
	}

	return bestAction;
}

int eGreedyPolicyAction(float epsilon, int* pos, float*** qTable){
	if(rand%100 < epsilon*100){
		return rand%NACTIONS;
	} else {
		float* qTableRow = &(qTable[pos[0]][pos[1]]);
		return getGreedyAction(qTableRow);
	}
}


void runEpisode_NStepSarsa(int n){
	float lastRewards[n];
	for(int i = 0; i < n; i++){
		lastRewards[i] = 0;
	}
	int t = 0;
	int startPos[2] = {2, 7}; int endPos[2] = {16, 7};
	
}

int main(){

}