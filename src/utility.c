#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include "constant.h"
#include "read_file.h"
#include "write_file.h"
#include "command.h"

/*! \file utility.c
	\brief this file contains some useful utilities for the main program and other modules of this program
	\author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/

WINDOW create_new_win(int height, int width, int starty, int startx)
{	
	WINDOW local_win;
	local_win = *newwin(height, width, starty, startx);
	return local_win;

}
void init_screen(char user[], char user_email[]){
	initscr();	

	if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WALL,    COLOR_BACKGROUND);
        init_pair(2, COLOR_PACMAN,  COLOR_BACKGROUND);
		init_pair(3, COLOR_GHOST,  COLOR_BACKGROUND);
		init_pair(4, COLOR_FRUIT,  COLOR_BACKGROUND);
		init_pair(5, COLOR_PELLET,  COLOR_BACKGROUND);
		init_pair(6, COLOR_YELLOW, COLOR_BACKGROUND);
		init_pair(7, 5, COLOR_BACKGROUND);
		init_pair(8, COLOR_GREEN, COLOR_BACKGROUND);
    }
    int counter =0;
    printw("Welcome to ");
	attron(COLOR_PAIR(6) | A_BOLD);
	printw(TITLE);
	attroff(COLOR_PAIR(6) | A_BOLD);
	printw(" powered by");
	attron(COLOR_PAIR(5) | A_BOLD);
	printw(" The Thunder Corp");
	attroff(COLOR_PAIR(5) | A_BOLD);
	printw(".\n\n");
	printw("Please Tell me your name (30 letters) and email (50 letters) so i can save scores,maps under your\nidentity, Thank you! \n\n");
	//getch();

    while(strlen(user) < 2 && strlen(user_email) < 2){
    	if(counter >= 1){
    		attron(COLOR_PAIR(3) | A_BOLD);
    		printw("Don't do that trick on me, Laziness is a big crime. Be mature! \n");
    		attroff(COLOR_PAIR(3) | A_BOLD);
    	}
    	counter +=1;
		attron(A_UNDERLINE);
		printw("Your Name:");
		attroff(A_UNDERLINE);
		printw(" ");
		getnstr(user,30);
		attron(A_UNDERLINE);
		printw("Your Email:");
		attroff(A_UNDERLINE);
		printw(" ");
		getnstr(user_email,50);
	}
	clear();
	raw();			/**< Line buffering disabled	 */
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();
}

void init_user_info(WINDOW* user_window,char user[], char user_email[], char map_name[], int map_row, int map_col, char author[], char author_email[]){
	wclear(user_window);
	wattron(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "User Info\n");
	wattroff(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "User:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, user);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Email:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, user_email);
	wattroff(user_window,COLOR_PAIR(2));


	wprintw(user_window,"\n\n");
	wattron(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "Map Info\n");
	wattroff(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);

	wprintw(user_window, "Title:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window,map_name );
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Creator:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, author);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Email:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, author_email);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Dimesion:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window,"%i" ,map_row);
	wprintw(user_window,"x");
	wprintw(user_window,"%i" ,map_col);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wrefresh(user_window);
}

