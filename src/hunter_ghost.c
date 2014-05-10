#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "hunter_ghost.h"

int HUNTER_SPEED =400000;
int isFirstTime = 1;
int wall_duration_constant = 15;
int wall_duration = 0;
int recently_defend = 0;
int isCollised = 0;
int vision_length = 0; 
enum Hunter_Directions hunter_directions;

struct Item_Struct my_item_struct;

struct Item_Struct *upper_left_struct;
struct Item_Struct *upper_struct;
struct Item_Struct *upper_right_struct;
struct Item_Struct *left_struct;
struct Item_Struct *right_struct;
struct Item_Struct *lower_left_struct;
struct Item_Struct *down_struct;
struct Item_Struct *lower_right_struct;

char game_map[35][66];

int getHunterCollision(){
	return isCollised;
}

void hunter_reset_colision(){
	if(isCollised==0){
		isCollised = 1;
	}else{
		isCollised = 0;
	}
}

void hunter_setDirection(enum Hunter_Directions first_direction){
	hunter_directions = first_direction;
}

void hunter_setItemStruct(struct Item_Struct *item){
	my_item_struct = *item;
}

void hunter_setDurationBuildWalls(int new_duration){
	wall_duration_constant = new_duration;
	wall_duration = wall_duration_constant;
}

struct Item_Struct *create_item_struct(){
    struct Item_Struct *my_item_struct= malloc(sizeof(struct Item_Struct));
    return my_item_struct;
}

void set_isFirstTime(){
	isFirstTime=1;
}

void hunter_setVisionLength(int new_length){
	vision_length = new_length;
}

void hunter_setDefendOriginChar(struct Item_Struct *new_upper_right_struct,struct Item_Struct *new_upper_struct, struct Item_Struct *new_left_struct,
	struct Item_Struct *new_upper_left_struct, struct Item_Struct *new_lower_left_struct, struct Item_Struct *new_lower_right_struct, struct Item_Struct *new_down_struct, 
	struct Item_Struct *new_right_struct){

	upper_left_struct=new_upper_left_struct;
	upper_struct=new_upper_struct;
	upper_right_struct=new_upper_right_struct;
	left_struct=new_left_struct;
	right_struct=new_right_struct;
	lower_left_struct=new_lower_left_struct;
	down_struct=new_down_struct;
	lower_right_struct=new_lower_right_struct;

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
	wattron(game_window,COLOR_PAIR(5));
	mvwaddch(game_window, my_ghost_char->ghost_row, my_ghost_char->ghost_col, ACS_CKBOARD);
	wattroff(game_window,COLOR_PAIR(5));
	wrefresh(game_window);

	wrefresh(game_window);
}

void hunter_defend(WINDOW *game_window, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char){
	recently_defend = 1;
	if(isCollised != 1){
	/** Build walls surrounds ghost **/
		if(wall_duration == 8){
			//store origin char
			int upper_right = (int)mvwinch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col+1);
			if(upper_right!= 4194912 || upper_right!= 4195169){

				upper_right_struct->value = upper_right;
				//change char, keep pos
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col+1, ACS_URCORNER);
				wattroff(game_window,COLOR_PAIR(3));
				upper_right_struct->x_pos = my_ghost_char->ghost_col+1;
				upper_right_struct->y_pos = my_ghost_char->ghost_row-1;
			}

		}
		if(wall_duration==7){
			//up char
			int up_char = (int)mvwinch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col);
			if(up_char!= 4194912 || up_char!= 4195169){
				upper_struct->value = up_char;
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col, ACS_HLINE);
				wattroff(game_window,COLOR_PAIR(3));
				upper_struct->x_pos = my_ghost_char->ghost_col;
				upper_struct->y_pos = my_ghost_char->ghost_row-1;
			}
		}
		if(wall_duration==6){
			//left char
			int left_char = (int)mvwinch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col-1);
			if(left_char!=4194912 || left_char!= 4195169){
				left_struct->value = left_char;
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col-1, ACS_VLINE);
				wattroff(game_window,COLOR_PAIR(3));
				left_struct->x_pos = my_ghost_char->ghost_col-1;
				left_struct->y_pos = my_ghost_char->ghost_row;
			}
		}
		if(wall_duration==5){
			//upper left char
			int upper_left = (int)mvwinch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col-1);
			if(upper_left!=4194912|| upper_left!= 4195169){
				upper_left_struct->value = upper_left;
				
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row-1,my_ghost_char->ghost_col-1, ACS_ULCORNER);
				wattroff(game_window,COLOR_PAIR(3));
				upper_left_struct->x_pos = my_ghost_char->ghost_col-1;
				upper_left_struct->y_pos = my_ghost_char->ghost_row-1;
			}
		}
		if(wall_duration==4){
			//lower right char
			int lower_right = (int)mvwinch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col+1);
			if(lower_right!=4194912 || lower_right!= 4195169){
				lower_right_struct->value = lower_right;
				
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col+1, ACS_LRCORNER);
				wattroff(game_window,COLOR_PAIR(3));
				lower_right_struct->x_pos = my_ghost_char->ghost_col+1;
				lower_right_struct->y_pos = my_ghost_char->ghost_row+1;
			}
		}
		if(wall_duration==3){
			//down char

			int down_char = (int)mvwinch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col);
			if(down_char!=4194912 || down_char!= 4195169){
				down_struct->value = down_char;
				
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col, ACS_HLINE);
				wattroff(game_window,COLOR_PAIR(3));
				down_struct->x_pos = my_ghost_char->ghost_col;
				down_struct->y_pos = my_ghost_char->ghost_row+1;
			}
		}
		if(wall_duration==2){
			//right char
			int right_char = (int)mvwinch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col+1);
			if(right_char!=4194912 || right_char!= 4195169){
				right_struct->value = right_char;
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row,my_ghost_char->ghost_col+1, ACS_VLINE);
				wattroff(game_window,COLOR_PAIR(3));
				right_struct->x_pos = my_ghost_char->ghost_col+1;
				right_struct->y_pos = my_ghost_char->ghost_row;
			}
		}
		if(wall_duration==1){
			//lower left char
			int lower_left = (int)mvwinch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col-1);
			if(lower_left!=4194912 || lower_left!= 4195169){
				lower_left_struct->value = lower_left;
				wattron(game_window,COLOR_PAIR(3));
				mvwaddch(game_window,my_ghost_char->ghost_row+1,my_ghost_char->ghost_col-1, ACS_LLCORNER);
				wattroff(game_window,COLOR_PAIR(3));
				lower_left_struct->x_pos = my_ghost_char->ghost_col-1;
				lower_left_struct->y_pos = my_ghost_char->ghost_row+1;
			}
		}

		if(wall_duration > 0){
			wall_duration--;
		}
		wrefresh(game_window);
	}
}

