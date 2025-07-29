#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h> // For wide character functions

/*
	Exercise 6.9 Windy Gridworld with King's moves
	Applying On-policy TD control (SARSA) to windy gridwold problem, but with
	8 moves instead of 4. The agent can go up, down, left, right and move di-
	agonally.
*/

#define INFINITE 99999999
#define WIDTH 10
#define HEIGHT 7
#define NACTION 8
#define EPS 0.1
#define STEPSIZE 0.5
#define DISCOUNT 1

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

bool checkActionIsValid(int x,int y,int a){
	int actions[][2] = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};
	int newX = x+actions[a][0]; int newY = y+actions[a][1];
	if(newX < 0 || newX >= WIDTH){return false;}
	if(newY < 0 || newY >= HEIGHT){return false;}
	return true;
}

int chooseAction(float* qTableActions, int x, int y){ //e-greedy policy
	if(rand()%100 < EPS*100){ //exploratory action
		int randomAction = rand()%NACTION;
		while(!checkActionIsValid(x,y,randomAction)){
			randomAction = rand()%NACTION;
		}
		return randomAction;
	} else { // greedy action
		int bestAction = -1;
		int bestActionQValue = -INFINITE;

		for(int i = 1; i < NACTION; i++){
			if(!checkActionIsValid(x,y,i)){
				continue;
			}
			if(qTableActions[i] > bestActionQValue){
				bestAction=i;
				bestActionQValue=qTableActions[i];
			}
		}

		return bestAction;
	}
}

void runEpisode(float*** qTable, int wind[WIDTH]){
	int actions[][2] = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};

	int oldPos[2] =  {0, 3};
	int currPos[2] = {0, 3};
	int endPos[2] =  {7, 3};

	int a = chooseAction(&qTable[currPos[0]][currPos[1]][0], currPos[0], currPos[1]);  //

	while(1){
		oldPos[0] = currPos[0];
		oldPos[1] = currPos[1];

		currPos[0] += actions[a][0];
		currPos[1] += actions[a][1];

		currPos[1] -= wind[currPos[0]];
		if(currPos[1]<0){currPos[1]=0;};

		if(currPos[0] == endPos[0] && currPos[1] == endPos[1]){
			return;
		}

		int nA = chooseAction(&qTable[currPos[0]][currPos[1]][0], currPos[0], currPos[1]);                                                       //
		qTable[oldPos[0]][oldPos[1]][a] += 
		STEPSIZE*(-1+DISCOUNT*qTable[currPos[0]][currPos[1]][nA]
		-qTable[oldPos[0]][oldPos[1]][a]);
		a = nA;
	}
}

int findGreedyAction(float* qTableRow, int x, int y){
	int bestAction = -1;
	int bestActionQValue = -INFINITE;

	for(int i = 1; i < NACTION; i++){
		if(!checkActionIsValid(x,y,i)){
			continue;
		}
		if(qTableRow[i] > bestActionQValue){
			bestAction=i;
			bestActionQValue=qTableRow[i];
		}
	}

	return bestAction;
}

void printActionMap(float*** qTable, int wind[WIDTH]){
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			if(j == 0 && i == 3){
				printf("[S ] ");
			} else if(j == 7 && i == 3){
				printf("[G ] ");
			} else {
				int action = findGreedyAction(&qTable[j][i][0], j, i);
				switch(action){
					case 0:
						printf("[↖] ");
						break;
					case 1:
						printf("[↑] ");
						break;
					case 2:
						printf("[↗] ");
						break;
					case 3:
						printf("[→] ");
						break;
					case 4:
						printf("[↘] ");
						break;
					case 5:
						printf("[↓] ");
						break;
					case 6:
						printf("[↙] ");
						break;
					case 7:
						printf("[←] ");
						break;
				}
			}
		}
		printf("\n");
	}

	for(int i = 0; i < WIDTH; i++){
		printf("(%d ) ", wind[i]);
	}
}

int main(){
	system("chcp 65001");
	system("cls");
	float*** qTable = initializeQTable();
	int wind[WIDTH] = {0, 0, 0, 1, 1, 1, 2, 2, 1, 0};
	int runs = 1000000;
	int episodes = 1;

	for(int episode = 0; episode < episodes; episode++){
		for(int i = 0; i < runs; i++){
			runEpisode(qTable, wind);
		}
	}

	printActionMap(qTable, wind);
	freeQTable(qTable);
	free(qTable);
}