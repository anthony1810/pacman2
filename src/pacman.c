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

int main(int argc, char * argv[]){
	
	//ask for username and email
	init_screen(user, user_email);

	int row,col;
	getmaxyx(stdscr,row,col);

	//init menu for selection
	init_menu(&title_window, &game_window, &command_window, &note_window, &wall, &user_window,user, user_email, path,
				isEnter, author, map_name, author_email,map_row, map_col,scr_x,scr_y, f, argc, argv);

	refresh();
	endwin();
	return 0;
}


