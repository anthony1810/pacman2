#ifndef GHOST_CHARACTER_H
#define GHOST_CHARACTER_H
struct ghost_char
{
	int ghost_row;
	int ghost_col;
	int speed_multiplier;
};
struct ghost_char *create_ghost_char();
void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col);
int transte_from_row_col(int row,int col,int map_col);
void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row+2)*(map_col+2)][(map_row+2)*(map_col+2)],int n,int prev[]);
int checkWall(int row,int col,int map_col,char map[][map_col+1]);
void initialize_dist_array(int row,int col,int map_row,int map_col,long dist [][(map_row+2)*(map_col+2)],char map[][map_col+1]);
void printPath(int ghost_num,int dest,int prev[],int ghost_path_size,int ghost_path[][ghost_path_size],int *current_path);
void ghost_move(int ghost_path_size,int ghost_path[][ghost_path_size],int *current_num,int translate_row_col[],int map_row,int map_col,char map[][map_col+1],struct ghost_char *my_ghost_char);
#endif
