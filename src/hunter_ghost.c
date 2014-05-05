#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "hunter_ghost.h"

int HUNTER_SPEED =400000;
int isFirstTime = 1;
int move_away = 0;
enum Hunter_Directions hunter_directions;
struct Item_Struct my_item_struct;

void hunter_setDirection(enum Hunter_Directions first_direction){
	hunter_directions = first_direction;
}

void hunter_setItemStruct(struct Item_Struct *item){
	my_item_struct = *item;
}

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

enum Hunter_Directions get_hunter_direction(){

	int r, i;
    srand(time(NULL));  // Seed ONCE

    for(i=0; i<100; ++i) // Loop 100 times for random numbers
    {
        r = rand();
        r = r % 5;
    }

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


int is_wall_ahead(WINDOW *game_window,WINDOW *title, int x_pos, int y_pos){
	int result = 1;
	int characters = (int)mvwinch(game_window, x_pos, y_pos);

	if(characters==4195169 || characters==4195710 || characters==4195686 || characters==4195453 || 
		characters==32 || characters==288 ||characters == 4194912){
		result = 0;
	}

	return result;
}


bool is_pacman_near(struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char){

	int row = my_ghost_char->ghost_row - my_pacman_char->pac_row;
	int col = my_ghost_char->ghost_col - my_pacman_char->pac_col;
	if(abs(row)<=10 && abs(col)<=10){
		return true;
	}else{
		return false;
	}

}

void movexy(WINDOW *game_window, struct ghost_char_2 *my_ghost_char,struct Item_Struct *my_item_struct, int des_x, int des_y){

	my_ghost_char->ghost_col+=des_x;
	my_ghost_char->ghost_row+=des_y;
	mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
			

	my_item_struct->x_pos = my_ghost_char->ghost_col;
	my_item_struct->y_pos = my_ghost_char->ghost_row;

	int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

	my_item_struct->value = characters;
	mvwaddch(game_window, my_ghost_char->ghost_row, my_ghost_char->ghost_col, ACS_CKBOARD);

	wrefresh(game_window);
}

void hunter_defend(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char){

	// if(my_ghost_char->ghost_row>my_pacman_char->pac_row){
	// 	mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row-1, ACS_RTEE);
	// 	wrefresh(game_window);
	// }else{
	// 	mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row+1, ACS_LTEE);
	// 	wrefresh(game_window);
	// }

	// if(my_ghost_char->ghost_row>my_pacman_char->pac_row){
	// 	mvwaddch(game_window,my_ghost_char->ghost_col-1,my_ghost_char->ghost_row, ACS_HLINE);
	// 	wrefresh(game_window);
	// }else{
	// 	mvwaddch(game_window,my_ghost_char->ghost_col+1,my_ghost_char->ghost_row, ACS_HLINE);
	// 	wrefresh(game_window);
	// }

	// mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row+1, ACS_VLINE);

	// wrefresh(game_window);
	// mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row-1, ACS_VLINE);
	// wrefresh(game_window);

	// mvwaddch(game_window,my_ghost_char->ghost_col+1,my_ghost_char->ghost_row, ACS_HLINE);
	// wrefresh(game_window);

	// mvwaddch(game_window,my_ghost_char->ghost_col-1,my_ghost_char->ghost_row, ACS_HLINE);
	// wrefresh(game_window);
	// int up = (int)mvwinch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row);
	// int down = (int)mvwinch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row);
	// int left = (int)mvwinch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row);
	// int right = (int)mvwinch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row);

		mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col+1, ACS_URCORNER);
		mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col, ACS_HLINE);
		mvwaddch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col-1, ACS_VLINE);
		mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col-1, ACS_ULCORNER);

		mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col+1, ACS_LRCORNER);
		mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col, ACS_HLINE);
		mvwaddch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col+1, ACS_VLINE);
		mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col-1, ACS_LLCORNER);
		
		wrefresh(game_window);

}

