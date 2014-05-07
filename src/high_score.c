#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "high_score.h"

int y_cur = 12;
int x_cur = 23;

void save_high_score(char user_email[], int score){
	FILE *fp;
	fp=fopen("high_score.txt", "a");
	fputs(user_email,fp);
	fputc('-',fp);
	fputc(score,fp);
	fputc('\n',fp);
	fclose(fp);
}

void show_option_log(WINDOW *note_window, int x_cur){
	if(x_cur == 23){
			wclear(note_window);
			wattron(note_window,COLOR_PAIR(1)); 
			wprintw(note_window, "EASY MODE\n");
			wattroff(note_window,COLOR_PAIR(1)); 
			wprintw(note_window, "- Power pellet duration lasts 15 seconds\n");
			wprintw(note_window, "- items such as pellet, fruit, .. will have more value\n\n");
			wprintw(note_window, "- Hunter Ghost create walls after 3 seconds\n");
			wprintw(note_window, "- Hunter Ghost will see pacman within only 5 meters\n");
			wprintw(note_window, "- Hunter Ghost will have less chance of attacking pacman\n\n");
			wprintw(note_window, "- caoanh Ghost speed equal 1/3 pacman speed \n");
			wrefresh(note_window);
		}else if(x_cur == 43){
			wclear(note_window);
			wattron(note_window,COLOR_PAIR(2)); 
			wprintw(note_window, "MEDIUM MODE\n");
			wattroff(note_window,COLOR_PAIR(2)); 
			wprintw(note_window, "- Power pellet duration lasts 10 seconds\n");
			wprintw(note_window, "- items such as pellet, fruit, .. will have normal value\n\n");
			wprintw(note_window, "- Hunter Ghost create walls after 1 seconds\n");
			wprintw(note_window, "- Hunter Ghost will see pacman within 10 meters\n");
			wprintw(note_window, "- Hunter Ghost will have more chance of attacking pacman\n\n");
			wprintw(note_window, "- caoanh Ghost speed equal pacman speed \n");
			wrefresh(note_window);
		}else if(x_cur == 63){
			wclear(note_window);
			wattron(note_window,COLOR_PAIR(7)); 
			wprintw(note_window, "IMPOSSIBLE MODE\n");
			wattroff(note_window,COLOR_PAIR(7)); 
			wprintw(note_window, "- Power pellet duration lasts 6 seconds\n");
			wprintw(note_window, "- items such as pellet, fruit, .. will have less value\n\n");
			wprintw(note_window, "- Hunter Ghost create walls after 0 seconds\n");
			wprintw(note_window, "- Hunter Ghost will see pacman within 15 meters\n");
			wprintw(note_window, "- Hunter Ghost will have high chance of attacking pacman\n\n");
			wprintw(note_window, "- caoanh Ghost speed equal 2 times pacman speed \n");
			wrefresh(note_window);
		}
}

