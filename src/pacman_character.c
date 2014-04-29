#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include "utility.h"
#include "pacman_character.h"
#include "constant.h"


int get_score(int row,int col,int map_col,char map[][map_col],struct map *my_map,int is_invul){
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
        case 'g':
        case 'G':
            return (is_invul==1) ? GHOST_SCORE :-1;
            break;
        case ' ':
            return 0;
            break;
        default:
            return -1;
            break;
    }
}
struct timeval pacman_invul_start,pacman_invul_end;
void pacman_char_move(struct pacman_char *my_pacman_char,int map_col,char map [][map_col],struct map *my_map,WINDOW *game_window){
    switch (my_pacman_char->current_direction){
        case DOWN:
            my_pacman_char->pac_row++;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state);
                if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==PELLET_SCORE){
                    my_map->remain_pellet--;
                }else if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==SUPER_PELLET_SCORE){
                    my_pacman_char->pac_state=INVULRABLE;
                    gettimeofday(&pacman_invul_start,NULL);
                }
                wattron(game_window,COLOR_PAIR(2));
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,ACS_DIAMOND);
                wattron(game_window,COLOR_PAIR(1));
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[--my_pacman_char->pac_row][my_pacman_char->pac_col]=' ';
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,' ');
                my_pacman_char->pac_row++;
            }else{
                my_pacman_char->pac_row--;
            }
            break;
        case UP:
            my_pacman_char->pac_row--;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state);
                if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==PELLET_SCORE){
                    my_map->remain_pellet--;
                }else if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==SUPER_PELLET_SCORE){
                    my_pacman_char->pac_state=INVULRABLE;
                    gettimeofday(&pacman_invul_start,NULL);
                }
                wattron(game_window,COLOR_PAIR(2));
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,ACS_DIAMOND);
                wattron(game_window,COLOR_PAIR(1));
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[++my_pacman_char->pac_row][my_pacman_char->pac_col]=' ';
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,' ');
                my_pacman_char->pac_row--;
            }else{
                my_pacman_char->pac_row++;
            }
            break;
        case LEFT:
            my_pacman_char->pac_col--;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state);
                if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==PELLET_SCORE){
                    my_map->remain_pellet--;
                }else if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==SUPER_PELLET_SCORE){
                    my_pacman_char->pac_state=INVULRABLE;
                    gettimeofday(&pacman_invul_start,NULL);
                }
                wattron(game_window,COLOR_PAIR(2));
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,ACS_DIAMOND);
                wattron(game_window,COLOR_PAIR(1));
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[my_pacman_char->pac_row][++my_pacman_char->pac_col]=' ';
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,' ');
                my_pacman_char->pac_col--;
            }else{
                my_pacman_char->pac_col++;
            }
            break;
        case RIGHT:
            my_pacman_char->pac_col++;
            if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)>=0){
                my_pacman_char->score+=get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state);
                if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==PELLET_SCORE){
                    my_map->remain_pellet--;
                }else if(get_score(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col,map,my_map,my_pacman_char->pac_state)==SUPER_PELLET_SCORE){
                    my_pacman_char->pac_state=INVULRABLE;
                    gettimeofday(&pacman_invul_start,NULL);
                }
                wattron(game_window,COLOR_PAIR(2));
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,ACS_DIAMOND);
                wattron(game_window,COLOR_PAIR(1));
                map[my_pacman_char->pac_row][my_pacman_char->pac_col]='P';
                map[my_pacman_char->pac_row][--my_pacman_char->pac_col]=' ';
                mvwaddch(game_window,my_pacman_char->pac_row,my_pacman_char->pac_col,' ');
                my_pacman_char->pac_col++;
            }else{
                my_pacman_char->pac_col--;
            }
            break;
    };
    gettimeofday(&pacman_invul_end,NULL);
    if(timeval_diff(NULL,&pacman_invul_end,&pacman_invul_start)>INVULRABLE_DURATION){
        my_pacman_char->pac_state=VULRABLE;
    }
}
int pacman_dead(struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char){
    for (int i = 0; i < 4; ++i)
    {
        if(my_pacman_char->pac_row==my_ghost_char[i].ghost_row
            &&my_pacman_char->pac_col==my_ghost_char[i].ghost_col && my_pacman_char->pac_state==VULRABLE){
            //dead
            return 4;
        }else if(my_pacman_char->pac_row==my_ghost_char[i].ghost_row
            &&my_pacman_char->pac_col==my_ghost_char[i].ghost_col && my_pacman_char->pac_state==INVULRABLE){
            //return dead ghost's number 
            return i;
        }
    }
    //NOTHING
    return 5;
}

