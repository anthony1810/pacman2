#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "read_file.h"
#include "constant.h"
#include "utility.h"
/*! @file read_file.c 
    @brief this file provide functions to handle reading from text file, modifying read text
*/

void read_command(FILE *f,char path[],WINDOW *command_window,WINDOW * game_window,WINDOW *user_window,int *map_row,int *map_col,char map_name[],char author[],char author_email[],
                    char user[],char user_email[],int scr_x){
            char s[100];
            //giong y chang code luc read
            fgets(s, 100, f);
            get_author_name_and_email(s,author,author_email);
            fgets(s, 100, f);
            fgets(s, 100, f);
            *map_row=atoi(s);
            fgets(s, 100, f);
            *map_col=atoi(s)+1;
            char map[*map_row][*map_col];         
            wclear(game_window);
            wrefresh(game_window);
            *game_window=create_new_win(*map_row,*map_col,GAME_STARTY,(scr_x)/3);
            readFile(game_window,*map_row,*map_col,map,s,map_name);
            fclose(f);
            wattron(command_window,COLOR_PAIR(8)); 
            wprintw(command_window,"Sucessfully read from ");
            wattron(command_window,A_UNDERLINE);   
            wprintw(command_window, map_name);
            wprintw(command_window,".pac");
            wattroff(command_window,A_UNDERLINE);  
            wprintw(command_window, ", press any key to edit..");
            wattroff(command_window,COLOR_PAIR(8));    
            wrefresh(command_window);
            init_user_info(user_window,user, user_email, map_name,*map_row,*map_col-1,author,author_email);
            cursorMove(game_window,*map_row,*map_col,map);        
}
void readFile(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[]){
    wclear(map_win);
    FILE *f ;
    char full_path[100]="" ;

    strcat(full_path,PATH);
    strcat(full_path,file_name);
    strcat(full_path,EXTENSION);
    f = fopen(full_path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open the file!\n");
    }
    char ch;
    int r=0;int c=0;
    //print Author info, map info, row ,column 
    fgets(s, 100, f);
    fgets(s, 100, f);
    fgets(s, 100, f);
    fgets(s, 100, f);

    do{
        ch=fgetc(f);
        wattron(map_win,COLOR_PAIR(1));
        switch(ch){
            case 'q':
            case 'Q':
                waddch(map_win,ACS_ULCORNER);
                map[r][c]='q';
                c++;
                break;
            case 'e':
            case 'E':
                waddch(map_win,ACS_URCORNER);
                map[r][c]='e';
                c++;
                break;
            case 'w':
            case 'x':
                waddch(map_win,ACS_HLINE);
                map[r][c]='w';
                c++;
                break;
            case 'a':               
            case 'd':
                waddch(map_win,ACS_VLINE);
                map[r][c]='a';
                c++;
                break;
            case 'z':
            case 'Z':
                waddch(map_win,ACS_LLCORNER);
                map[r][c]='z';
                c++;
                break; 
            case 'c':
            case 'C':
                waddch(map_win,ACS_LRCORNER);                            
                map[r][c]='c';
                c++;
                break;
            case 'A':
                waddch(map_win,ACS_LTEE);                
                map[r][c]='A';
                c++;
                break;
            case 'D':
                waddch(map_win,ACS_RTEE);
                map[r][c]='D';
                c++;
                break;
            case 'W':
                waddch(map_win,ACS_TTEE);                    
                map[r][c]='W';
                c++;
                break;
            case 'X':
                waddch(map_win,ACS_BTEE);    
                map[r][c]='X';
                c++;
                break;
            case '\n':
                waddch(map_win,'\n');
                map[r][c]='\n';
                r++;
                c=0;
                break;    
            case 'G':
            case 'g':
                wattron(map_win,COLOR_PAIR(3));
                waddch(map_win,ACS_CKBOARD);
                map[r][c]='G';
                c++;
                wattron(map_win,COLOR_PAIR(1));
                break;
            case 's':
                wattron(map_win,COLOR_PAIR(5));
                waddch(map_win,ACS_BULLET);
                map[r][c]='s';
                c++;
                wattron(map_win,COLOR_PAIR(1));
                break;
            case 'S':
                wattron(map_win,COLOR_PAIR(5));
                waddch(map_win,ACS_DEGREE);
                map[r][c]='S';
                c++;
                wattron(map_win,COLOR_PAIR(1));
                break;  
			case 'P':
            case 'p':
				wattron(map_win,COLOR_PAIR(2));
				waddch(map_win,ACS_DIAMOND);                  
				map[r][c]='P';
                c++;
				wattron(map_win,COLOR_PAIR(1));                   	
				break;
            case 'f':
            case 'F':
                wattron(map_win,COLOR_PAIR(4));
                waddch(map_win,ACS_STERLING);
                map[r][c]='f';
                c++;
                wattron(map_win,COLOR_PAIR(1));
                break;  
            case ' ':
                waddch(map_win,' ');
                map[r][c]=' ';
                c++;
                break;
            case ':':
                break;
        }
    }while(ch!=EOF);
    //move the cursor to the lower right of the map 
    wmove(map_win,row-1,col-2);
    
    refresh();
    wrefresh(map_win);

}

