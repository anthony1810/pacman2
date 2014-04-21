#ifndef GHOST_CHARACTER_H
#define GHOST_CHARACTER_H
struct ghost_char
{
	int ghost_row;
	int ghost_col;
};
struct ghost_char *create_ghost_char();
void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col);
int transte_from_row_col(int row,int col,int map_col);
void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row+2)*(map_col+2)][(map_row+2)*(map_col+2)],int n,int prev[]);

#endif
