#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H
#include "ghost_character.h"
struct pacman_char
{
	int pac_row;
	int pac_col;
	int current_direction;
	int score;
};
struct pacman_char *create_pacman_char();
void pacman_char_move(struct pacman_char *my_pacman_char,int map_col,char map [][map_col]);
int get_score(int row,int col,int map_col,char map[][map_col]);
int pacman_dead(struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char);
#endif
