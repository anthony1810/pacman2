#ifndef COMMAND_H
#define COMMAND_H

/*! \file command.h
	\brief this is the header file of \ref command.c
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/


/*! \brief take pointer of a ncurse window to start command function
 *
 *  \details target to a ncurses window to clear, initilise command and recieving input
 *	\param command_win pointer of a ncurse window where recieving input command
 *	\param y_cor position of the cursor where input is recieved horizontally
 *
 *  \code 
 		WINDOW command_win;
 		COMMAND_STARTY = 15;
 		start_command_window(&command_window, COMMAND_STARTY);
	\endcode
*/
void start_command_window(WINDOW *command_win,int y_cor);


/*! \brief take pointer of a ncurse window to stop command function and move to game map at x_cor,y_cor
 *
 *  \details target to a ncurses window to clear everything on that ncurse window and move to modify mode at specific location
 *	\param command_win pointer of a ncurse window where recieving input command
 *	\param game_window pointer of a ncurse window
 *	\param y_cor position of the cursor horizontally
 *	\param x_cor position of the cursor vertically
 *
 *  \code 
 		WINDOW command_win;
 		WINDOW game_window;
 		map_row = 15;
 		map_col = 0;
 		stop_command_window(&command_window, &game_window,map_row,map_col);	
	\endcode
*/
void stop_command_window(WINDOW *command_win,WINDOW *game_window, int y_cor, int x_cor);


/*! \brief clear the command window
 *
 *	\param command_win pointer of a ncurse window where recieving input command
 *
 *  \code 
 		WINDOW command_win;
 		refresh_command_window(&command_window);	
	\endcode
*/

void refresh_command_window(WINDOW *command_win);

#endif