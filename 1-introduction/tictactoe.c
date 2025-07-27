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
1  -> X
0  -> O
-1 -> Empty
*/

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




int main(){
	int* board = malloc(sizeof(int)*9);
	for(int i = 0; i < 9; i++){board[i]=-1;};
	board[0]=1;
	board[4]=1;
	board[8]=1;

	printBoard(board);
	cannonical_state(board);
	printBoard(board);
}