void updateMap(WINDOW *map_win,int row,int col,char map[row][col],int cursorY,int cursorX){
	wclear(map_win);
	refresh();
    wrefresh(map_win);
    wattron(map_win,COLOR_PAIR(1));
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			switch(map[i][j]){
            case 'q':
            case 'Q':
                waddch(map_win,ACS_ULCORNER);
                break;
            case 'e':
            case 'E':
                waddch(map_win,ACS_URCORNER);
                break;
            case 'w':
            case 'x':
                waddch(map_win,ACS_HLINE);
                break;
            case 'a':               
            case 'd':
                waddch(map_win,ACS_VLINE);                
                break;
            case 'z':
            case 'Z':
                waddch(map_win,ACS_LLCORNER);
                break; 
            case 'c':
            case 'C':
                waddch(map_win,ACS_LRCORNER);
                break;
            case 'A':
                waddch(map_win,ACS_LTEE);                
                break;
            case 'D':
                waddch(map_win,ACS_RTEE);
                break;
            case 'W':
                waddch(map_win,ACS_TTEE);                    
                break;
            case 'X':
                waddch(map_win,ACS_BTEE);    
                break;
            case '\n':
                waddch(map_win,'\n');
                break;    
            case 'G':
            case 'g':
                wattron(map_win,COLOR_PAIR(3));
                waddch(map_win,ACS_CKBOARD);
                wattron(map_win,COLOR_PAIR(1));                     
                break;
            case 's':
                wattron(map_win,COLOR_PAIR(5));
                waddch(map_win,ACS_BULLET);
                wattron(map_win,COLOR_PAIR(1));
                break;
            case 'S':
                wattron(map_win,COLOR_PAIR(5));
                waddch(map_win,ACS_DEGREE);
                wattron(map_win,COLOR_PAIR(1));
                break;  
			case 'P':
            case 'p':
				wattron(map_win,COLOR_PAIR(2));
				waddch(map_win,ACS_DIAMOND);                  
				wattron(map_win,COLOR_PAIR(1));                   	
				break;
            case 'f':
            case 'F':
                wattron(map_win,COLOR_PAIR(4));
                waddch(map_win,ACS_STERLING);
                wattron(map_win,COLOR_PAIR(1));
                break; 
            case ' ':
                waddch(map_win,' ');
                break;
        	}

		}

	}
    wmove(map_win,cursorY,cursorX);
	refresh();
    wrefresh(map_win);
}

void cursorMove(WINDOW *game_window,int map_row,int map_col,char map[map_row][map_col]){

    int c;
    int cursorX;
    int cursorY;

    while((c=getch())!=':'){
        cursorX=getcurx(game_window);
        cursorY=getcury(game_window);
        switch(c){
            case ' ':
                wmove(game_window,cursorY,cursorX);;
                map[getcury(game_window)][getcurx(game_window)]=c;
                updateMap(game_window,map_row,map_col,map,cursorY,cursorX);
                break;
            case KEY_UP:
                wmove(game_window,--cursorY,cursorX);
                break; 
            case KEY_DOWN:
                cursorY= (cursorY==map_row-1) ? map_row-1 : cursorY+1;
                wmove(game_window,cursorY,cursorX);                     
                break;
            case KEY_LEFT:
                wmove(game_window,cursorY,--cursorX);          
                break;
            case KEY_RIGHT:
                cursorX= (cursorX==map_col-2) ? map_col-2 : cursorX+1;
                wmove(game_window,cursorY,cursorX);
                break;
            case 'q':
            case 'Q':
            case 'E':
            case 'e':
            case 'w':
            case 'W':
            case 'a':
            case 'A':
            case 's':
            case 'S':
            case 'd':
            case 'D':
            case 'z':
            case 'Z':
            case 'x':
            case 'X':
            case 'c':
            case 'C':
            case 'G':
            case 'P':
            case 'f':
            case 'F':
                map[getcury(game_window)][getcurx(game_window)]=c;
                updateMap(game_window,map_row,map_col,map,cursorY,cursorX);
                break;
            }
            wrefresh(game_window);
    }
}

void get_author_name_and_email(char fullString[],char author[], char email[]){
    int emailBegin=0;
    int i;
    for (i = 0; fullString[i+1]!='>'; i++)
    {
        if(fullString[i]=='<'){
            emailBegin=i;
        }
        if(emailBegin==0){
            author[i]=fullString[i];
        }
        if(emailBegin!=0){
            email[i-emailBegin]=fullString[i+1];
        }
    }
    email[i-(emailBegin)]='\0';
    author[emailBegin-1]='\0';
}

void initialize_map_array(int map_row,int map_col,char map[map_row][map_col]){
    for (int i = 0; i < map_row; i++)
    {
        for (int j = 0; j < map_col; j++)
        {   if(j==map_col-1){
                map[i][j]='\n';
            }else{
                map[i][j]=' ';
            }
        }
    }
    for(int j=0;j<map_col-2;j++){
        map[0][j]='w';
        map[map_row-1][j]='x';
    }
    for(int i=0;i<map_row;i++){
        map[i][0]='a';
        map[i][map_col-2]='a';
    }

    map[0][0]='q';
    map[map_row-1][0]='z';
    map[0][map_col-2]='e';
    map[map_row-1][map_col-2]='c';
}