void show_option(WINDOW *title_window, int *option){
	clear();
	int row,col;
	getmaxyx(stdscr,row,col);
	//init title, version and company
	*title_window = create_new_win(TITLE_HEIGHT, col, 0, 0);
	WINDOW note_window = create_new_win(100, 100, 20, 23);
	wprintw(&note_window, "fuck");
	wrefresh(&note_window);
	refresh();
	wattron(title_window,A_BOLD);
	wattron(title_window,COLOR_PAIR(6));
	mvwprintw(title_window,0,(col-strlen(TITLE))/2,"%s",TITLE);
	wattroff(title_window,A_BOLD);  
	mvwprintw(title_window,1,col/2 + strlen(TITLE)/2,"%s",VERSION);
	mvwprintw(title_window,2,col/2 + strlen(TITLE)/2,"%s",CREATOR);
	wattroff(title_window,COLOR_PAIR(6)); 
	wrefresh(title_window);

	attron(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);
	mvprintw(6, (col - strlen("GAME OPTION"))/2, "%s", "GAME OPTION");
	attroff(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);

	attron(A_BOLD | COLOR_PAIR(2));
	mvprintw(8, (col - strlen("-type 'a' and 'd' to select, 'enter' to choose and 'q' to quit-"))/2, "%s", "-type 'a' and 'd' to select, 'enter' to choose and 'q' to quit-");
	attroff(A_BOLD | COLOR_PAIR(2));

	attron(COLOR_PAIR(1)); 
	mvprintw(12,27,"%s","EASY");
	attroff(COLOR_PAIR(1)); 

	attron(COLOR_PAIR(2)); 
	mvprintw(12,47,"%s","MEDIUM");
	attroff(COLOR_PAIR(2)); 

	attron(COLOR_PAIR(7)); 
	mvprintw(12,67,"%s","IMPOSSIBLE");
	attroff(COLOR_PAIR(7)); 

	refresh();

	
	
	attron(COLOR_PAIR(6));
	mvprintw(y_cur,x_cur,"%s", "->");
	attroff(COLOR_PAIR(6));

	show_option_log(&note_window, x_cur);

	char ch;
	while((ch = getch()) != 'q'){
		if(ch == 'a' && x_cur > 23){
			mvprintw(y_cur,x_cur,"%s", "  ");
			x_cur -= 20;
			attron(COLOR_PAIR(6));
			mvprintw(y_cur,x_cur,"%s", "->");
			attroff(COLOR_PAIR(6));

			show_option_log(&note_window, x_cur);

			refresh();
		}
		if(ch == 'd' && x_cur < 63 ){
			mvprintw(y_cur,x_cur,"%s", "  ");
			x_cur += 20;
			attron(COLOR_PAIR(6));
			mvprintw(y_cur,x_cur,"%s", "->");
			attroff(COLOR_PAIR(6));
			show_option_log(&note_window, x_cur);
			refresh();
		}
		if(ch==10){
			if(x_cur == 23){
				*option = 1;
			}else if(x_cur == 43){
				*option = 2;
			}else{
				*option = 3;
			}
			break;
		}
	}
	refresh();
}

void show_credit(WINDOW *title_window){
	clear();
	cbreak();
	int row,col;
	getmaxyx(stdscr,row,col);

	//init title, version and company
	*title_window = create_new_win(TITLE_HEIGHT, col, 0, 0);
	refresh();
	wattron(title_window,A_BOLD);
	wattron(title_window,COLOR_PAIR(6));
	mvwprintw(title_window,0,(col-strlen(TITLE))/2,"%s",TITLE);
	wattroff(title_window,A_BOLD);  
	mvwprintw(title_window,1,col/2 + strlen(TITLE)/2,"%s",VERSION);
	mvwprintw(title_window,2,col/2 + strlen(TITLE)/2,"%s",CREATOR);
	wattroff(title_window,COLOR_PAIR(6)); 
	wrefresh(title_window);
	
	attron(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);
	mvprintw(6, (col - strlen("CREDIT"))/2, "%s", "CREDIT");
	attroff(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);

	attron(A_BOLD | COLOR_PAIR(2));
	mvprintw(7, (col - strlen("-type any key to return to the menu-"))/2, "%s", "-type any key to return to the menu-");
	attroff(A_BOLD | COLOR_PAIR(2));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(13, (col - strlen("Phuc Vo Duc-s3425481"))/2, "%s", "Phuc Vo Duc-s3425481");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(16, (col - strlen("Tran Nhat Quang-s3312399"))/2, "%s", "Tran Nhat Quang-s3312399");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(19, (col - strlen("Huynh Phung Cao Anh-s3357672"))/2, "%s", "Huynh Phung Cao Anh-s3357672");
	attroff(A_BOLD | COLOR_PAIR(7));

	// attron(A_BOLD | COLOR_PAIR(7));
	// mvprintw(22, (col - strlen("-type any key to return to the menu-"))/2, "%s", "-type any key to return to the menu-");
	// attroff(A_BOLD | COLOR_PAIR(7));
	getch();
}

