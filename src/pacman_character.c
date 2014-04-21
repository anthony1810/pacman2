#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "pacman_character.h"
#define PELLET_SCORE 100
#define SUPER_PELLET_SCORE 200
#define FRUIT_SCORE 300
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
struct pacman_char *create_pacman_char(){
    struct pacman_char *my_pacman_char= malloc(sizeof(struct pacman_char));
    return my_pacman_char;
}
int get_score(int row,int col,int map_col,char map[][map_col]){
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
void pacman_char_move(struct pacman_char *my_pacman_char,int map_col,char map [][map_col]){
    switch (my_pacman_char->current_direction){
        case DOWN:
            my_pacman_char->pac_row++;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map);
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[--my_pacman_char->pac_row][my_pacman_char->pac_col]=' ';
                my_pacman_char->pac_row++;
            }else{
                my_pacman_char->pac_row--;
            }
            break;
        case UP:
            my_pacman_char->pac_row--;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map);
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[++my_pacman_char->pac_row][my_pacman_char->pac_col]=' ';
                my_pacman_char->pac_row--;
            }else{
                my_pacman_char->pac_row++;
            }
            break;
        case LEFT:
            my_pacman_char->pac_col--;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map);
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[my_pacman_char->pac_row][++my_pacman_char->pac_col]=' ';
                my_pacman_char->pac_col--;
            }else{
                my_pacman_char->pac_col++;
            }
            break;
        case RIGHT:
            my_pacman_char->pac_col++;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map);
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[my_pacman_char->pac_row][--my_pacman_char->pac_col]=' ';
                my_pacman_char->pac_col++;
            }else{
                my_pacman_char->pac_col--;
            }
            break;
    };
}
int pacman_dead(struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char){
    if(my_pacman_char->pac_row==my_ghost_char->ghost_row
        &&my_pacman_char->pac_col==my_ghost_char->ghost_col){
        return 1;
    }
    return 0;
}

