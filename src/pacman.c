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

int ghost_path [35*65]={0};
char s[100];
int n; /* The number of nodes in the graph */
long dist[(GAME_HEIGHT+1)*(GAME_WIDTH+1)][(GAME_HEIGHT+1)*(GAME_WIDTH+1)]; /* dist[i][j] is the distance between node i and j; or 0 if there is no direct connection */
void random_path(int map_row,int map_col,struct ghost_char *my_ghost_char,char map[][map_col+1],int random_row_col[]);

 void printPath2(){
        for (int i = 0; i < 35*65; i++){
            if (path[i] != 0)
            printf("%d ", path[i]);
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
		if(ch == 's' && y_cur < 18 ){
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
				//create a struct of pacman and reset the score/current direction
				struct pacman_char *my_pacman_char=create_pacman_char();
			    my_pacman_char->score=0;
			    my_pacman_char->live=3;
			    my_pacman_char->current_direction=0;
			    my_pacman_char->pac_state=VULRABLE;
			    struct ghost_char *my_ghost_char=create_ghost_char();
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



			    new_game_read_file(&game_window, map_row,map_col+1, map, s, "hunter_map", my_pacman_char, my_ghost_char, my_map);

				struct ghost_char_2 *my_ghost_char_2=create_ghost_char2();
				my_ghost_char_2->ghost_row=3;
				my_ghost_char_2->ghost_col=13;

				// struct pacman_char_2 *my_pacman_char_2=create_pacman_char2();
				// my_pacman_char_2->pac_col=16;
				// my_pacman_char_2->pac_row=32;

				enum Hunter_Directions first_direction;
				hunter_setDirection(first_direction);

				struct Item_Struct my_item_struct;
				hunter_setItemStruct(&my_item_struct);
				
				wrefresh(&game_window);
			    int prev [(map_row)*(map_col)];
				
				long d[(map_row)*(map_col)];  //d[i] is the length of the shortest file_path between the source (s) and node i 
				int translate_row_col[2];
			    n=map_row*map_col;
			    initialize_dist_array(map_row-2,map_col-2,map_row,map_col,dist,map);

			    //caculate the initial file_path
			    dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
			    printPath(3,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,map_row*map_col,ghost_path,my_ghost_char);
			    //after calculate, reset the number of current file_path to 0
			    my_ghost_char[3].current_path=0;

			    struct timeval pacman_delay_start,pacman_delay_end,ghost_delay_start,ghost_delay_end;
			    gettimeofday(&pacman_delay_start,NULL);
			    gettimeofday(&ghost_delay_start,NULL);
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
						for(int i=0;i<4;i++){
							map[my_ghost_char[i].ghost_row][my_ghost_char[i].ghost_col]=' ';
							map[my_ghost_char[i].initial_ghost_row][my_ghost_char[i].initial_ghost_col]='G';
							my_ghost_char[i].ghost_row=my_ghost_char[i].initial_ghost_row;
							my_ghost_char[i].ghost_col=my_ghost_char[i].initial_ghost_col;
						}
						my_pacman_char->current_direction=0;
						// // dead_reset(my_pacman_char,my_ghost_char,map_col,map_row,map);
						new_game_update_map(&game_window,map_row,map_col+1,map);
			    	}else if(field_status_code!=5){
			    		wrefresh(&game_window);
			    		map[my_ghost_char[field_status_code].ghost_row][my_ghost_char[field_status_code].ghost_col]=' ';
						map[my_ghost_char[field_status_code].initial_ghost_row][my_ghost_char[field_status_code].initial_ghost_col]='G';
						my_ghost_char[field_status_code].ghost_row=my_ghost_char[field_status_code].initial_ghost_row;
						my_ghost_char[field_status_code].ghost_col=my_ghost_char[field_status_code].initial_ghost_col;
						
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
			        gettimeofday(&ghost_delay_end,NULL);
			       	//ajust the speed of ghost based on remain pellet
					if(my_map->remain_pellet/my_map->total_pellet>0.75){
						my_ghost_char[3].speed_multiplier=1.4;
					}else if(0.75<my_map->remain_pellet/my_map->total_pellet<0.25){
						my_ghost_char[3].speed_multiplier=0.5;
					}

					if(timeval_diff(NULL,&pacman_delay_end,&pacman_delay_start)>=DELAY){
			            gettimeofday(&pacman_delay_start,NULL);
			            pacman_char_move(my_pacman_char,map_col+1,map,my_map,&game_window);
			            wrefresh(&game_window);
					}
					if(timeval_diff(NULL,&ghost_delay_end,&ghost_delay_start)>=(DELAY*my_ghost_char[3].speed_multiplier)){
						gettimeofday(&ghost_delay_start,NULL);
						if(my_pacman_char->pac_state==VULRABLE){
				            if(my_ghost_char[3].current_path==2){
				            	//before calculate, reset the current ghost file_path to 0
				                my_ghost_char[3].current_path=0;
				                dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
				                printPath(3,transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,(map_row)*(map_col),ghost_path,my_ghost_char);
				                //after calculate, reset the number of current file_path to 0
				                my_ghost_char[3].current_path=0;
				                ghost_move((map_row+2)*(map_col+2),ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window);
				            }else{
				           	 	ghost_move((map_row+2)*(map_col+2),ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window);
				        	}
				        	//hunter ghost
				        	hunter_move(&game_window,&title_window, my_ghost_char_2, my_pacman_char,&my_item_struct);
			        	}else if(my_pacman_char->pac_state==INVULRABLE){
			        		if(my_ghost_char[3].current_path==2){
				        		int random_row_col[2];
				        		random_path(map_row,map_col,my_ghost_char,map,random_row_col);
				        		my_ghost_char[3].current_path=0;
					            dijkstra(transte_from_row_col(my_ghost_char[3].ghost_row,my_ghost_char[3].ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
					            printPath(3,transte_from_row_col(random_row_col[0],random_row_col[1],map_col),prev,(map_row)*(map_col),ghost_path,my_ghost_char);
					            my_ghost_char[3].current_path=0;
					            ghost_move((map_row+2)*(map_col+2),ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window);
				        	}else{
				            	ghost_move((map_row+2)*(map_col+2),ghost_path,translate_row_col,map_row,map_col,map,my_ghost_char,&game_window);
				        	}
				        	
				        	// lee(30,4,11,7, map,&game_window);
				        	// printPath2();
			        	}

			        	wrefresh(&game_window);
			        	start_stats(&user_window,user, user_email, my_pacman_char->score, my_pacman_char->live, 1);

					}
    			}
    			//reset the ghost file_path for "2nd" new game
    			// for (int i = 0; i < (map_row+2)*(map_col+2); ++i)
    			// {
    			// 	for (int j = 0; j < 4; ++j)
    			// 	{
    			// 		ghost_path[j][i]=0;
    			// 	}
    			// }
				free(my_pacman_char);
				free(my_ghost_char);
				//wait for user to enter st
				getch();
				clear();
				//return to menu
				init_menu(&title_window);
				attron(COLOR_PAIR(6));
				mvprintw(y_cur,x_cur,"%s", "->");
				attroff(COLOR_PAIR(6));

			// highscore
			}else if(y_cur == 12){
				show_high_score(&title_window,&command_window);
			// level editor
			}else if(y_cur == 14){
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
			}else if(y_cur == 16){

			// end game
			}else if(y_cur == 18){
				break;
			}
		}
	}

	refresh();
	endwin();
	return 0;
}
void random_path(int map_row,int map_col,struct ghost_char *my_ghost_char,char map[][map_col+1],int random_row_col[]){
	random_row_col[0]=rand()%map_row;
	random_row_col[1]=rand()%map_col;
	while(checkWall(random_row_col[0],random_row_col[1],map_col,map)==0){
		random_row_col[0]=rand()%map_row;
		random_row_col[1]=rand()%map_col;
	}
	
}