void show_high_score(WINDOW *title_window, WINDOW *command_window){

	clear();
	cbreak();
	int row,col;
	getmaxyx(stdscr,row,col);

	//init title, version and company
	*title_window = create_new_win(TITLE_HEIGHT, col, 0, 0);
	refresh();
	wattron(title_window,A_BOLD);
	wattron(title_window,COLOR_PAIR(6));
	mvwprintw(title_window,0,(col-strlen(TITLE))/2,"%s",TITLE);
	wattroff(title_window,A_BOLD);  
	mvwprintw(title_window,1,col/2 + strlen(TITLE)/2,"%s",VERSION);
	mvwprintw(title_window,2,col/2 + strlen(TITLE)/2,"%s",CREATOR);
	wattroff(title_window,COLOR_PAIR(6)); 
	wrefresh(title_window);
	
	attron(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);
	mvprintw(6, (col - strlen("HIGH SCORE"))/2, "%s", "HIGH SCORE");
	attroff(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);

	attron(A_BOLD | COLOR_PAIR(2));
	mvprintw(7, (col - strlen("-type any key to return to the menu-"))/2, "%s", "-type any key to return to the menu-");
	attroff(A_BOLD | COLOR_PAIR(2));


	FILE * pFile;
	char user_email[100];
	char string[100];
	pFile = fopen ("high_score.txt","r");

	struct Node first;
	first.score = 0;
	struct Node second;
	second.score = 0;
	struct Node third;
	third.score = 0;
	struct Node temp;
	third.score = 0;

	struct Node fourth;
	fourth.score = 0;
	struct Node fifth;
	fifth.score = 0;


	if (pFile!=NULL)
	{
		while( fgets(string,100,pFile) != NULL)
		{	 			
			char *token;
			/* get the first token */
			token = strtok(string,"-");
			strcpy(user_email,token);
			token = strtok(NULL,"-");
			int numb = atoi(token);

			if(numb>first.score){
				strcpy(temp.user_email,first.user_email);
				temp.score = first.score;
				strcpy(first.user_email,user_email);
				first.score = numb;
				strcpy(second.user_email,temp.user_email);
				second.score = temp.score;
			}else if(numb>second.score){
				strcpy(temp.user_email,second.user_email);
				temp.score = second.score;
				strcpy(second.user_email,user_email);
				second.score = numb;
				strcpy(third.user_email,temp.user_email);
				third.score = temp.score;
			}else if(numb>third.score){
				strcpy(temp.user_email,third.user_email);
				temp.score = third.score;
				strcpy(third.user_email,user_email);
				third.score = numb;
				strcpy(fourth.user_email,temp.user_email);
				fourth.score = temp.score;
			}else if(numb>fourth.score){
				strcpy(temp.user_email,fourth.user_email);
				temp.score = fourth.score;
				strcpy(fourth.user_email,user_email);
				fourth.score = numb;
				strcpy(fifth.user_email,temp.user_email);
				fifth.score = temp.score;
			}
			else if(numb>fifth.score){
				strcpy(fifth.user_email,user_email);
				fifth.score = numb;
			}
		}
	}

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(10, (col - strlen(" #1 - TRUE-GOD"))/2," #1 - Rampage");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(5));
	mvprintw(11,(col - strlen(" #1 - TRUE-GOD"))/2,"%s - %i", first.user_email,first.score);
	attroff(COLOR_PAIR(5));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(13, (col - strlen(" #2 - GOD-LIKE"))/2, "%s", " #2 - GOD-LIKE");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(4));
	mvprintw(14,(col - strlen(" #2 - GOD-LIKE"))/2,"%s - %i", second.user_email,second.score);
	attroff(COLOR_PAIR(4));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(16, (col - strlen(" #2 - GOD-LIKE"))/2, "%s", " #3 - WICKED-SICK");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(3));
	mvprintw(17, (col - strlen(" #2 - GOD-LIKE"))/2,"%s - %i", third.user_email,third.score);
	attroff(COLOR_PAIR(3));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(19, (col - strlen(" #2 - GOD-LIKE"))/2, "%s", " #4 - DOMINATED");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(2));
	mvprintw(20,(col - strlen(" #2 - GOD-LIKE"))/2,"%s - %i",  fourth.user_email,fourth.score);
	attroff(COLOR_PAIR(2));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(22, (col - strlen(" #2 - GOD-LIKE"))/2, "%s", " #5 - ABNORMAL");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(1));
	mvprintw(23, (col - strlen(" #2 - GOD-LIKE"))/2,"%s - %i",  fifth.user_email,fifth.score);
	attroff(COLOR_PAIR(1));

	//wclear(command_window);
	//wprintw(command_window, "type any key to quit");
	curs_set(0);
	refresh();
	
	fclose (pFile); 

	getch();
}

