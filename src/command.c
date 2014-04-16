#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "write_file.h"
#include "read_file.h"

/*! \file command.c 
	\brief this file provide functions to handle vi-commands style.
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/

void start_command_window(WINDOW *command_win,int y_cor){
	wclear(command_win);
	waddch(command_win, ':');
	move(y_cor,1);
	refresh();
	wrefresh(command_win);
}

void stop_command_window(WINDOW *command_win,WINDOW *game_window, int y_cor, int x_cor){
	wclear(command_win);
	wrefresh(command_win);
	wmove(game_window,y_cor-1,x_cor-2);
	refresh();
	wrefresh(game_window);
}

void refresh_command_window(WINDOW *command_win){
	wclear(command_win);
	waddch(command_win, ':');
	wrefresh(command_win);
}




