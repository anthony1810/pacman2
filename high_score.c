#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>




void save_high_score(char user_email[], int score){
	FILE *fp;
	fp=fopen("high_score.txt", "a");
	fputs(user_email,fp);
	fputc('-',fp);
	fputc(score,fp);
	fputc('\n',fp);
	fclose(fp);
}

void show_high_score(WINDOW *title_window){

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

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(8, (col - strlen(MENU_INSTRUCTION))/2, "%s", MENU_INSTRUCTION);
	attroff(A_BOLD | COLOR_PAIR(7));



	FILE * pFile;
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


	attron(COLOR_PAIR(5));
	mvprintw(10,(col-strlen(first.user_email))/2,"%s:%i", first.user_email,first.score);
	mvprintw(12,(col-strlen(second.user_email))/2,"%s:%i", second.user_email,second.score);
	mvprintw(14,(col-strlen(third.user_email))/2,"%s:%i", third.user_email,third.score);
	mvprintw(16,(col-strlen(fourth.user_email))/2,"%s:%i",  fourth.user_email,fourth.score);
	mvprintw(18,(col-strlen(fifth.user_email))/2,"%s:%i",  fifth.user_email,fifth.score);
	attroff(COLOR_PAIR(5));

	curs_set(0);
	refresh();
	
	getch();
	
	// wprintw(hs_win,"%s:%i \n",first.user_email,first.score);
	// wprintw(hs_win,"%s:%i \n",second.user_email,second.score);
	// wprintw(hs_win,"%s:%i \n",third.user_email,third.score);
	// wprintw(hs_win,"%s:%i \n",fourth.user_email,fourth.score);
	// wprintw(hs_win,"%s:%i \n",fifth.user_email,fifth.score);
	// wrefresh(hs_win);
	fclose (pFile); 
}

int main(){
	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 20;
	width = 20;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/

	my_win = create_newwin(height, width, starty, startx);
	refresh();

	// char user_email[20];
	// int score;

	// getnstr(user_email,20);
	// score = getch();

	// save_high_score(user_email,score);
	show_high_score(my_win);
	getch();

	endwin();
	return 0;
}