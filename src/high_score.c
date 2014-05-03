#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "high_score.h"


void save_high_score(char user_email[], int score){
	FILE *fp;
	fp=fopen("high_score.txt", "a");
	fputs(user_email,fp);
	fputc('-',fp);
	fputc(score,fp);
	fputc('\n',fp);
	fclose(fp);
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
	mvprintw(10, (col - strlen("RANK #1 - TRUE-GOD"))/2,"RANK #1 - TRUE-GOD");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(5));
	mvprintw(11,(col-strlen(first.user_email))/2,"%s - %i", first.user_email,first.score);
	attroff(COLOR_PAIR(5));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(13, (col - strlen("RANK #2 - GOD-LIKE"))/2, "%s", "RANK #2 - GOD-LIKE");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(4));
	mvprintw(14,(col-strlen(second.user_email))/2,"%s - %i", second.user_email,second.score);
	attroff(COLOR_PAIR(4));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(16, (col - strlen("RANK #3 - WICKED-SICK"))/2, "%s", "RANK #3 - WICKED-SICK");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(3));
	mvprintw(17,(col-strlen(third.user_email))/2,"%s - %i", third.user_email,third.score);
	attroff(COLOR_PAIR(3));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(19, (col - strlen("RANK #4 - DOMINATED"))/2, "%s", "RANK #4 - DOMINATED");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(2));
	mvprintw(20,(col-strlen(fourth.user_email))/2,"%s - %i",  fourth.user_email,fourth.score);
	attroff(COLOR_PAIR(2));

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(22, (col - strlen("RANK #5 - ABNORMAL"))/2, "%s", "RANK #5 - ABNORMAL");
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(1));
	mvprintw(23,(col-strlen(fifth.user_email))/2,"%s - %i",  fifth.user_email,fifth.score);
	attroff(COLOR_PAIR(1));

	//wclear(command_window);
	//wprintw(command_window, "type any key to quit");
	curs_set(0);
	refresh();
	
	fclose (pFile); 

	getch();
}

