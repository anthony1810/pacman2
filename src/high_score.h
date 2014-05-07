#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include "utility.h"
#include "constant.h"


struct Node {
	char user_email[100];
	int score;
};

void save_high_score(char user_email[], int score);
void show_high_score(WINDOW *hs_win,WINDOW *command_window);
void show_credit(WINDOW *title_win);

#endif