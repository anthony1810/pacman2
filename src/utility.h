#ifndef UTILITY_H
#define UTILITY_H

/*! \file utility.h
	\brief this is the header file of \ref utility.c
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/

#include "curses.h"

/*! \brief create a ncurse window 
 *
 *	\param height the height of the desire window
 *	\param width the width of the desire window
 *	\param starty the start location horizontally
 *	\param startx the start location horizontally
 *	\return ncurse window
 *
 *  \code 
 		WINDOW title_window;
		*title_window = create_new_win(TITLE_HEIGHT, col, 0, 0);
	\endcode
*/
WINDOW create_new_win(int height, int width, int starty, int startx);


/*! \brief init gui for user to interact with the program
 *
 *  \details initilise title_window, game_window, command_window, note_window, user_window to init GUI appearance 
 *
 *	\param title_window the ncurse window responsible for show up title, version and creator
 *	\param game_window the ncurse window responsible for show up game map
 *	\param command_window the ncurse window responsible for recieving commands, driving program
 *	\param note_window the ncurse window responsible for explain some special character of the game
 *	\param wall the ncurse window responsible for showing a wall sepeate between game and user_window
 *  \param user_window the ncuses window responsible for display user info and map info 
 *
 *  \code 
 		WINDOW title_window;
		WINDOW game_window;
		WINDOW command_window;
		WINDOW note_window;
		WINDOW wall;
		WINDOW user_window;

 		init_screen(&title_window, &game_window, &command_window, &note_window, &wall, &user_window, user, user_email);
	\endcode
*/

void init_screen(char user[], char user_email[]);


/*! \brief display and update information of user and map
 *
 *  \details clear content of a ncurses window, print formatted information to it 
 *
 *	\param user_window the ncurse window targeted for printing
 *	\param user the name of the user who use the program
 *	\param user_email the email of the user who use the prgram
 *	\param map_name the title of the map
 *	\param map_row the row length of the ncurse window
 *  \param map_col the col lenth of the ncurse window
 *  \param author the author of the map
 *  \param author_email the email of the one who create the map
 *
 *  \code 
 		WINDOW user_window;
 		init_user_info(&user_window,user, user_email, map_name,map_row,map_col-1,author,author_email);
	\endcode
*/
	

void init_user_info(WINDOW* user_window,char user[], char user_email[], char map_name[],
	 int map_row, int map_col, char author[], char author_email[]);

void init_editor(WINDOW *title_window, WINDOW *game_window, WINDOW *command_window, 
	WINDOW *note_window, WINDOW *wall, WINDOW *user_window, char user[], char user_email[], char path[100],
	int isEnter, char author[50], char map_name[50], char author_email[50], int map_row, int map_col, int scr_x, int scr_y, FILE *f,int argc, char *argv[]);

void init_menu(WINDOW *title_window);

void start_stats(WINDOW *user_window,char user[], char user_email[], int scores, int life, int level);

void init_game(WINDOW *title_window, WINDOW *game_window, WINDOW *command_window, WINDOW *note_window, WINDOW *wall, WINDOW *user_window, char user[], char user_email[], int level);

#endif