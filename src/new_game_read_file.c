#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "new_game_read_file.h"

#include "constant.h"

void new_game_read_file(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[],
                        struct pacman_char *my_pacman_char,struct ghost_char *my_ghost_char,struct map *my_map){
    wclear(map_win);
    FILE *f ;
    char full_path[100]="" ;

    strcat(full_path,PATH);
    strcat(full_path,file_name);
    strcat(full_path,".pac");
    f = fopen(full_path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open the file!\n");
    }
    char ch;
    int r=0;int c=0;
    int current_ghost=0;
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
                if(current_ghost==0){
                    wattron(map_win,COLOR_PAIR(1));    
                }//chase and random
                else if(current_ghost==1){
                    wattron(map_win,COLOR_PAIR(2));
                }//mimic pacman (yellow)
                else if(current_ghost==2){
                    wattron(map_win,COLOR_PAIR(3));
                }//chase
                else if(current_ghost==3){
                    wattron(map_win,COLOR_PAIR(4));
                }
                
                waddch(map_win,ACS_CKBOARD);
                map[r][c]='G';
                my_ghost_char[current_ghost].ghost_row=r;
                my_ghost_char[current_ghost].ghost_col=c;
                my_ghost_char[current_ghost].initial_ghost_row=r;
                my_ghost_char[current_ghost].initial_ghost_col=c;
                my_ghost_char[current_ghost].ghost_num=current_ghost;
                c++;
                current_ghost++;
                wattron(map_win,COLOR_PAIR(1));
                break;
            case 's':
                wattron(map_win,COLOR_PAIR(5));
                waddch(map_win,ACS_BULLET);
                map[r][c]='s';
                my_map->remain_pellet++;
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
                my_pacman_char->pac_col=c;
                my_pacman_char->pac_row=r;
                my_pacman_char->initial_pac_col=c;
                my_pacman_char->initial_pac_row=r;
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
    my_map->total_pellet=my_map->remain_pellet;
    refresh();
    wrefresh(map_win);

}

void new_game_update_map(WINDOW *map_win,int row,int col,char map[row][col]){
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
	refresh();
    wrefresh(map_win);
}

