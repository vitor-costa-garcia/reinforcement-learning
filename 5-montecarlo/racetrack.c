#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TRACK_HEIGHT 20
#define TRACK_WIDTH 10

#define WALL 1
#define PATH 0
#define START 2
#define END 3

#define MAX_V 4

typedef struct {
    int map[TRACK_HEIGHT][TRACK_WIDTH];
    int* start = malloc(sizeof(int)*2);
    int* end = malloc(sizeof(int)*2);
} Track;

typedef struct {
    int* pos = malloc(sizeof(int)*2);
    int* v = malloc(sizeof(int)*2);
} Kart;

void generateTrack(int track[TRACK_HEIGHT][TRACK_WIDTH], int start_x, int start_y, int end_x, int end_y){
    for(int i = 0; i < TRACK_HEIGHT; i++){
        for(int j = 0; j < TRACK_WIDTH; j++){
            if(i == 0 || j == 0 || i == TRACK_HEIGHT - 1 || j == TRACK_WIDTH - 1){
                track[i][j] = WALL;
            } else {
                track[i][j] = PATH;
            }
        }
    }

    if(start_x == 0 || start_x == TRACK_WIDTH-1 || start_y == 0 || start_y == TRACK_HEIGHT-1){
        track[start_y][start_x] = START;
    } else {
        return;
    }

    if(end_x == 0 || end_x == TRACK_WIDTH-1 || end_y == 0 || end_y == TRACK_HEIGHT-1){
        track[end_y][end_x] = END;
    } else {
        return;
    }

    for(int x = 4; x < TRACK_HEIGHT-1; x++){track[x][1] = WALL;}
    for(int x = 7; x < TRACK_HEIGHT-1; x++){track[x][2] = WALL;}
    for(int x = 10; x < TRACK_HEIGHT-1; x++){track[x][3] = WALL;}
    for(int x = 14; x < TRACK_HEIGHT-1; x++){track[x][4] = WALL;}
    for(int x = 17; x < TRACK_WIDTH-1; x++){track[x][5] = WALL;}
    for(int x = 2; x > 0; x--){track[x][6] = WALL;}
    for(int x = 4; x > 0; x--){track[x][7] = WALL;}
    for(int x = 12; x > 0; x--){track[x][8] = WALL;}
}

void showTrack(int track[TRACK_HEIGHT][TRACK_WIDTH]){
    for(int i = 0; i < TRACK_HEIGHT; i++){
        for(int j = 0; j < TRACK_WIDTH; j++){
            printf("%d ",track[i][j]);
        }
        printf("\n");
    }
}

void runEpisode(Track track, int policy[TRACK_HEIGHT][TRACK_WIDTH][MAX_V + 1][MAX_V + 1][2]){
    int available_actions[][2] = {{-1, -1},{0, -1},{1, -1},{-1, 0},{0, 0},{1, 0},{-1, 1},{0, 1},{1, 1}};
    Kart kart;

    //Initialize initial values
    kart.pos = track.start;
    kart.v = {0, 0};

    //Grab Policy action
    int* action = policy[current_pos[1]][current_pos[0]][current_v[1]][current_v[0]];

    //Accelerating 
    current_pos[0] += action[0];
    current_pos[1] += action[1];

}

int main(){
    Track track;
    generateTrack(track.map, 0, 1, 8, 19);
    showTrack(track.map);
}