void init_editor(WINDOW *title_window, WINDOW *game_window, WINDOW *command_window, 
	WINDOW *note_window, WINDOW *wall, WINDOW *user_window, char user[], char user_email[], char path[100],
	int isEnter, char author[50], char map_name[50], char author_email[50], int map_row, int map_col, int scr_x,int scr_y, FILE *f,int argc, char *argv[]){

	int row,col;
	getmaxyx(stdscr,row,col);

	int note_length = strlen(pacman) + strlen(ghost) + strlen(pellet) + strlen(super_pellet) + strlen(fruit) + (strlen(sepChar)*5)+5;
	
	
	*game_window = create_new_win(GAME_HEIGHT, GAME_WIDTH, GAME_STARTY, col/2);
	*command_window = create_new_win(COMMAND_HEIGHT, col, COMMAND_STARTY, 0);
	*note_window = create_new_win(TITLE_HEIGHT, col, 4, (col - note_length)/2);
	int user_window_width = 27;
	int user_window_startY = TITLE_HEIGHT+3;
	*user_window = create_new_win(10, user_window_width, user_window_startY,1) ;
	*wall = create_new_win(GAME_HEIGHT+2,1,user_window_startY-2,user_window_width+2);
	refresh();
	for(int i=0;i<GAME_HEIGHT;i++){
		mvwprintw(wall,i,0,"%s", "|");
	}
	wrefresh(wall);


	wrefresh(user_window);

	
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

	wattron(note_window,COLOR_PAIR(2));
	waddch(note_window,ACS_DIAMOND);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window,pacman);
	wprintw(note_window,sepChar);


	wattron(note_window,COLOR_PAIR(3));
	waddch(note_window, ACS_CKBOARD);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, ghost);
	wprintw(note_window,sepChar);
	

	wattron(note_window,COLOR_PAIR(5));
	waddch(note_window, ACS_BULLET);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, pellet);
	wprintw(note_window,sepChar);
	
	
	wattron(note_window,COLOR_PAIR(4));
	waddch(note_window, ACS_STERLING);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, fruit);
	wprintw(note_window,sepChar);


	wattron(note_window,COLOR_PAIR(5));
	waddch(note_window, ACS_DEGREE);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, super_pellet);
	
	
	wrefresh(note_window);

	init_user_info(user_window,user, user_email, "",0,0,"","");
	int i;


	for(i =0; i< col;i++){
		mvprintw(5,i,"%s","-");
	}

	wrefresh(title_window);
	// wprintw(game_window,"");
	// wrefresh(game_window);
	wprintw(command_window,"\n");

	for(i =0; i< col;i++){
		mvprintw(41,i,"%s","-");
	}
	curs_set(0);
	wattron(command_window,COLOR_PAIR(6));
	mvwprintw(command_window,0,0,"%s", "To enable command mode, type ':' ");
	wattroff(command_window,COLOR_PAIR(6));
	wrefresh(command_window);


	int ch;
	int input;

	 if(argc==2){
	 	curs_set(1);
        strcpy(path,PATH);
        strcat(path,argv[1]);
        strcat(path,EXTENSION);
        strcpy(map_name,argv[1]);     
        f = fopen(path, "r");
        if(!f){
            wprintw(command_window,"%s", " ,File not found ");
            wrefresh(command_window);
        }else{ wclear(command_window);		
			read_command(f,path,command_window,game_window,user_window,&map_row,&map_col,map_name,author,author_email,user,user_email,scr_x);		
			wclear(command_window);
			wattron(command_window,COLOR_PAIR(6));
			wprintw(command_window,"To enable command mode, type ':' ");
			wattroff(command_window,COLOR_PAIR(6));
			wrefresh(command_window);
	 	}
    }


	const char str_error[8] = "Error: ";
	const char s[2] = " ";
	while((ch = getch()) != 'q'){
		if(ch == ':'){
			curs_set(1);
			wclear(command_window);
			waddch(command_window, ':');
			wrefresh(command_window);
			while((input = getch())!= 27){
				waddch(command_window,input);
				wrefresh(command_window);
				if(input == KEY_BACKSPACE || input == KEY_DC){
					wclear(command_window);
					waddch(command_window, ':');
					wrefresh(command_window);
				}else if(input == 10 && isEnter == 0){
					isEnter = 1;
					//int characters
					char commands[100] = "";
						for(int k=0;k<100;k++){
							int characters = (int)mvwinch(command_window, 0, k);
							char Str;	
							if(k==1 && characters==32){
								break;
							}else{
								if(characters!=58 && characters!=4555){
									sprintf(&Str, "%c", characters);
									strcat(commands,&Str);
								}
							}
						}																
					if(strlen(commands)>0){
						char *token;
						char *str_recieve[4];
						wrefresh(command_window);		   
						/* get the first token */
						token = strtok(commands,s);
						str_recieve[0] = token;
						token = strtok(NULL, s);
						str_recieve[1] = token;
						token = strtok(NULL, s);
						str_recieve[2] = token;
						token = strtok(NULL, s);
						str_recieve[3] = token;

						wclear(command_window);
						if(strcmp(commands,"q")==0){
							break;
						}else if(strcmp(commands,"w")==0 && str_recieve[1]== NULL){
							if(strlen(map_name)>0 && strlen(author)>0 && strlen(author_email)>0){
								write_to_file(map_name, author, author_email, game_window,map_col-1, map_row);
							}else if(strlen(map_name)>0 && strlen(user)>0 && strlen(user_email)>0){
								write_to_file(map_name, user, user_email, game_window,map_col-1, map_row);
							}
							
							wattron(command_window,COLOR_PAIR(8));
							wprintw(command_window,"Successfully write to ");
							wattron(command_window, A_UNDERLINE);
							wprintw(command_window, map_name);	
							wprintw(command_window,".pac");
							wattroff(command_window, A_UNDERLINE);
							wprintw(command_window," ! press any key to continue!");
							wattroff(command_window,COLOR_PAIR(8));
							wrefresh(command_window);			
							isEnter=0;
							getch();
							stop_command_window(command_window, game_window,map_row,map_col);
							start_command_window(command_window, COMMAND_STARTY);
						}else if(strcmp(str_recieve[0],"w") == 0 && str_recieve[1] != NULL){
							strcpy(map_name,str_recieve[1]);
							if(strlen(map_name)>0 && strlen(author)>0 && strlen(author_email)>0){
								write_to_file(map_name, author, author_email, game_window,map_col-1, map_row);
							}else if(strlen(map_name)>0 && strlen(user)>0 && strlen(user_email)>0){
								write_to_file(map_name, user, user_email, game_window,map_col-1, map_row);
							}

							wattron(command_window,COLOR_PAIR(8));
							wprintw(command_window,"Sucessfully write to ");
							wattron(command_window,A_UNDERLINE);
							wprintw(command_window, str_recieve[1]);
							wprintw(command_window,".pac");
							wattroff(command_window,A_UNDERLINE);
							wprintw(command_window, " ! press any key to continue!");		
							wattroff(command_window,COLOR_PAIR(8));						
							wrefresh(command_window);
							getch();
							stop_command_window(command_window, game_window,map_row,map_col);	
							isEnter=0;
							start_command_window(command_window, COMMAND_STARTY);
						}else if(strcmp(commands,"wq")==0 && str_recieve[1]== NULL){
							if(strlen(map_name)>0 && strlen(author)>0 && strlen(author_email)>0){
								write_to_file(map_name, author, author_email, game_window,map_col-1, map_row);
							}else if(strlen(map_name)>0 && strlen(user)>0 && strlen(user_email)>0){
								write_to_file(map_name, user, user_email, game_window,map_col-1, map_row);
							}
							wattron(command_window,COLOR_PAIR(8));	
							wprintw(command_window,"Successfully write to ");
							wattron(command_window,A_UNDERLINE);	
							wprintw(command_window, map_name);
							wprintw(command_window,".pac");
							wattroff(command_window,A_UNDERLINE);	
							wprintw(command_window," and will quit by typing any key");
							wrefresh(command_window);
							getch();
							wattroff(command_window,COLOR_PAIR(8));	
							break;
						}else if(strcmp(commands,"wq")==0 && str_recieve[1]!= NULL){
							strcpy(map_name, str_recieve[1]);
							if(strlen(map_name)>0 && strlen(author)>0 && strlen(author_email)>0){
								write_to_file(map_name, author, author_email, game_window,map_col-1, map_row);
							}else if(strlen(map_name)>0 && strlen(user)>0 && strlen(user_email)>0){
								write_to_file(map_name, user, user_email, game_window,map_col-1, map_row);
							}
							wattron(command_window,COLOR_PAIR(8));	
							wprintw(command_window,"Sucessfully write to ");
							wprintw(command_window, str_recieve[1]);
							wattron(command_window,A_UNDERLINE);	
							wprintw(command_window,".pac");
							wattroff(command_window,A_UNDERLINE);	
							wprintw(command_window, " and will quit by typing any key");
							wrefresh(command_window);
							getch();
							wattroff(command_window,COLOR_PAIR(8));	
							break;
						}else if(strcmp(str_recieve[0],"r") == 0 && str_recieve[1] != NULL){
							    strcpy(map_name,str_recieve[1]);
							    strcpy(path,PATH);
							    strcat(path,str_recieve[1]);
							    strcat(path,EXTENSION);
							    f = fopen(path, "r");
								if(!f){									
									wattron(command_window,COLOR_PAIR(7));
									wprintw(command_window, str_error);
									wattroff(command_window,COLOR_PAIR(7));
									wprintw(command_window, "File not found! type any key to continue!");
									wrefresh(command_window);
									getch();
									wclear(command_window);
									start_command_window(command_window, COMMAND_STARTY);
								}else{
									read_command(f,path,command_window,game_window,user_window,&map_row,&map_col,map_name,author,author_email,
                    					user,user_email,col);	
									start_command_window(command_window, COMMAND_STARTY);
								}
								isEnter=0;
						}else if(strcmp(str_recieve[0],"n") == 0 && str_recieve[1] != NULL 
								&& str_recieve[2] != NULL && str_recieve[3] != NULL){
								if(atoi(str_recieve[2]) > 0 && atoi(str_recieve[2]) <= 35 && atoi(str_recieve[3]) > 0 && atoi(str_recieve[3]) <=65){
									wclear(game_window);
							    	wrefresh(game_window);
									strcpy(map_name,str_recieve[1]);
									map_row = atoi(str_recieve[2]);
									map_col = atoi(str_recieve[3])+1;

									//FILE *fp;
									strcpy(path,PATH);
								 	strcat(path,map_name);
									strcat(path,EXTENSION);

									char map[map_row][map_col];
									initialize_map_array(map_row,map_col,map);
									strcpy(author,user);
									strcpy(author_email,user_email);
									//write_to_file(map_name, "CaoAnh", "s3357672@rmit.edu.vn", &game_window,map_col-1, map_row);
									wattron(command_window,COLOR_PAIR(8));	
									wprintw(command_window,"Sucessfully create a map called ");
									wattron(command_window,A_UNDERLINE);	
									wprintw(command_window,map_name);
									wprintw(command_window,".pac ");
									wattroff(command_window,A_UNDERLINE);	
									wprintw(command_window,"with row = ");
									wprintw(command_window, "%d",map_row);
									wprintw(command_window," and col = ");
									wprintw(command_window,"%d", map_col-1);
									wattroff(command_window,COLOR_PAIR(8));	
									wrefresh(command_window);
									init_user_info(user_window,user, user_email, map_name,map_row,map_col-1,author,author_email);
								    // stop_command_window(&command_window,&game_window, map_row, map_col);	
								    *game_window=create_new_win(map_row,map_col,GAME_STARTY,col);

								    wmove(game_window,map_row-1,map_col-2);
								    updateMap(game_window,map_row,map_col,map,getcury(game_window),getcurx(game_window));
								    refresh();
								    wrefresh(game_window);
								    cursorMove(game_window,map_row,map_col,map);
								}else{
									wattron(command_window,COLOR_PAIR(7));
									wprintw(command_window,str_error);
									wattroff(command_window,COLOR_PAIR(7));
									wprintw(command_window,"Map rows or cols is invalid! type any key to continue");
									wrefresh(command_window);
									getch();
									stop_command_window(command_window,game_window, map_row, map_col);	
								}
								isEnter=0;
								start_command_window(command_window, COMMAND_STARTY);

						}else{
								wattron(command_window,COLOR_PAIR(7));
								wprintw(command_window,str_error);
								wattroff(command_window,COLOR_PAIR(7));
								wprintw(command_window, "Can't recognise that commands! type any key to continue!");
								wrefresh(command_window);
								wclear(command_window);
								waddch(command_window,':');
								isEnter = 0;
						}
					}else {
								wclear(command_window);
								wattron(command_window,COLOR_PAIR(7));
								wprintw(command_window,str_error);
								wattroff(command_window,COLOR_PAIR(7));
								wprintw(command_window, "Can't recognise that commands! type any key to continue!");
								wrefresh(command_window);
								wclear(command_window);
								waddch(command_window,':');
								isEnter = 0;
						}
				}	
			}
			stop_command_window(command_window,game_window, map_row, map_col);
			// char map[map_row][map_col];
			// for(int i=0;i<map_row;i++){
			// 	for(int j=0;j<=map_col-1;j++){
			// 		if(j==map_col){
			// 			map[i][j]='\n';
			// 		}else{
			// 			int characters = (int)mvwinch(game_window, i, j);
			// 			map[i][j]=getTranslatedChar(characters);
			// 		}
			// 	}
			// }

			// wmove(game_window,map_row-1,map_col-2);
			// updateMap(game_window,map_row,map_col,map,getcury(game_window),getcurx(game_window));
			// refresh();
			// wrefresh(game_window);
			// cursorMove(game_window,map_row,map_col,map);	
			break;
		}
	}

}

