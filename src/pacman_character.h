#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H
#include "ghost_character.h"
#include "struct.h"
void pacman_char_move(struct pacman_char *my_pacman_char,int map_col,char map [][map_col],struct map *my_map,WINDOW *game_window);
int get_score(int row,int col,int map_col,char map[][map_col],struct map *my_map,int is_invul);
int pacman_dead(struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char);
#endif