void restore_after_defend(WINDOW *game_window, WINDOW *info){

	// wclear(info);
	// wprintw(info, "%ix%i: %i - ", upper_right_struct->y_pos,upper_right_struct->x_pos,upper_right_struct->value);
	// wrefresh(info);

	mvwaddch(game_window,upper_right_struct->y_pos,upper_right_struct->x_pos,upper_right_struct->value);
	mvwaddch(game_window,upper_struct->y_pos,upper_struct->x_pos, upper_struct->value);
	mvwaddch(game_window,left_struct->y_pos,left_struct->x_pos, left_struct->value);
	mvwaddch(game_window,upper_left_struct->y_pos,upper_left_struct->x_pos, upper_left_struct->value);
	mvwaddch(game_window,lower_right_struct->y_pos,lower_right_struct->x_pos, lower_right_struct->value);
	mvwaddch(game_window,down_struct->y_pos,down_struct->x_pos, down_struct->value);
	mvwaddch(game_window,right_struct->y_pos,right_struct->x_pos, right_struct->value);
	mvwaddch(game_window,lower_left_struct->y_pos,lower_left_struct->x_pos, lower_left_struct->value);

	recently_defend = 0;
	wall_duration = wall_duration_constant;
}

void chase_pacman(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct){

	int ghost_x = my_ghost_char->ghost_col;
	int pac_x = my_pacman_char->pac_col;
	int ghost_y = my_ghost_char->ghost_row;
	int pac_y = my_pacman_char->pac_row;

	// if(pac_x==ghost_x && pac_y==ghost_y){
	// 	wclear(title);
	// 		    	wprintw(title, "%i", getHunterCollision());
	// 		    	wrefresh(title);
	// }

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
	
	if(pac_x == ghost_x){
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
	
}


void hunter_move(WINDOW *game_window, WINDOW *title, struct ghost_char_2 *my_ghost_char, struct pacman_char *my_pacman_char,struct Item_Struct *my_item_struct,
				int map_col,char map[][map_col+1]){
	//my_pacman_char->pac_state = 1;
	// wclear(title);
	// wprintw(title, "Pac State: %i,Defend Index: %i",my_pacman_char->pac_state ,recently_defend);
	// wrefresh(title);
	if(is_pacman_near(my_ghost_char, my_pacman_char)  && my_pacman_char->pac_state == VULRABLE){
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
		if(recently_defend == 1)
			restore_after_defend(game_window,title);
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
			game_map[my_ghost_char->ghost_row][my_ghost_char->ghost_col] = 'G';


			my_item_struct->x_pos = new_movex;
			my_item_struct->y_pos = new_movey;

			if(my_item_struct->x_pos == my_pacman_char->pac_row && my_item_struct->y_pos == my_pacman_char->pac_col){
				my_item_struct->value = ' ';
			}else{
				my_item_struct->value = characters;
			}
			wattron(game_window,COLOR_PAIR(5));
			mvwaddch(game_window, my_ghost_char->ghost_row, my_ghost_char->ghost_col, ACS_CKBOARD);
			wattroff(game_window,COLOR_PAIR(5));
			wrefresh(game_window);
		}else{

			enum Hunter_Directions new_hunter_direction = get_hunter_direction();
			if(hunter_directions==new_hunter_direction){
				new_hunter_direction = get_hunter_direction();
			}
			hunter_setDirection(new_hunter_direction);
		}
	}
	update_map_array(map_col,map,game_window);
}
void update_map_array(int map_col,char map[][map_col+1],WINDOW *game_window){
	
	// map[34][18]='q';
	for(int i=0; i<35; i++){
		for(int k=0;k<map_col;k++){
			int characters = (int)mvwinch(game_window, i, k);
 			
			map[i][k]=getTranslatedChar(characters);
		}
	}	
}



struct ghost_char_2 *create_ghost_char2(){
    struct ghost_char_2 *my_ghost_char= malloc(sizeof(struct ghost_char_2));
    return my_ghost_char;
}


