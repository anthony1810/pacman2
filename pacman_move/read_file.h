#ifndef READ_FILE_H
#define READ_FILE_H
#include "pacman.h"
/*! @file read_file.h 
    @brief this is the header file of \ref read_file.c.
*/
/*!
@brief readFile,get_author_name_and_email function will be called within.
*/
void readFile(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[],struct pacman *my_pacman,int *ghost_row,int*ghost_col);

void updateMap(WINDOW *map_win,int row,int col,char map[row][col]);
#endif
