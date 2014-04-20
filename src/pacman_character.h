#ifndef PACMAN_CHARACTER_H
#define PACMAN_CHARACTER_H
struct pacman_char
{
	int pac_row;
	int pac_col;
	int current_direction;
	int score;
};
struct pacman_char *create_pacman_char();
void pacman_char_move(struct pacman_char *my_pacman_char,char map [35][66]);
int get_score(int row,int col,char map[35][66]);
#endif
