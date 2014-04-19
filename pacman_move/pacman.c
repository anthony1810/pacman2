#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "pacman.h"
#define PELLET_SCORE 100
#define SUPER_PELLET_SCORE 200
#define FRUIT_SCORE 300
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
struct pacman *create_pacman(){
    struct pacman *my_pacman= malloc(sizeof(struct pacman));
    return my_pacman;
}
int get_score(int row,int col,char map[35][66]){
    switch (map[row][col]){
        case 's':
            return PELLET_SCORE;
            break;
        case 'S':
            return SUPER_PELLET_SCORE;
            break;
        case 'f':
        case 'F':
            return FRUIT_SCORE;
            break;
        case ' ':
            return 0;
            break;
        default:
            return -1;
            break;
    }
}
void pacman_move(struct pacman *my_pacman,char map [35][66]){
    switch (my_pacman->current_direction){
        case DOWN:
            my_pacman->pac_row++;
            if(get_score(my_pacman->pac_row,my_pacman->pac_col,map)>=0){
                my_pacman->score+=get_score(my_pacman->pac_row,my_pacman->pac_col,map);
                map[my_pacman->pac_row][my_pacman->pac_col]='P';
                map[--my_pacman->pac_row][my_pacman->pac_col]=' ';
                my_pacman->pac_row++;
            }else{
                my_pacman->pac_row--;
            }
            break;
        case UP:
            my_pacman->pac_row--;
            if(get_score(my_pacman->pac_row,my_pacman->pac_col,map)>=0){
                my_pacman->score+=get_score(my_pacman->pac_row,my_pacman->pac_col,map);
                map[my_pacman->pac_row][my_pacman->pac_col]='P';
                map[++my_pacman->pac_row][my_pacman->pac_col]=' ';
                my_pacman->pac_row--;
            }else{
                my_pacman->pac_row++;
            }
            break;
        case LEFT:
            my_pacman->pac_col--;
            if(get_score(my_pacman->pac_row,my_pacman->pac_col,map)>=0){
                my_pacman->score+=get_score(my_pacman->pac_row,my_pacman->pac_col,map);
                map[my_pacman->pac_row][my_pacman->pac_col]='P';
                map[my_pacman->pac_row][++my_pacman->pac_col]=' ';
                my_pacman->pac_col--;
            }else{
                my_pacman->pac_col++;
            }
            break;
        case RIGHT:
            my_pacman->pac_col++;
            if(get_score(my_pacman->pac_row,my_pacman->pac_col,map)>=0){
                my_pacman->score+=get_score(my_pacman->pac_row,my_pacman->pac_col,map);
                map[my_pacman->pac_row][my_pacman->pac_col]='P';
                map[my_pacman->pac_row][--my_pacman->pac_col]=' ';
                my_pacman->pac_col++;
            }else{
                my_pacman->pac_col--;
            }
            break;
    };
}

