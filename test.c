#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>

int HUNTER_SPEED =400000;
int isFirstTime = 1;

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

struct ghost_char
{
	int ghost_row;
	int ghost_col;
	int speed_multiplier;
};

struct pacman_char
{
	int pac_row;
	int pac_col;
	int current_direction;
	int score;
};

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

enum Hunter_Directions hunter_directions;

int is_wall_ahead(WINDOW *game_window, int x_pos, int y_pos){
	//wrefresh(game_window);
	int result = 1;
	int characters = (int)mvwinch(game_window, y_pos, x_pos);
	// printw("%ix%i: %i - ",y_pos,x_pos,characters);
	// refresh();
	// getch();
	// printw("%i",(int)mvwinch(game_window, 1, 9));
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

bool is_pacman_near(struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char){

	int row = my_ghost_char->ghost_row - my_pacman_char->pac_row;
	int col = my_ghost_char->ghost_col - my_pacman_char->pac_col;
	if(abs(row)<=4 && abs(col)<=4){
		return true;
	}else{
		return false;
	}

}

void chase_pacman(WINDOW *game_window,WINDOW *info, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct){
	int row = my_ghost_char->ghost_row - my_pacman_char->pac_row;
	int col = my_ghost_char->ghost_col - my_pacman_char->pac_col;

	if(my_pacman_char->pac_col > my_ghost_char->ghost_col){
		while(my_pacman_char->pac_col != my_ghost_char->ghost_col){
			my_ghost_char->ghost_col++;

			mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
		

			my_item_struct->x_pos = my_ghost_char->ghost_row;
			my_item_struct->y_pos = my_ghost_char->ghost_col;

			int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

			my_item_struct->value = characters;
			mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
			sleep(HUNTER_SPEED);
			wrefresh(game_window);
		}

	}

	if(my_pacman_char->pac_col < my_ghost_char->ghost_col){
		while(my_pacman_char->pac_col != my_ghost_char->ghost_col){
			my_ghost_char->ghost_col--;

			mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
		

			my_item_struct->x_pos = my_ghost_char->ghost_row;
			my_item_struct->y_pos = my_ghost_char->ghost_col;

			int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

			my_item_struct->value = characters;
			mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
			sleep(HUNTER_SPEED);
			wrefresh(game_window);
		}

	}


	if(my_pacman_char->pac_row > my_ghost_char->ghost_row){
		while(my_pacman_char->pac_row != my_ghost_char->ghost_row){
			if(abs(my_pacman_char->pac_row - my_ghost_char->ghost_row)==1){
				sleep(5000000);
			}
			my_ghost_char->ghost_row++;

			mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
		

			my_item_struct->x_pos = my_ghost_char->ghost_row;
			my_item_struct->y_pos = my_ghost_char->ghost_col;

			int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

			my_item_struct->value = characters;
			mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
			sleep(HUNTER_SPEED);
			wrefresh(game_window);
		}

	}

	if(my_pacman_char->pac_row < my_ghost_char->ghost_row){
		while(my_pacman_char->pac_row != my_ghost_char->ghost_row){
			if(abs(my_pacman_char->pac_row - my_ghost_char->ghost_row)==1){
				wclear(info);
				wprintw(info, "Hunter's waiting to strike pacman.");
				wrefresh(info);
				sleep(5000000);
			}
			my_ghost_char->ghost_row--;

			mvwaddch(game_window, my_item_struct->y_pos, my_item_struct->x_pos, my_item_struct->value);
		

			my_item_struct->x_pos = my_ghost_char->ghost_row;
			my_item_struct->y_pos = my_ghost_char->ghost_col;

			int characters = (int)mvwinch(game_window, my_item_struct->y_pos, my_item_struct->x_pos);

			my_item_struct->value = characters;
			mvwaddch(game_window, my_ghost_char->ghost_col, my_ghost_char->ghost_row, ACS_CKBOARD);
			sleep(HUNTER_SPEED);
			wrefresh(game_window);
		}

	}
	wclear(info);
	wprintw(info, "Hunter just kill pacman!");
	wrefresh(info);
	getch();
}

int is_wall_between(WINDOW *game_window, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char){
	int ghost_x = my_ghost_char->ghost_row;
	int pac_x = my_pacman_char->pac_row;
	int ghost_y = my_ghost_char->ghost_col;
	int pac_y = my_pacman_char->pac_col;

	//printw("%ix%i vs %ix%i",ghost_y,ghost_x,pac_y,pac_x);

	int result = 0;
	if(my_pacman_char->pac_col > my_ghost_char->ghost_col){
		while(pac_y != ghost_y){
			ghost_y++;
			if(is_wall_ahead(game_window, ghost_y, ghost_x)){
				result = 1;
			}
		}

	}

	if(my_pacman_char->pac_col < my_ghost_char->ghost_col){
		while(pac_y !=ghost_y){
			ghost_y--;
			if(is_wall_ahead(game_window, ghost_y, ghost_x)){
				result = 1;
			}
		}

	}

	if(my_pacman_char->pac_row > my_ghost_char->ghost_row){
		while(pac_x != ghost_x){
			ghost_x++;
			if(is_wall_ahead(game_window, ghost_y, ghost_x)){
				result = 1;
			}
		}

	}

	if(my_pacman_char->pac_row < my_ghost_char->ghost_row){
		while(pac_x != ghost_x){
			ghost_x--;
			if(is_wall_ahead(game_window, ghost_y, ghost_x)){
				result = 1;
			}
		}

	}

	return result;
}


void hunter_move(WINDOW *game_window, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct){
	WINDOW *info;
	info = create_newwin(1,100,23,0);
	refresh(); 

	if(is_pacman_near(my_ghost_char, my_pacman_char)){
		wclear(info);
		wprintw(info, "Hunter saw pacman...Hunter start chasing.");
		wrefresh(info);
		if(!is_wall_between(game_window, my_ghost_char,my_pacman_char)){
			HUNTER_SPEED = 200000;
			chase_pacman(game_window,info, my_ghost_char, my_pacman_char,my_item_struct);
		}else{
			HUNTER_SPEED = 400000;
			wclear(info);
			wprintw(info, "a wall has covered pacman.");
			wrefresh(info);
		}
	}else{
		wclear(info);
		wprintw(info, "Hunter's doing a Patrol.");
		wrefresh(info);
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

	if(is_wall_ahead(game_window,new_movey,new_movex)==0){

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


void start_hunter(WINDOW *game_window, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char){
	
	hunter_directions = get_hunter_direction();
	struct Item_Struct my_item_struct;
	
	char ch;
	while(1){
		hunter_move(game_window, my_ghost_char, my_pacman_char,&my_item_struct);
	}
	
}

struct ghost_char *create_ghost_char(){
    struct ghost_char *my_ghost_char= malloc(sizeof(struct ghost_char));
    return my_ghost_char;
}

struct pacman_char *create_pacman_char(){
    struct pacman_char *my_pacman_char= malloc(sizeof(struct pacman_char));
    return my_pacman_char;
}


int main(){
	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 20;
	width = 20;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	curs_set(0);
	refresh();
	my_win = create_newwin(height, width, starty, startx);
	mvwaddch(my_win,1,3, ACS_BULLET);
	wrefresh(my_win);

	struct ghost_char *my_ghost_char=create_ghost_char();
	my_ghost_char->ghost_row=1;
	my_ghost_char->ghost_col=1;

	struct pacman_char *my_pacman_char=create_pacman_char();
	my_pacman_char->pac_col=7;
	my_pacman_char->pac_row=14;

	mvwaddch(my_win,my_pacman_char->pac_col,my_pacman_char->pac_row, ACS_DIAMOND);
	mvwaddch(my_win,my_ghost_char->ghost_col,my_ghost_char->ghost_row, ACS_CKBOARD);
	//add a wall
	//mvwaddch(my_win,my_pacman_char->pac_col,my_pacman_char->pac_row+1, ACS_VLINE);
	wrefresh(my_win);

	start_hunter(my_win,my_ghost_char,my_pacman_char);

	endwin();
	return 0;
}
