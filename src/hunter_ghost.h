#ifndef HUNTER_GHOST_H
#define HUNTER_GHOST_H

#include "struct.h"

struct Item_Struct{
	int x_pos;
	int y_pos;
	int value;
};

enum Hunter_Directions{
	Up,
	Down,
	Left,
	Right
};

struct ghost_char_2
{
	int ghost_row;
	int ghost_col;
	int speed_multiplier;
};

enum Hunter_Directions get_hunter_direction();
void hunter_setDirection(enum Hunter_Directions first_direction);
void hunter_setItemStruct(struct Item_Struct *item);
int is_wall_ahead(WINDOW *game_window, WINDOW *title, int x_pos, int y_pos);
WINDOW *create_newwin(int height, int width, int starty, int startx);
bool is_pacman_near(struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);
void movexy(WINDOW *game_window, struct ghost_char_2 *my_ghost_char,struct Item_Struct *my_item_struct, int des_x, int des_y);
void chase_pacman(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct);
void hunter_move(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct);
void start_hunter(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char);
struct ghost_char_2 *create_ghost_char2();

#endif