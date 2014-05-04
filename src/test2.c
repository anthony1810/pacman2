#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "test2.h"

int HUNTER_SPEED =400000;
int isFirstTime = 1;
int move_away = 0;

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


int is_wall_ahead(WINDOW *game_window, int x_pos, int y_pos){
	//wrefresh(game_window);
	int result = 1;
	int characters = (int)mvwinch(game_window, y_pos, x_pos);
	// printw("%ix%i: %i - ",y_pos,x_pos,characters);
	// refresh();
	//  getch();
	// printw("%i - ",(int)mvwinch(game_window, y_pos, x_pos));
	// refresh();
	//if pacman
	if(characters==4194912){
		result = 3;
	}
	//if ghost
	if(characters==4195169){
		result = 0;
	}
	//if pellet
	if(characters==4195710){
		result = 0;
	}
	//if power pellet
	if(characters==4195686){
		result = 0;
	}
	//if fruit
	if(characters==4195453){
		result = 0;
	}
	//if space
	if(characters==32){
		result = 0;
	}
	if(characters==4194430){
		result = 0;
	}
	if(characters==4194400){
		result = 0;
	}

	return result;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

bool is_pacman_near(struct ghost_char_2 *my_ghost_char, struct pacman_char_2 *my_pacman_char){

	int row = my_ghost_char->ghost_row - my_pacman_char->pac_row;
	int col = my_ghost_char->ghost_col - my_pacman_char->pac_col;
	if(abs(row)<=5 && abs(col)<=5){
		return true;
	}else{
		return false;
	}

}

void movexy(WINDOW *game_window, struct ghost_char_2 *my_ghost_char,struct Item_Struct *my_item_struct, int des_x, int des_y){

	my_ghost_char->ghost_col+=des_y;
	my_ghost_char->ghost_row+=des_x;
	mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
			

	my_item_struct->x_pos = my_ghost_char->ghost_row;
	my_item_struct->y_pos = my_ghost_char->ghost_col;

	int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

	my_item_struct->value = characters;
	mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
	sleep(HUNTER_SPEED);
	wrefresh(game_window);
}

void hunter_defend(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char_2 *my_pacman_char){
	sleep(1000000);
	if(my_ghost_char->ghost_row>my_pacman_char->pac_row){
		mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row-1, ACS_RTEE);
		wrefresh(game_window);
	}else{
		mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row+1, ACS_LTEE);
		wrefresh(game_window);
	}
	sleep(1000000);
	if(my_ghost_char->ghost_row>my_pacman_char->pac_row){
		mvwaddch(game_window,my_ghost_char->ghost_col-1,my_ghost_char->ghost_row, ACS_HLINE);
		wrefresh(game_window);
	}else{
		mvwaddch(game_window,my_ghost_char->ghost_col+1,my_ghost_char->ghost_row, ACS_HLINE);
		wrefresh(game_window);
	}
	sleep(1000000);
	mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row+1, ACS_VLINE);
	sleep(1000000);
	wrefresh(game_window);
	mvwaddch(game_window,my_ghost_char->ghost_col,my_ghost_char->ghost_row-1, ACS_VLINE);
	wrefresh(game_window);
	sleep(1000000);
	mvwaddch(game_window,my_ghost_char->ghost_col+1,my_ghost_char->ghost_row, ACS_HLINE);
	wrefresh(game_window);
	sleep(1000000);
	mvwaddch(game_window,my_ghost_char->ghost_col-1,my_ghost_char->ghost_row, ACS_HLINE);
	wrefresh(game_window);

}

void chase_pacman(WINDOW *game_window,WINDOW *info, struct ghost_char_2 *my_ghost_char, struct pacman_char_2 *my_pacman_char,struct Item_Struct *my_item_struct){
	int row = my_ghost_char->ghost_row - my_pacman_char->pac_row;
	int col = my_ghost_char->ghost_col - my_pacman_char->pac_col;

	int ghost_x = my_ghost_char->ghost_row;
	int pac_x = my_pacman_char->pac_row;
	int ghost_y = my_ghost_char->ghost_col;
	int pac_y = my_pacman_char->pac_col;


	if(pac_y > ghost_y){
		//printw("1");
		refresh();
		while(pac_y != ghost_y){
			ghost_y++;
			if(is_wall_ahead(game_window, ghost_x, ghost_y)==0){
				movexy(game_window, my_ghost_char,my_item_struct, 0, 1);
			}else{
				ghost_y--;
				break;
			}		
		}

	}

	if(pac_y < ghost_y){
		//printw("2");
		refresh();
		while(pac_y != ghost_y){
			ghost_y--;
			if(is_wall_ahead(game_window, ghost_x, ghost_y)==0){
				movexy(game_window, my_ghost_char,my_item_struct, 0, -1);
			}else{
				ghost_y++;
				break;
			}		
		}

	}


	if(pac_x > ghost_x){
		//printw("3");
		refresh();
		while(pac_x != ghost_x){
			ghost_x++;
			if(is_wall_ahead(game_window, ghost_x, ghost_y)==0){
				if(abs(my_pacman_char->pac_row - my_ghost_char->ghost_row)==1){
					wclear(info);
					wprintw(info, "Waiting to strike by 3");
					wrefresh(info);
					sleep(5000000);
				}
				movexy(game_window, my_ghost_char,my_item_struct, 1, 0);
			}else{
				ghost_x--;
				break;
			}
		}

	}

	if(pac_x < ghost_x){
		//printw("4");
		refresh();
		while(pac_x != ghost_x){
			ghost_x--;
			if(is_wall_ahead(game_window, ghost_x, ghost_y)==0){
				if(abs(my_pacman_char->pac_row - my_ghost_char->ghost_row)==1){
					wclear(info);
					wprintw(info, "Waiting to strike by 4");
					wrefresh(info);
					sleep(5000000);
				}
				movexy(game_window, my_ghost_char,my_item_struct, -1, 0);
			}else{
				ghost_x++;
				break;
			}	
		}

	}

	wclear(info);
	wprintw(info, "Attempt to destroy the wall");
	wrefresh(info);
	sleep(1000000);
	// move_away = 1;
	//hunter_defend(game_window,my_ghost_char, my_pacman_char);
}


