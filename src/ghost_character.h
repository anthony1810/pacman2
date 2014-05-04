#ifndef GHOST_CHARACTER_H
#define GHOST_CHARACTER_H
#include "struct.h"

void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col);
int transte_from_row_col(int row,int col,int map_col);
void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row)*(map_col)][(map_row)*(map_col)],int n,int prev[]);
int checkWall(int row,int col,int map_col,char map[][map_col+1]);
void initialize_dist_array(int row,int col,int map_row,int map_col,long dist [][(map_row)*(map_col)],char map[][map_col+1]);
void printPath(int ghost_num,int dest,int prev[],int ghost_path_size,int ghost_path[ghost_path_size],struct ghost_char *my_ghost_char);

void ghost_move(int ghost_path_size,int ghost_path[ghost_path_size],int translate_row_col[],
int map_row,int map_col,char map[][map_col+1],struct ghost_char *my_ghost_char,WINDOW *game_window);
chtype convert_to_map_character(char text_character);
#endif
