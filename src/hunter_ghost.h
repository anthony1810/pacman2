#ifndef HUNTER_GHOST_H
#define HUNTER_GHOST_H

#include "struct.h"
#include "constant.h"
#include "write_file.h"

/*! \file write_file.h 
	\brief this is the header file of \ref hunter_ghost.c.
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/


/*! 
	\brief this struct used to save the character that hunter walk by so that hunter won't eat item such as another ghost, pellet, ..
	
*/

struct Item_Struct{
	int x_pos;
	int y_pos;
	int value;
};

/*! 
	\brief this enumeration used to define direction of hunter.
	
*/

enum Hunter_Directions{
	Up,
	Down,
	Left,
	Right
};

/*! 
	\brief this is the struct of hunter ghost.
	
*/

struct ghost_char_2
{
	int ghost_row;
	int ghost_col;
	int speed_multiplier;
};

/*!
	@\brief this is the variable which will hold the current direction of hunter.
*/
enum Hunter_Directions get_hunter_direction();

/*!
	\brief this function will responsible to inform hunter that he and pacman is collise.
*/
int getHunterCollision();

/*!
	\brief this function will reset whatever collision value of hunter and pacman collision
*/
void hunter_reset_colision();

/*! \brief this function use to set the different direction to the local direction variable.
 *
 *	\param new_direction the enum Hunter_Directions type.
*/
void hunter_setDirection(enum Hunter_Directions new_direction);

/*! \brief this function use to set the local item struct type.
 *  \details Item_struct is a struct use to store the x,y position and the characters that hunter walk by. I store it in a struct to easily restore it when hunter walk away.
 *
 *	\param *item is a pointer to a new struct of Item_Struct type.
*/
void hunter_setItemStruct(struct Item_Struct *item);

/*! \brief this function use to set the local variable wall_duration.
 *  \details this wall duration variable will determine how long before hunter starts to build walls surrounding him.
 *
 *	\param new_duration int value of new duration.
*/
void hunter_setDurationBuildWalls(int new_duration);

/*! \brief this function use to set the value of local variable isFirstTime..
 *
 *  \details isFirstTime variable determine whether hunter start to move for the first time or not, 
 *   so that i don't have to use a struct to store what was on his feet to restore later on.
 *
*/
void set_isFirstTime();

/*! \brief this function use to set the value of local variable vision_length..
 *
 *  \details vision_length is a variable to determine how far hunter can see pacman so that he can chase him.
 *
 *	\param new_length is new int value of vision_length that can be set to.
*/
void hunter_setVisionLength(int new_length);

/*! \brief this function use to create 5 instants of Item_Struct Struct type.
 *
 *  \details when hunter build walls surrounding him, he need to keep track of what was beneath those walls so that he will be able to restore them when the walls are gone.
 *
 *	\param new_upper_right_struct Item_Struct type to keep track of what was on upper right corner of hunter.
 *	\param new_upper_struct Item_Struct type to keep track of what was on upper of hunter.
 *	\param new_right_struct Item_Struct type to keep track of what was on the right  of hunter.
 *	\param new_left_struct Item_Struct type to keep track of what was on the left of hunter.
 *	\param new_upper_left_struct Item_Struct type to keep track of what was on upper left corner of hunter.
 *	\param new_lower_left_struct Item_Struct type to keep track of what was on lower left corner of hunter.
 *	\param new_lower_right_struct Item_Struct type to keep track of what was on lower right corner of hunter.
 *	\param new_down_struct Item_Struct type to keep track of what was on down side of hunter.
*/
void hunter_setDefendOriginChar(struct Item_Struct *new_upper_right_struct,struct Item_Struct *new_upper_struct, struct Item_Struct *new_left_struct,
struct Item_Struct *new_upper_left_struct, struct Item_Struct *new_lower_left_struct, struct Item_Struct *new_lower_right_struct, struct Item_Struct *new_down_struct, 
struct Item_Struct *new_right_struct);

/*! \brief this function use to check if there is any wall in the x,y position of a WINDOW.
 *
 *	\param game_window WINDOW type, the window that will be detected.
 *	\param x_pos int type, the x position that will be detected.
 *	\param x_pos int type, the y position that will be detected.
*/
int is_wall_ahead(WINDOW *game_window, WINDOW *title, int x_pos, int y_pos);

/*! \brief this function use to check if pacman is near.
 *
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
*/
bool is_pacman_near(struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);

/*! \brief this function use to move hunter to a specific location.
 *
 *	\param game_window the window that move in.
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
 *	\param my_item_struct the pointer to the struct that use to store items that hunter step on.
 *	\param des_x destionation x.
 *	\param des_y destionation y.
*/
void movexy(WINDOW *game_window, struct ghost_char_2 *my_ghost_char,struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct, int des_x, int des_y);

/*! \brief this function use to let hunter chase pacman if pacman is near.
 *
 *	\param game_window the window that move in.
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
 *	\param my_item_struct the pointer to the struct that use to store items that hunter step on.
*/
void chase_pacman(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct);

/*! \brief this function use let hunter buil walls surround him to defend.
 *
 *	\param game_window the window that move in.
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
*/
void hunter_defend(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);

/*! \brief this function use let hunter buil walls surround him to defend.
 *
 *	\param game_window the window that move in.
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
*/
void restore_after_defend(WINDOW *game_window,WINDOW *info);

/*! \brief this is the main fuction of hunter where hunter actually move, chase or defend. 
 *
 *	\param game_window the window that move in.
 *	\param my_ghost_char the pointer to the struct of ghost.
 *	\param my_pacman_char the pointer to the struct of pacman.
 *	\param my_item_struct the pointer to the struct that use to store items that hunter step on.
 * 	\param map the static map contains all characters including ghost, pacman, pellet,....
 *  \map_col int value, the number of collumns of the static  map
*/
void hunter_move(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct,
					int map_col,char map[][map_col+1]);

/*! \brief this function use create hunter struct.
 *
*/
struct ghost_char_2 *create_ghost_char2();


/*! \brief this function use create Item_Struct.
 *
*/
struct Item_Struct *create_item_struct();


/*! \brief this function use update what happens so far and add it to the static map.
 *
 * 	\param map the static map contains all characters including ghost, pacman, pellet,....
 *  \map_col int value, the number of collumns of the static  map
 *	\param game_window the window that move in.
*/
void update_map_array(int map_col,char map[][map_col+1],WINDOW *game_window);
#endif