void hunter_move(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char_2 *my_pacman_char,struct Item_Struct *my_item_struct){
	WINDOW *info;
	info = create_newwin(1,100,23,0);
	refresh(); 

	if(is_pacman_near(my_ghost_char, my_pacman_char)){
		wclear(info);
		wprintw(info, "Hunter saw pacman...Hunter start chasing.");
		wrefresh(info);
		HUNTER_SPEED = 200000;
		my_item_struct->value = ' ';
		chase_pacman(game_window,info, my_ghost_char, my_pacman_char,my_item_struct);
	}else{
		HUNTER_SPEED = 400000;
		wclear(info);
		wprintw(info, "Hunter's doing a Patrol.");
		wrefresh(info);
		//move_away = 0;
		//getch();
	}


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
	int new_movex = my_ghost_char->ghost_row + new_x;
	int new_movey = my_ghost_char->ghost_col + new_y;
	int characters = (int)mvwinch(game_window, new_movey, new_movex);
	wclear(info);
	wprintw(info, "%ix%i: %i",new_movex,new_movey,characters);
	wrefresh(info);
	getch();
	// wprintw(game_window, "%ix%i",my_ghost_char->ghost_row + new_x,my_ghost_char->ghost_col + new_y );
	// wrefresh(game_window);

	if(is_wall_ahead(game_window,new_movey,new_movex)==0){
		// wprintw(game_window, "%ix%i",my_ghost_char->ghost_row + new_x,my_ghost_char->ghost_col + new_y );
		// wrefresh(game_window);

		if(isFirstTime==1){
			isFirstTime = 0;
			mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ' ');
		}

		int characters = (int)mvwinch(game_window, new_movey, new_movex);
		mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);

		my_ghost_char->ghost_col+=new_y;
		my_ghost_char->ghost_row+=new_x;


		my_item_struct->x_pos = new_movex;
		my_item_struct->y_pos = new_movey;
		if(my_item_struct->x_pos == my_pacman_char->pac_row && my_item_struct->y_pos == my_pacman_char->pac_col){
			my_item_struct->value = ' ';
			//return;
		}else{
			my_item_struct->value = characters;
		}
		mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
		wrefresh(game_window);
		sleep(HUNTER_SPEED);
		wrefresh(game_window);
	}else{
		sleep(HUNTER_SPEED);
		enum Hunter_Directions new_hunter_direction = get_hunter_direction();
		while(hunter_directions==new_hunter_direction){
			new_hunter_direction = get_hunter_direction();
		}
		hunter_directions = new_hunter_direction;
	}
}


void start_hunter(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char_2 *my_pacman_char){
	
	hunter_directions = get_hunter_direction();
	
	while(1){
		hunter_move(game_window, my_ghost_char, my_pacman_char,&my_item_struct);
	}
	
}

struct ghost_char_2 *create_ghost_char2(){
    struct ghost_char_2 *my_ghost_char= malloc(sizeof(struct ghost_char_2));
    return my_ghost_char;
}

struct pacman_char_2 *create_pacman_char2(){
    struct pacman_char_2 *my_pacman_char= malloc(sizeof(struct pacman_char_2));
    return my_pacman_char;
}


// int main(){
// 	WINDOW *my_win;
// 	int startx, starty, width, height;
// 	int ch;

// 	initscr();			/* Start curses mode 		*/
// 	cbreak();			/* Line buffering disabled, Pass on
// 					 * everty thing to me 		*/
// 	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

// 	height = 20;
// 	width = 20;
// 	starty = (LINES - height) / 2;	/* Calculating for a center placement */
// 	startx = (COLS - width) / 2;	/* of the window		*/
// 	curs_set(0);
// 	refresh();
// 	my_win = create_newwin(height, width, starty, startx);
// 	mvwaddch(my_win,1,3, ACS_CKBOARD);

// 	wrefresh(my_win);

// 	struct ghost_char_2 *my_ghost_char=create_ghost_char();
// 	my_ghost_char->ghost_row=1;
// 	my_ghost_char->ghost_col=1;

// 	struct pacman_char_2 *my_pacman_char=create_pacman_char();
// 	my_pacman_char->pac_col=5;
// 	my_pacman_char->pac_row=14;

// 	mvwaddch(my_win,my_pacman_char->pac_col,my_pacman_char->pac_row, ACS_DIAMOND);
// 	mvwaddch(my_win,my_ghost_char->ghost_col,my_ghost_char->ghost_row, ACS_CKBOARD);
// 	//add a wall
// 	mvwaddch(my_win,my_pacman_char->pac_col,my_pacman_char->pac_row+1, ACS_VLINE);
// 	mvwaddch(my_win,my_pacman_char->pac_col-1,my_pacman_char->pac_row, ACS_VLINE);
// 	mvwaddch(my_win,my_pacman_char->pac_col,my_pacman_char->pac_row-3, ACS_VLINE);
// 	wrefresh(my_win);

// 	start_hunter(my_win,my_ghost_char,my_pacman_char);

// 	endwin();
// 	return 0;
// }
