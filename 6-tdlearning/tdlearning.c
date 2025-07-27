#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Applying TD(0)*/

void randomWalk(int* pos){
	int i = rand() % 2; // 0->Left | 1->Right
	if(!i){
		(*pos)--;
		return;
	}
	(*pos)++;
}

void TDupdate(float* qvalues, float stepsize, float gamma, float r, int s, int ns){
	qvalues[s] += stepsize*(r + gamma * qvalues[ns] - qvalues[s]);
};

float* randomWalkTemporalDifference(unsigned int runs, float a){
	float* qvalues;
	qvalues = malloc(sizeof(float)*7);
	for(int i = 1; i < 6; i++){
		qvalues[i] = 0.5;
	}
	qvalues[0]=0;qvalues[6]=0;

	float gamma = 1;

	for(int i = 0; i < runs; i++){

		//Restarting values...
		int pos = 3;
		int oldpos = 3;

		//Main random walk loop
		while(1){

			oldpos = pos;
			randomWalk(&pos);
			if(pos < 1 || pos > 5){

				//Verifying terminal states left and right
				if(pos == 6){
					TDupdate(qvalues, a, gamma, 1, oldpos, pos);
					break;
				}
				TDupdate(qvalues, a, gamma, 0, oldpos, pos);
				break;
			}

			TDupdate(qvalues, a, gamma, 0, oldpos, pos);
		}
	}
	return qvalues;
}

int main(){
	float qreal[7] = {0, 1.0/6, 2.0/6, 3.0/6, 4.0/6, 5.0/6, 0};
	float qestimated[] = {0, 0, 0, 0, 0, 0, 0};
	int episodes;
	int runs;
	float stepsize;
	float rsme_total = 0;

	printf("Episodes: ");
	scanf("%d", &episodes);

	printf("Runs: ");
	scanf("%d", &runs);

	printf("Stepsize: ");
	scanf("%f", &stepsize);

	for(int run = 0; run < runs; run++){
		float* qvalues = randomWalkTemporalDifference(episodes, stepsize);
		float rsme = 0;

		for(int j = 0; j < 7; j++){
			rsme += (float)pow(qreal[j] - qvalues[j], 2)/5.0;
			qestimated[j] += qvalues[j];
		}
		rsme_total += sqrt(rsme);

		free(qvalues);
	}

	printf("Episodes per run: %d | Runs: %d\n", episodes, runs);
	printf("Estimated state-values:\n");

	for(int i = 0; i < 6; i++){
		printf("%f, ", qestimated[i]/runs);
	}

	printf("%f\n", qestimated[7]/runs);

	printf("Real state-values:\n");

	for(int i = 0; i < 6; i++){
		printf("%f, ", qreal[i]);
	}

	printf("%f\n", qreal[7]);

	printf("RSME mean: %f", rsme_total/(float)runs);
}