void chase_pacman(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct){

	int ghost_x = my_ghost_char->ghost_col;
	int pac_x = my_pacman_char->pac_col;
	int ghost_y = my_ghost_char->ghost_row;
	int pac_y = my_pacman_char->pac_row;

	if(pac_x==ghost_x && pac_y==ghost_y){
		wclear(title);
		wprintw(title, "Finish!");
		wrefresh(title);
	}

	if(pac_x > ghost_x){
			ghost_x++;
			if(is_wall_ahead(game_window, title, ghost_y, ghost_x)==0){
				movexy(game_window, my_ghost_char,my_item_struct, 1, 0);
			}else{
				ghost_x--;
			}

	}



	if(pac_x < ghost_x){
			ghost_x--;
			if(is_wall_ahead(game_window,title,  ghost_y, ghost_x)==0){
				movexy(game_window, my_ghost_char,my_item_struct, -1, 0);
			}else{
				ghost_x++;
			}	

	}

	if(pac_y < ghost_y){
			ghost_y--;
			if(is_wall_ahead(game_window, title, ghost_y, ghost_x)==0){
				movexy(game_window, my_ghost_char,my_item_struct, 0, -1);
			}else{
				ghost_y++;
			}		
	}


	if(pac_y > ghost_y){
			ghost_y++;
			if(is_wall_ahead(game_window,title,  ghost_y, ghost_x)==0){
				movexy(game_window, my_ghost_char,my_item_struct, 0, 1);
			}else{
				ghost_y--;
			}		

	}
	
}


void hunter_move(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct){
	//my_pacman_char->pac_state = 1;
	if(is_pacman_near(my_ghost_char, my_pacman_char)  && my_pacman_char->pac_state == 0){
		// wclear(title);
		// wprintw(title, "Ghost: %ix%i (Pacman: %ix%i)",my_ghost_char->ghost_row, my_ghost_char->ghost_col, my_pacman_char->pac_row,my_pacman_char->pac_col);
		// wrefresh(title);
		//my_item_struct->value = ' ';
		chase_pacman(game_window,title, my_ghost_char, my_pacman_char,my_item_struct);
	}
	// else{
	// 	wclear(title);
	// 	wprintw(title, "Ghost: %ix%i (Pacman: %ix%i)",my_ghost_char->ghost_row, my_ghost_char->ghost_col, my_pacman_char->pac_row,my_pacman_char->pac_col);
	// 	wrefresh(title);
	// }
	if(my_pacman_char->pac_state == 1){
		hunter_defend(game_window, my_ghost_char, my_pacman_char);
	}else{

		int new_x = 0;
		int new_y = 0;
		
		if(hunter_directions == Down){
			new_x = 0;
			new_y = 1;
		}else if(hunter_directions == Up){
			new_x = 0;
			new_y = -1;
		}else if(hunter_directions == Left){
			new_x = -1;
			new_y = 0;
		}else{
			new_x = 1;
			new_y = 0;
		}
		int new_movex = my_ghost_char->ghost_col + new_x;
		int new_movey = my_ghost_char->ghost_row + new_y;



		if(is_wall_ahead(game_window,title, new_movey,new_movex)==0){

			if(isFirstTime==1){
				isFirstTime = 0;
				mvwaddch(game_window, my_ghost_char->ghost_row, my_ghost_char->ghost_col, ' ');
			}

			int characters = (int)mvwinch(game_window, new_movey, new_movex);
			mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);

			my_ghost_char->ghost_row+=new_y;
			my_ghost_char->ghost_col+=new_x;
			


			my_item_struct->x_pos = new_movex;
			my_item_struct->y_pos = new_movey;

			if(my_item_struct->x_pos == my_pacman_char->pac_row && my_item_struct->y_pos == my_pacman_char->pac_col){
				my_item_struct->value = ' ';
			}else{
				my_item_struct->value = characters;
			}
			mvwaddch(game_window, my_ghost_char->ghost_row, my_ghost_char->ghost_col, ACS_CKBOARD);

		}else{

			enum Hunter_Directions new_hunter_direction = get_hunter_direction();
			while(hunter_directions==new_hunter_direction){
				new_hunter_direction = get_hunter_direction();
			}
			hunter_setDirection(new_hunter_direction);
		}
	}
}

struct ghost_char_2 *create_ghost_char2(){
    struct ghost_char_2 *my_ghost_char= malloc(sizeof(struct ghost_char_2));
    return my_ghost_char;
}