void init_menu(WINDOW *title_window){
	//init menu

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
	mvprintw(6, (col - strlen(MENU_TITLE))/2, "%s", MENU_TITLE);
	attroff(A_BOLD | COLOR_PAIR(4) | A_UNDERLINE);

	attron(A_BOLD | COLOR_PAIR(7));
	mvprintw(8, (col - strlen(MENU_INSTRUCTION))/2, "%s", MENU_INSTRUCTION);
	attroff(A_BOLD | COLOR_PAIR(7));

	attron(COLOR_PAIR(5));
	mvprintw(10,(col-strlen(MENU_NEW))/2,"%s", MENU_NEW);
	mvprintw(12,(col-strlen(MENU_HSCORE))/2,"%s", MENU_HSCORE);
	mvprintw(14,(col-strlen(MENU_EDITOR))/2,"%s", MENU_EDITOR);
	mvprintw(16,(col-strlen(MENU_CREDIT))/2,"%s",  MENU_CREDIT);
	mvprintw(18,(col-strlen(MENU_END))/2,"%s",  MENU_END);
	attroff(COLOR_PAIR(5));

	curs_set(0);
	refresh();

}

void start_stats(WINDOW *user_window,char user[],char user_email[], int scores, int life, int level){
	wclear(user_window);
	wattron(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "User Info\n");
	wattroff(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "User:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, user);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Email:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window, user_email);
	wattroff(user_window,COLOR_PAIR(2));


	wprintw(user_window,"\n\n");
	wattron(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);
	wprintw(user_window, "Game Info\n");
	wattroff(user_window,A_BOLD | COLOR_PAIR(1) | A_UNDERLINE);

	wprintw(user_window, "Level:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window,"%i" ,level );
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Scores:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window,"%i" ,scores);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wprintw(user_window,"Life:");
	wprintw(user_window, " ");
	wattron(user_window,COLOR_PAIR(2));
	wprintw(user_window,"%i" , life);
	wattroff(user_window,COLOR_PAIR(2));
	wprintw(user_window, "\n");

	wrefresh(user_window);

}

void init_game(WINDOW *title_window, WINDOW *game_window, WINDOW *command_window, WINDOW *note_window, WINDOW *wall, WINDOW *user_window, char user[], char user_email[], int level){
	clear();
	int row,col;
	getmaxyx(stdscr,row,col);

	int note_length = strlen(pacman) + strlen(ghost) + strlen(pellet) + strlen(super_pellet) + strlen(fruit) + (strlen(sepChar)*5)+5;
	
	
	*game_window = create_new_win(GAME_HEIGHT, GAME_WIDTH, GAME_STARTY, 30);
	*command_window = create_new_win(COMMAND_HEIGHT, col, COMMAND_STARTY, 0);
	*note_window = create_new_win(TITLE_HEIGHT, col, 4, (col - note_length)/2);
	int user_window_width = 27;
	int user_window_startY = TITLE_HEIGHT+3;
	*user_window = create_new_win(10, user_window_width, user_window_startY,1) ;
	*wall = create_new_win(GAME_HEIGHT+2,1,user_window_startY-2,user_window_width+2);
	refresh();
	for(int i=0;i<GAME_HEIGHT;i++){
		mvwprintw(wall,i,0,"%s", "|");
	}
	wrefresh(wall);


	wrefresh(user_window);

	
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

	wattron(note_window,COLOR_PAIR(2));
	waddch(note_window,ACS_DIAMOND);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window,pacman);
	wprintw(note_window,sepChar);


	wattron(note_window,COLOR_PAIR(3));
	waddch(note_window, ACS_CKBOARD);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, ghost);
	wprintw(note_window,sepChar);
	

	wattron(note_window,COLOR_PAIR(5));
	waddch(note_window, ACS_BULLET);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, pellet);
	wprintw(note_window,sepChar);
	
	
	wattron(note_window,COLOR_PAIR(4));
	waddch(note_window, ACS_STERLING);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, fruit);
	wprintw(note_window,sepChar);


	wattron(note_window,COLOR_PAIR(5));
	waddch(note_window, ACS_DEGREE);
	wattron(note_window,COLOR_PAIR(1));
	wprintw(note_window, super_pellet);
	
	
	wrefresh(note_window);
	start_stats(user_window,user, user_email, 0, 3, level);
	
	int i;


	for(i =0; i< col;i++){
		mvprintw(5,i,"%s","-");
	}

	wrefresh(title_window);
	// wprintw(game_window,"");
	// wrefresh(game_window);
	wprintw(command_window,"\n");

	for(i =0; i< col;i++){
		mvprintw(41,i,"%s","-");
	}
	wattron(command_window,COLOR_PAIR(6));
	mvwprintw(command_window,0,0,"%s", "Press anything to return to the menu");
	wattroff(command_window,COLOR_PAIR(6));
	wrefresh(command_window);

	}


	long long timeval_diff(struct timeval *difference,
             struct timeval *end_time,
             struct timeval *start_time
            )
		{
		  struct timeval temp_diff;

		  if(difference==NULL)
		  {
		    difference=&temp_diff;
		  }

		  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
		  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

		  /* Using while instead of if below makes the code slightly more robust. */

		  while(difference->tv_usec<0)
		  {
		    difference->tv_usec+=1000000;
		    difference->tv_sec -=1;
		  }

		  return (1000000LL*difference->tv_sec+
		                   difference->tv_usec)/1000;

	} 