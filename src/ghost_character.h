#ifndef GHOST_CHARACTER_H
#define GHOST_CHARACTER_H
#include "struct.h"
/*! @file ghost_character.h 
    @brief this is the header file of \ref ghost_character.c.
*/

/*!
@brief convert position from 1D to 2D array (row and col)
@code
        wmove(map_win,row-1,col-2);  
        refresh();
        wrefresh(map_win);      
 @endcode
 move the cursor to the lower-right coner of the map
 @param input the index of position in 1D array
 @param translate_row_col[] the array for storing converted row and caol
*/
void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col);
/*!
@brief convert the position of 2D array to 1D array
 move the cursor to the lower-right coner of the map
 @param col,row contain the row and col need to convert
 @param map_col the map_col used for converting
*/
int transte_from_row_col(int row,int col,int map_col);

/*!
@brief read the file and print it on screen
@code
       for (i = 1; i <= n; ++i) {
        d[i] = INFINITY;
        prev[i] = -1; 
        visited[i] = 0; 
    }      
 @endcode
 prepare all the array
 @param *map_win the window that contain the map to write the character on it
 @param col,row contain the row and col for the array
 @param map[row][col] 2d array store the map for later use,NOTE:  the col is read from the file PLUS 1(the '\n' character )
*/
void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row)*(map_col)][(map_row)*(map_col)],int n,int prev[]);

/*!
@brief check if the position is wall
 @param col,row position to check
 @param map[row][col] 2d array store the map for checking
 \return 0 if wall, 1 otherwise
*/
int checkWall(int row,int col,int map_col,char map[][map_col+1]);

/*!
@brief initilaize the dist 2D array storing the distance to neighbor for all node

*/
void initialize_dist_array(int row,int col,int map_row,int map_col,long dist [][(map_row)*(map_col)],char map[][map_col+1]);

/*!
@brief store all the caculated path to for later usage using recursion
*/
void printPath(int ghost_num,int dest,int prev[],int ghost_path_size,int ghost_path[ghost_path_size],struct ghost_char *my_ghost_char);
/*!
@brief reposible for moving the ghost around, update map 2d array
 @param ghost_num the ghost number need to move
 @param ghost_path the ghost corresponding calculated path
 @param map[row][col] 2d array use for updating 
*/
void ghost_move(int ghost_num,int ghost_path[],int translate_row_col[],
				int map_row,int map_col,char map[][map_col+1],struct ghost_char *my_ghost_char,WINDOW *game_window,WINDOW *user_window);

/*!
@brief use for converting map text character(g,p,f) to on-screen character(ACS_CKBOARD,ACS_DIAMOND,ACS_STERLING)
*/
chtype convert_to_map_character(char text_character);

void ghost_mimic_pacman(struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char,int map_col,char map[][map_col+1],WINDOW *game_window);


#endif
