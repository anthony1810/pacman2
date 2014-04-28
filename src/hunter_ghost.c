#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "ghost_character.h"

enum Hunter_Directions{
	Up,
	Down,
	Left,
	Right
};

enum Hunter_Directions get_hunter_direction(){
	int r = rand() % 5;
	enum Hunter_Directions hunter_directions;
	if(r==1){
		hunter_directions = Up;
	}else if(r==2){
		hunter_directions = Down;
	}else if(r==3){
		hunter_directions = Left;
	}else{
		hunter_directions = Right;
	}
	return hunter_directions;
}

int is_wall_ahead(WINDOW *game_window, int x_pos, int y_pos){
	int result;
	int characters = (int)mvwinch(game_window, y_pos, x_pos);
	//if pacman
	if(characters==4194912){
		result = 0;
	}else
	//if ghost
	if(characters==4195169){
		result = 0;
	}else
	//if pellet
	if(characters==4195710){
		result = 0;
	}else
	//if power pellet
	if(characters==4195686){
		result = 0;
	}else
	//if fruit
	if(characters==4195453){
		result = 0;
	}else{
		result = 1;
	}

	return result;
}

void start_hunter(WINDOW *game_window, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char){
	int new_x;
	int new_y;
	//enum Hunter_Directions hunter_directions;
	if((enum Hunter_Directions)get_hunter_direction == Up){
		new_x = 0;
		new_y = 1;
	}else if((enum Hunter_Directions)get_hunter_direction == Down){
		new_x = 0;
		new_y = -1;
	}else if((enum Hunter_Directions)get_hunter_direction == Left){
		new_x = -1;
		new_y = 0;
	}else{
		new_x = 1;
		new_y = 0;
	}

	if(is_wall_ahead == 0){
		mvwaddch(game_window, my_ghost_char->ghost_col+new_y, my_ghost_char->ghost_row+new_x, ' ');
		mvwaddch(game_window, my_ghost_char->ghost_col+new_y, my_ghost_char->ghost_row+new_x, ACS_CKBOARD);
	}else {
		start_hunter(game_window, my_ghost_char, my_pacman_char);
	}
	wrefresh(game_window);
}


int main(){
	initscr();
	WINDOW *game_window;
	game_window = newwin(10, 10, 1, 1);
	refresh();
	struct ghost_char *my_ghost_char=create_ghost_char();
	my_ghost_char->ghost_row=1;
	my_ghost_char->ghost_col=1;
	mvwaddch(game_window,1,1, ACS_CKBOARD);
	wrefresh(game_window);

	// for(int i=0; i<=10;i++){
	// 	start_hunter(game_window,my_ghost_char);
	// }
	endwin();
	return 0;
}
