#ifndef READ_FILE_H
#define READ_FILE_H
/*! @file read_file.h 
    @brief this is the header file of \ref read_file.c.
*/
/*!
@brief readFile,get_author_name_and_email function will be called within.
*/
void read_command(FILE *f,char path[],WINDOW *command_window,WINDOW * game_window,WINDOW *user_window,int *map_row,int *map_col,char map_name[],char author[],char author_email[],
                    char user[],char user_email[],int scr_x);
/*!
@brief read the file and print it on screen
@code
        wmove(map_win,row-1,col-2);  
        refresh();
        wrefresh(map_win);      
 @endcode
 move the cursor to the lower-right coner of the map
 @param *map_win the window that contain the map to write the character on it
 @param col,row contain the row and col for the array
 @param map[row][col] 2d array store the map for later use,NOTE:  the col is read from the file PLUS 1(the '\n' character )
*/
void readFile(WINDOW *map_win,int row,int col,char map [row][col],char s[],char file_name[]);
/*!
    @brief update the character on the screen at the given Y,X based on the change of the map array
    @code
    wclear(map_win);
    refresh();
    wrefresh(map_win);
    @endcode
    before re-adding,first clear the whole window to avoid misdisplay
    @param cursorY,cursorX the position adding new character
*/
void updateMap(WINDOW *map_win,int row,int col,char map[row][col],int cursorY,int cursorX);
/*!
    @brief move and check the cursor to assure in the boundary of map, change the map array and call the updateMap function
    @code
         case ' ':
                wmove(game_window,cursorY,cursorX);;
                map[getcury(game_window)][getcurx(game_window)]=c;
                updateMap(game_window,map_row,map_col,map,cursorY,cursorX);
                break;
    @endcode
        
*/
void cursorMove(WINDOW *game_window,int map_row,int map_col,char map[map_row][map_col]);
/*! 
@brief spilt the input string to author and author email
@param fullString the firstline read from file

*/
void get_author_name_and_email(char fullString[],char author[], char email[]);
/*!
@brief fill the map array with border,last character of row with '\n' and the rest with space
*/
void initialize_map_array(int map_row,int map_col,char map[map_row][map_col]);
#endif