#ifndef PACMAN_H
#define PACMAN_H
struct pacman
{
	int pac_row;
	int pac_col;
	int current_direction;
	int score;
};
struct pacman *create_pacman();
void pacman_move(struct pacman *my_pacman,char map [35][66]);
int get_score(int row,int col,char map[35][66]);
#endif
