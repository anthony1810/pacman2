#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H
#include "ghost_character.h"
#include "struct.h"
/*! @file pacman_character.h 
    @brief this is the header file of \ref pacman_character.c.
*/
/*!
@brief moving pacman 4 direction and update score, invulrable duration
 @param *my_pacman_char the pacman 's struct pointer
 @param *my_map the map's struct pointer for updating the pellet count
*/
void pacman_char_move(struct pacman_char *my_pacman_char,int map_col,char map [][map_col],struct map *my_map,WINDOW *game_window);

/*!
@brief check wall and return the score as the same time
*/
int get_score(int row,int col,int map_col,char map[][map_col],struct map *my_map,int is_invul);
/*!
@brief check the ghost eat pacman by comparing the position of ghost and pacman and pacman status
	\return 4 if pacman dead, 0,1,2,3 correspoding to ghost dead, 5 if nothing
*/
int pacman_dead(struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char);
#endif
