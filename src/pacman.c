#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "write_file.h"
#include "read_file.h"
#include "command.h"
#include "constant.h"
#include "utility.h"
#include "new_game_read_file.h"
#include "pacman_character.h"
#include "ghost_character.h"
#include "struct.h"
#include "Lee.h"
#include "hunter_ghost.h"
#include "high_score.h"
/*!
 *	@mainpage COSC2451 Pacman Editor
 *	@author The Thunder Corp (Tran Nhat Quang <s3312399@rmit.edu.vn> - Huynh Phung Cao Anh <s3357672@rmit.edu.vn>)
 *  @version 1.7.5
 *
 * \section intro_sec Introduction
 * Pacman Level Editor Project was started since the very first day back when this Project had just been release by Dr. Denis Rinfret, We had Holding an ambition of creating a
 * high quality and sophisticated program to surpass any other college teams.
 *
 * \section LICENSE 
 *	This Program is freeware as long as quoting both author, you can modify it, publish it but the 
 * copyright remains to The Thunder Corp.
 *
 * \section install_sec Installation
 * \subsection step1 Step 1: Opening the project folder location with terminal
 * \subsection step2 Step 2: typing make to compile sourcode
 * \subsection step3 Step 3: type ./pacman.sh <map_name> or ./pacman.sh
 * \subsection step4 Step 4: Enter your name and email as required
 * \subsection step5 Step 5: type ':'' to enter command mode and 'esc' to quit command mode
*/

/*! \file pacman.c 
	\brief this file is used to run all the seperate modules of this program and handle vi-command style
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/

WINDOW title_window; 	/* window to display title, version and company name */
WINDOW game_window; 	/* window to display map game */
WINDOW command_window; 	/*window to display command and recieve command */
WINDOW note_window; 	/* window to explain special characters of the map */
WINDOW wall; 			/* window to seperate game map and user info window */
WINDOW user_window; 	/* window to display user info and map info */

char user[30] ="";
char user_email[50]="";
char file_path[100] = "";
int isEnter=0;
char author[50]="";
char map_name[50]="";
char author_email[50]="";
int map_row=0;
int map_col=0;    
int scr_x, scr_y;
FILE *f ;

int option = 1;

int ghost_path [35*65]={0};
int ghost_path_2 [35*65]={0};

int move_count=0;
char phase[]="random";
int MAX_MOVE_RANDOM=30;
int MAX_MOVE_CHASE=50;


char s[100];
int n; /* The number of nodes in the graph */
long dist[(GAME_HEIGHT+1)*(GAME_WIDTH+1)][(GAME_HEIGHT+1)*(GAME_WIDTH+1)]; /* dist[i][j] is the distance between node i and j; or 0 if there is no direct connection */
void avoid_path(int avoid_row_col[],struct pacman_char *my_pacman_char);
void ghost_path_2_reset();
void ghost_path_1_reset();
void random_path(int map_col,char map[][map_col+1],struct pacman_char *my_pacman_char,int random[]);
int random_row_col[2]={0,0};

int current_map=0;
 void printPath2(WINDOW *game_window){
 	wclear(game_window);
        for (int i = 0; i < 35*65; i++){
            if (path[i] != 0)
        	wprintw(game_window,"%d",path[i]);
        }
        printf("\n");
    }


