#include <stdio.h>
#include <stdlib.h>

#define a 0.1
#define gamma 1

/*
Environment dynamics
A->A' = R->10 Can imagine like a robot picking up a box in point A then taking it to point A'. since its further than B->B', recieves a higher reward
B->B' = R->5 Same thing as above, but a closer path.
*/

int main(){
	//State-values of each position
	float[][5] gridworld = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	}

	//Position of A and A'
	posA[2] = {0,1};
	posAi[2] = {4, 1};

	//Position of B and B'
	posB[2] = {0,3};
	posBi[2] = {2, 3};

	//I'll use the Q-learning algorithm to solve this gridworld
	void QLearningUpdate(float r, int* s, int* olds){
		return;
	};
}