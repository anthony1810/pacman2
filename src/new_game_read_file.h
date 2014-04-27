#ifndef NEW_GAME_READ_FILE_H
#define NEW_GAME_READ_FILE_H
#include "pacman_character.h"
#include "ghost_character.h"
/*! @file read_file.h 
    @brief this is the header file of \ref read_file.c.
*/
/*!
@brief readFile,get_author_name_and_email function will be called within.
*/
void new_game_read_file(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[],struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char
	,struct map *my_map);

void new_game_update_map(WINDOW *map_win,int row,int col,char map[row][col]);
#endif