int main(int argc, char * argv[]){
	
	//ask for username and email
	init_screen(user, user_email);

	//init menu
	init_menu(&title_window);

	int row,col;
	getmaxyx(stdscr,row,col);

	int y_cur = 10;
	int x_cur = (col-strlen(MENU_EDITOR))/2 - 4;
	
	attron(COLOR_PAIR(6));
	mvprintw(y_cur,x_cur,"%s", "->");
	attroff(COLOR_PAIR(6));

	char ch;
	while((ch = getch()) != 'q'){

		if(ch == 'w' && y_cur > 10){
			mvprintw(y_cur,x_cur,"%s", "  ");
			y_cur -= 2;
			attron(COLOR_PAIR(6));
			mvprintw(y_cur,x_cur,"%s", "->");
			attroff(COLOR_PAIR(6));
			refresh();
		}
		if(ch == 's' && y_cur < 20 ){
			mvprintw(y_cur,x_cur,"%s", "  ");
			y_cur += 2;
			attron(COLOR_PAIR(6));
			mvprintw(y_cur,x_cur,"%s", "->");
			attroff(COLOR_PAIR(6));
			refresh();
		}
		if(ch == 10){
			// new game
			if(y_cur == 10){
				
				//init title, version, game info window
				init_game(&title_window, &game_window, &command_window, &note_window, &wall, &user_window, user, user_email,1);
				timeout(DELAY);
				nodelay(stdscr,TRUE);
				move_count=0;
				//create a struct of pacman and reset the score/current direction
				struct pacman_char *my_pacman_char=create_pacman_char();
				
			    my_pacman_char->score=0;
			    my_pacman_char->live=3;
			    my_pacman_char->current_direction=0;
			    my_pacman_char->pac_state=VULRABLE;
			    my_pacman_char->invulrable_duration=10000;
				
			    // my_pacman_char->invulrable_duration=5000;
			    struct ghost_char *my_ghost_char=create_ghost_char();
			    //reset
			    my_ghost_char[1].speed_multiplier=1;
			    my_ghost_char[3].speed_multiplier=0.8;
			    my_ghost_char[3].current_path=0;
			    my_ghost_char[1].current_path=0;
			    my_ghost_char[3].item_overlap=' ';
			    my_ghost_char[1].item_overlap=' ';
			    ghost_path_2_reset();
			    ghost_path_1_reset();

			    struct map *my_map=create_map();

			    // read the file and get the row&col
			    char full_path[100]="" ;
			    strcat(full_path,PATH);
			    strcat(full_path,"caoanh2");
			    strcat(full_path,".pac");
			    f = fopen(full_path, "r");
			    
	            fgets(s, 100, f);
	            fgets(s, 100, f);
	            fgets(s, 100, f);
	            map_row=atoi(s);
	            fgets(s, 100, f);
	            map_col=atoi(s);
			    char map[map_row][map_col+1];

			    // new_game_read_file(&game_window,map_row,map_col+1,map,s,"caoanh2",my_pacman_char,my_ghost_char,my_map);
			    if(argc==2){
			    	strcpy(map_name,argv[1]);   
			    }else{
			    	strcpy(map_name,"hunter_map");   
			    }


			    new_game_read_file(&game_window, map_row,map_col+1, map, s, map_name, my_pacman_char, my_ghost_char, my_map);

				struct ghost_char_2 *my_ghost_char_2=create_ghost_char2();
				my_ghost_char_2->ghost_row=my_ghost_char[0].ghost_row;
				my_ghost_char_2->ghost_col=my_ghost_char[0].ghost_col;

				// struct pacman_char_2 *my_pacman_char_2=create_pacman_char2();
				// my_pacman_char_2->pac_col=16;
				// my_pacman_char_2->pac_row=32;

				enum Hunter_Directions first_direction;
				first_direction = get_hunter_direction();
				hunter_setDirection(first_direction);

				struct Item_Struct my_item_struct;
				hunter_setItemStruct(&my_item_struct);
				
				struct Item_Struct *new_upper_left_struct = create_item_struct();
				struct Item_Struct *new_upper_struct = create_item_struct();
				struct Item_Struct *new_upper_right_struct = create_item_struct();
				struct Item_Struct *new_left_struct = create_item_struct();
				struct Item_Struct *new_right_struct = create_item_struct();
				struct Item_Struct *new_lower_left_struct = create_item_struct();
				struct Item_Struct *new_down_struct = create_item_struct();
				struct Item_Struct *new_lower_right_struct = create_item_struct();

				hunter_setDefendOriginChar(new_upper_right_struct,new_upper_struct, new_left_struct,
				new_upper_left_struct, new_lower_left_struct, new_lower_right_struct, new_down_struct, 
				new_right_struct);

				
				
				if(option == 1){
					//hunter buils wall 
						hunter_setDurationBuildWalls(11);
						//hunter only see pacman 
						hunter_setVisionLength(5);
						//ghost speed
						my_ghost_char[3].speed_multiplier = 1.5;
						my_pacman_char->invulrable_duration=15000;
						MAX_MOVE_RANDOM=50;
						MAX_MOVE_CHASE=30;

				}else if(option == 2){
						//hunter buils wall 
						hunter_setDurationBuildWalls(9);
						// //hunter only see pacman 
						hunter_setVisionLength(10);
						//ghost speed
						my_ghost_char[3].speed_multiplier = 1.3;
						my_pacman_char->invulrable_duration=10000;	
						
						//
						MAX_MOVE_RANDOM=30;
						MAX_MOVE_CHASE=30;
					
				}else if(option == 3){
					
						//hunter buils wall 
						hunter_setDurationBuildWalls(8);
						//hunter only see pacman 
						hunter_setVisionLength(15);
						//ghost speed
						my_ghost_char[3].speed_multiplier = 0.5;
						my_pacman_char->invulrable_duration=6000;

						MAX_MOVE_RANDOM=10;
						MAX_MOVE_CHASE=50;
					
				}
				//reset no complete map
				wrefresh(&game_window);
			    int prev [(map_row)*(map_col)];
				
				long d[(map_row)*(map_col)];  //d[i] is the length of the shortest file_path between the source (s) and node i 
				int translate_row_col[2];
			    n=map_row*map_col;
			    initialize_dist_array(map_row-2,map_col-2,map_row,map_col,dist,map);


			    // caculate the initial file_path
			    random_path(map_col,map,my_pacman_char,random_row_col);
			    dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
			    printPath(1,transte_from_row_col(random_row_col[0],random_row_col[1],map_col),prev,map_row*map_col,ghost_path_2,my_ghost_char);
			    //after calculate, reset the number of current file_path to 0
			    my_ghost_char[1].current_path=0;

			      //caculate the initial file_path
			    dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
			    printPath(3,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,map_row*map_col,ghost_path,my_ghost_char);
			    //after calculate, reset the number of current file_path to 0
			    my_ghost_char[3].current_path=0;

			    struct timeval pacman_delay_start,pacman_delay_end,ghost_1_delay_start,ghost_1_delay_end;
			    struct timeval ghost_3_delay_start,ghost_3_delay_end;
			    gettimeofday(&pacman_delay_start,NULL);
			    gettimeofday(&ghost_1_delay_start,NULL);
			    gettimeofday(&ghost_3_delay_start,NULL);
			    char ch2=0;
			    while((ch2 = getch()) != 'q' && my_pacman_char->live!=0){ 	
			    	
			    	int field_status_code=pacman_dead(my_pacman_char,my_ghost_char);
			    	
			    	//reset after dead 
			    	if(field_status_code==4){
						my_pacman_char->live--;
						map[my_pacman_char->pac_row][my_pacman_char->pac_col]=' ';
						map[my_pacman_char->initial_pac_row][my_pacman_char->initial_pac_col]='P';
						my_pacman_char->pac_row=my_pacman_char->initial_pac_row;
						my_pacman_char->pac_col=my_pacman_char->initial_pac_col;
						for(int i=1;i<4;i++){
							map[my_ghost_char[i].ghost_row][my_ghost_char[i].ghost_col]=' ';
							map[my_ghost_char[i].initial_ghost_row][my_ghost_char[i].initial_ghost_col]='G';
							my_ghost_char[i].ghost_row=my_ghost_char[i].initial_ghost_row;
							my_ghost_char[i].ghost_col=my_ghost_char[i].initial_ghost_col;
						}
						my_ghost_char_2->ghost_row=my_ghost_char[0].ghost_row;
						my_ghost_char_2->ghost_col=my_ghost_char[0].ghost_col;
						my_pacman_char->current_direction=0;

						my_ghost_char[1].speed_multiplier=1;
						my_ghost_char[3].speed_multiplier=0.8;
					    my_ghost_char[3].current_path=0;
					    my_ghost_char[1].current_path=0;
					    my_ghost_char[3].item_overlap=' ';
					    my_ghost_char[1].item_overlap=' ';
					    ghost_path_2_reset();
					    ghost_path_1_reset();
					    random_path(map_col,map,my_pacman_char,random_row_col);
					    dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
					    printPath(1,transte_from_row_col(random_row_col[0],random_row_col[1],map_col),prev,map_row*map_col,ghost_path_2,my_ghost_char);
					    //after calculate, reset the number of current file_path to 0
					    my_ghost_char[1].current_path=0;

					      //caculate the initial file_path
					    dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
					    printPath(3,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,map_row*map_col,ghost_path,my_ghost_char);
					    //after calculate, reset the number of current file_path to 0
					    my_ghost_char[3].current_path=0;

						// // dead_reset(my_pacman_char,my_ghost_char,map_col,map_row,map);
						new_game_update_map(&game_window,map_row,map_col+1,map);
			    	}else if(field_status_code!=5){
			    		wrefresh(&game_window);
			    		map[my_ghost_char[field_status_code].ghost_row][my_ghost_char[field_status_code].ghost_col]=' ';
						map[my_ghost_char[field_status_code].initial_ghost_row][my_ghost_char[field_status_code].initial_ghost_col]='G';
						my_ghost_char[field_status_code].ghost_row=my_ghost_char[field_status_code].initial_ghost_row;
						my_ghost_char[field_status_code].ghost_col=my_ghost_char[field_status_code].initial_ghost_col;
						
			    	}
			    	// wclear(&title_window);
			    	// wprintw(&title_window, "%i", getHunterCollision());
			    	// wrefresh(&title_window);
			    	if(my_ghost_char_2->ghost_col == my_pacman_char->pac_col && my_ghost_char_2->ghost_row==my_pacman_char->pac_row && my_pacman_char->pac_state == INVULRABLE){
			    		hunter_reset_colision();
			    		set_isFirstTime();
			    		my_ghost_char_2->ghost_col = my_ghost_char[0].initial_ghost_col;
			    		my_ghost_char_2->ghost_row = my_ghost_char[0].initial_ghost_row;
			    		mvwaddch(&game_window,my_ghost_char_2->ghost_row, my_ghost_char_2->ghost_col, ACS_CKBOARD );
			    		
			    		wrefresh(&game_window);
			    	}
					// w
			        if(ch2==119){
			            my_pacman_char->current_direction=UP;
			        }
			        //s
			        if(ch2==115){
			            my_pacman_char->current_direction=DOWN;
			        }
			        //d
			        if(ch2==100){
			            my_pacman_char->current_direction=RIGHT;
			        }
			        //a
			        if(ch2==97){
			            my_pacman_char->current_direction=LEFT;
			        }

			        gettimeofday(&pacman_delay_end,NULL);
			        gettimeofday(&ghost_1_delay_end,NULL);
			        gettimeofday(&ghost_3_delay_end,NULL);
			  //      	//ajust the speed of ghost based on remain pellet
					// if(my_map->remain_pellet/my_map->total_pellet>0.75){
					// 	my_ghost_char[3].speed_multiplier=1.4;
					// }else if(0.75<my_map->remain_pellet/my_map->total_pellet<0.25){
					// 	my_ghost_char[3].speed_multiplier=0.5;
					// }

					if(timeval_diff(NULL,&pacman_delay_end,&pacman_delay_start)>=DELAY){
			            gettimeofday(&pacman_delay_start,NULL);
			            pacman_char_move(my_pacman_char,map_col+1,map,my_map,&game_window);
			            if(my_pacman_char->score==2000){
			    			my_pacman_char->live++;
			    		}
			            wrefresh(&game_window);
					}
					//ghost [0][2][3]
					if(timeval_diff(NULL,&ghost_3_delay_end,&ghost_3_delay_start)>=(DELAY*my_ghost_char[3].speed_multiplier)){
						gettimeofday(&ghost_3_delay_start,NULL);
						if(my_pacman_char->pac_state==VULRABLE){
				        	//hunter ghost
				        	hunter_move(&game_window,&title_window, my_ghost_char_2, my_pacman_char,&my_item_struct,map_col,map);
				        	// other ghost
				        	//ghost_mimic_pacman(my_ghost_char,my_pacman_char,map_col,map,&game_window);
							if(my_ghost_char[3].current_path==2){
				            	//before calculate, reset the current ghost file_path to 0
				                my_ghost_char[3].current_path=0;
				                dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
				                printPath(3,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,(map_row)*(map_col),ghost_path,my_ghost_char);
				                //after calculate, reset the number of current file_path to 0
				                my_ghost_char[3].current_path=0;
				                ghost_move(3,ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				            }else{
				           	 	ghost_move(3,ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				        	}
				        	// lee(my_pacman_char->pac_col,my_pacman_char->pac_row,my_ghost_char[1].ghost_col,my_ghost_char[1].ghost_row, map,&game_window);
				        	// ghost_move(1,path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window);
				        	// printPath2(&game_window);
			        	}else if(my_pacman_char->pac_state==INVULRABLE){
			        		if(my_ghost_char[3].current_path==2){
				        		int avoid_row_col[2];
				        		avoid_path(avoid_row_col,my_pacman_char);
				        		my_ghost_char[3].current_path=0;
					            dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
					            printPath(3,transte_from_row_col(avoid_row_col[0],avoid_row_col[1],map_col),prev,(map_row)*(map_col),ghost_path,my_ghost_char);
					            my_ghost_char[3].current_path=0;
					            ghost_move(3,ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				        	}else{
				            	ghost_move(3,ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				        	}
				        	hunter_move(&game_window,&title_window, my_ghost_char_2, my_pacman_char,&my_item_struct,map_col,map);
			        	}

			        	wrefresh(&game_window);
			        	start_stats(&user_window,user, user_email, my_pacman_char->score, my_pacman_char->live, 1);
					}
					//ghost [1]
					if(timeval_diff(NULL,&ghost_1_delay_end,&ghost_1_delay_start)>=(DELAY*my_ghost_char[1].speed_multiplier)){
						gettimeofday(&ghost_1_delay_start,NULL);
				

				            if(strcmp(phase,"random")==0 && move_count<=MAX_MOVE_RANDOM){
				            	int next_step=my_ghost_char[1].current_path;
				            	if(ghost_path_2[next_step+1]!=0){
						 	   		ghost_move(1,ghost_path_2,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				            	}else{
				            		my_ghost_char[1].current_path=0;
			            		   	random_path(map_col,map,my_pacman_char,random_row_col);
								    dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
								    printPath(1,transte_from_row_col(random_row_col[0],random_row_col[1],map_col),prev,map_row*map_col,ghost_path_2,my_ghost_char);
								    my_ghost_char[1].current_path=0;
								    ghost_move(1,ghost_path_2,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				            	}
				            	move_count++;
				            	if(move_count==MAX_MOVE_RANDOM){
				            		move_count=0;
				            		strcpy(phase,"chase");
				            		ghost_path_2_reset();
				            		my_ghost_char[1].current_path=0;
				              		dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
				             		printPath(1,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,(map_row)*(map_col),ghost_path_2,my_ghost_char);
				               		//after calculate, reset the number of current file_path to 0
				               		my_ghost_char[1].current_path=0;
				            	}
				            }else if(strcmp(phase,"chase")==0 && move_count<=MAX_MOVE_CHASE){
				            	if(my_ghost_char[1].current_path==2){
				            		//before calculate, reset the current ghost file_path to 0
				               		my_ghost_char[1].current_path=0;
				              		dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
				             		printPath(1,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,(map_row)*(map_col),ghost_path_2,my_ghost_char);
				               		//after calculate, reset the number of current file_path to 0
				               		my_ghost_char[1].current_path=0;
							   		ghost_move(1,ghost_path_2,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
						 		}else{
				            		ghost_move(1,ghost_path_2,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);

				            	}
				            	move_count++;
				            
				            	if(move_count==MAX_MOVE_CHASE){
				            		move_count=0;
				            		strcpy(phase,"random");
				            		ghost_path_2_reset();
				            		my_ghost_char[1].current_path=0;
			            		   	random_path(map_col,map,my_pacman_char,random_row_col);
								    dijkstra(transte_from_row_col(my_ghost_char[1].ghost_row,my_ghost_char[1].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
								    printPath(1,transte_from_row_col(random_row_col[0],random_row_col[1],map_col),prev,map_row*map_col,ghost_path_2,my_ghost_char);
								    my_ghost_char[1].current_path=0;
								    ghost_move(1,ghost_path_2,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window,&user_window);
				            	}

				            }
				            wrefresh(&game_window);
					}
    			}
    			int row2,col2;
				getmaxyx(&game_window,row2,col2);
    			wclear(&game_window);
    			mvwprintw(&game_window,10, (col2-strlen("GAME OVER"))/2, "GAME OVER");
    			mvwprintw(&game_window,12, (col2-strlen("This is your score: "))/2, "This is your score: %i ",my_pacman_char->score);
    			mvwprintw(&game_window,13, (col2-strlen("Your score has been saved high_score.txt"))/2, "Your score has been saved in high_score.txt");
    			wrefresh(&game_window);
    			timeout(-1);
    			getch();
    			if(my_pacman_char->score > 0)
    			save_high_score(user, my_pacman_char->score);
    			
    			set_isFirstTime();
    			//reset the ghost file_path for "2nd" new game
    			for (int i = 0; i < map_row*map_col; ++i)
    			{
    					ghost_path[i]=0;
    					ghost_path_2[i]=0;
    				
    			}
				free(my_pacman_char);
				free(my_ghost_char);
				free(my_ghost_char_2);
				//wait for user to enter st
				clear();
				//return to menu
				init_menu(&title_window);
				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));

			// Option
			}else if(y_cur == 12){
				timeout(-1);
				show_option(&title_window,&option);
				clear();
				init_menu(&title_window);
				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));
			// high score
			}else if(y_cur == 14){
				timeout(-1);
				show_high_score(&title_window,&command_window);
				clear();
				init_menu(&title_window);
				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));
			// level editor
			}else if(y_cur == 16){
				timeout(-1);
				clear();
				refresh();
				//init level editor
				init_editor(&title_window, &game_window, &command_window, &note_window, &wall, &user_window, user, user_email, file_path,
				isEnter, author, map_name, author_email,map_row, map_col,scr_x,scr_y, f, argc, argv);

				wrefresh(&title_window);
				wrefresh(&game_window);
				clear();

				init_menu(&title_window);

				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));

				continue;
			// credits
			}else if(y_cur == 18){
				timeout(-1);
				show_credit(&title_window);
				clear();
				init_menu(&title_window);
				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));
			// end game
			}else if(y_cur == 20){
				break;
			}
		}
	}

	refresh();
	endwin();
	return 0;
}
void avoid_path(int avoid_row_col[],struct pacman_char *my_pacman_char){
	if(my_pacman_char->pac_row<17 && my_pacman_char->pac_col<32){
		avoid_row_col[0]=1;
		avoid_row_col[1]=63;
	}else if(my_pacman_char->pac_row<17 && my_pacman_char->pac_col>32){
		avoid_row_col[0]=1;
		avoid_row_col[1]=1;
	}else if(my_pacman_char->pac_row>17 && my_pacman_char->pac_col<32){
		avoid_row_col[0]=33;
		avoid_row_col[1]=63;
	}else if(my_pacman_char->pac_row>17 && my_pacman_char->pac_col>32){
		avoid_row_col[0]=33;
		avoid_row_col[1]=1;
	}	
}

void ghost_path_2_reset(){
	for(int i=0;ghost_path_2[i]!=0;i++){
		ghost_path_2[i]=0;
	}
}
void ghost_path_1_reset(){
	for(int i=0;ghost_path[i]!=0;i++){
		ghost_path[i]=0;
	}
}
void random_path(int map_col,char map[][map_col+1],struct pacman_char *my_pacman_char,int random[]){
	random[0]=rand()% 33;
	random[1]=rand()%63;
	while(checkWall(random[0],random[1],map_col,map)==0){
		random[0]=rand() %33;
		random[1]=rand() %63;
	}

}





