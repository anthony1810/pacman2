#ifndef NEW_GAME_READ_FILE_H
#define NEW_GAME_READ_FILE_H
#include "pacman_character.h"
#include "ghost_character.h"
#include "struct.h"
/*! @file new_game_read_file.h 
    @brief this is the header file of \ref new_game_read_file.c.
*/
/*!
@brief read the file and print it on screen, this is basically same as read_file.c but it take more parameters

*/
void new_game_read_file(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[],struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char
	,struct map *my_map);
/*!
@brief re-paint the screen if the map array is updated
*/
void new_game_update_map(WINDOW *map_win,int row,int col,char map[row][col]);
#endif
