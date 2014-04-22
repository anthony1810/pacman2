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
char path[100] = "";
int isEnter=0;
char author[50]="";
char map_name[50]="";
char author_email[50]="";
int map_row=0;
int map_col=0;    
int scr_x, scr_y;
FILE *f ;


char s[100];
int e; /* The number of nonzero edges in the graph */
int n; /* The number of nodes in the graph */
long dist[(GAME_HEIGHT+2)*(GAME_WIDTH+1)][(GAME_HEIGHT+2)*(GAME_WIDTH+1)]; /* dist[i][j] is the distance between node i and j; or 0 if there is no direct connection */



int current_num=0;


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
				//create a struct of pacman and reset the score/current direction
				struct pacman_char *my_pacman_char=create_pacman_char();
			    my_pacman_char->score=0;
			    my_pacman_char->current_direction=0;

			    struct ghost_char *my_ghost_char=create_ghost_char();
			    //read the file and get the row&col
			    char full_path[100]="" ;
			    strcat(full_path,PATH);
			    strcat(full_path,"caoanh");
			    strcat(full_path,".pac");
			    f = fopen(full_path, "r");
			    char s[100];
	            fgets(s, 100, f);
	            fgets(s, 100, f);
	            fgets(s, 100, f);
	            map_row=atoi(s);
	            fgets(s, 100, f);
	            map_col=atoi(s);
			    char map[map_row][map_col+1];

			    new_game_read_file(&game_window,map_row,map_col+1,map,s,"caoanh",my_pacman_char,my_ghost_char);
			    wclear(&game_window);
			    wrefresh(&game_window);

			    int prev [(map_row+2)*(map_col+2)];
				int ghost_path [(map_row+2)*(map_col+2)];

				long d[(map_row+2)*(map_col+2)]; /* d[i] is the length of the shortest path between the source (s) and node i */
				int translate_row_col[2];

			    n=map_row*map_col;
			    initialize_dist_array(map_row-2,map_col-2,map_row,map_col,dist,map);

			    //caculate the initial path
			    dijkstra(transte_from_row_col(my_ghost_char->ghost_row,my_ghost_char->ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
			    printPath(transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,ghost_path,&current_num);
			 
			    current_num=0;
			    struct timeval earlier;
			    struct timeval later;
			    gettimeofday(&earlier,NULL);
			    char ch2=0;
			    while((ch2 = getch()) != 'q' && pacman_dead(my_pacman_char,my_ghost_char)==0){ 	 
					//w
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
			        gettimeofday(&later,NULL);
			        //timeval_diff
			        if(timeval_diff(NULL,&later,&earlier)>=DELAY){
			            gettimeofday(&earlier,NULL);
			            
			            pacman_char_move(my_pacman_char,map_col+1,map);
			            
			            // //ghost move 4 and calculate new path
			            // if(current_num==4){
			            //     current_num=0;
			            //     dijkstra(transte_from_row_col(my_ghost_char->ghost_row,my_ghost_char->ghost_col,map_col),map_row,map_col,d,dist,n,prev); 
			            //     printPath(transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col,map_col),prev,ghost_path,&current_num);
			            //     current_num=0;
			            //     //to prevent the ghost flashing when calculating new path
			            //     ghost_move(ghost_path,&current_num,translate_row_col,map_row,map_col,map,my_ghost_char);
			            // }
			            // ghost_move(ghost_path,&current_num,translate_row_col,map_row,map_col,map,my_ghost_char);
			            new_game_update_map(&game_window,map_row,map_col+1,map);
					}
    			}
    			//reset the ghost path for "2nd" new game
    			for (int i = 0; i < (map_row+2)*(map_col+2); ++i)
    			{
    				ghost_path[i]=0;

    			}
    			current_num=0;
				//call your ghost movement fuction here

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

			// level editor
			}else if(y_cur == 14){
				timeout(-1);
				clear();
				refresh();
				//init level editor
				init_editor(&title_window, &game_window, &command_window, &note_window, &wall, &user_window, user, user_email, path,
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


