#ifndef HUNTER_GHOST_H
#define HUNTER_GHOST_H

#include "struct.h"
#include "constant.h"
#include "write_file.h"

/*! @file write_file.h 
	@brief this is the header file of \ref hunter_ghost.c.
	@author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/


/*! 
	@brief this struct used to save the character that hunter walk by so that hunter won't eat item such as another ghost, pellet, ..
	
*/

struct Item_Struct{
	int x_pos;
	int y_pos;
	int value;
};

/*! 
	@brief this enumeration used to define direction of hunter
	
*/

enum Hunter_Directions{
	Up,
	Down,
	Left,
	Right
};

/*! 
	@brief this is the struct of hunter ghost
	
*/

struct ghost_char_2
{
	int ghost_row;
	int ghost_col;
	int speed_multiplier;
};

/*!
	&brief this is the variable which will hold the current direction of hunter
*/
enum Hunter_Directions get_hunter_direction();

/*!
	this function will responsible to inform hunter that he and pacman is collise
*/
int getHunterCollision();

/*!
	this function will reset whatever boolean value of hunter and pacman collision
*/
void hunter_reset_colision();

/*!
	this function will reset whatever boolean value of hunter and pacman collision
*/

void hunter_setDirection(enum Hunter_Directions first_direction);
void hunter_setItemStruct(struct Item_Struct *item);
void hunter_setDurationBuildWalls(int new_duration);
void set_isFirstTime();
void hunter_setVisionLength(int new_length);
void hunter_setDefendOriginChar(struct Item_Struct *new_upper_right_struct,struct Item_Struct *new_upper_struct, struct Item_Struct *new_left_struct,
struct Item_Struct *new_upper_left_struct, struct Item_Struct *new_lower_left_struct, struct Item_Struct *new_lower_right_struct, struct Item_Struct *new_down_struct, 
struct Item_Struct *new_right_struct);
int is_wall_ahead(WINDOW *game_window, WINDOW *title, int x_pos, int y_pos);
WINDOW *create_newwin(int height, int width, int starty, int startx);
bool is_pacman_near(struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);
void movexy(WINDOW *game_window, struct ghost_char_2 *my_ghost_char,struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct, int des_x, int des_y);
void chase_pacman(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct);
void hunter_defend(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);
void restore_after_defend(WINDOW *game_window,WINDOW *info);
void hunter_move(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct,
					int map_col,char map[][map_col+1]);
void start_hunter(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);
struct ghost_char_2 *create_ghost_char2();
struct Item_Struct *create_item_struct();

void update_map_array(int map_col,char map[][map_col+1],WINDOW *game_window);
#endif