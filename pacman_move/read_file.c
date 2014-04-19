#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "read_file.h"


void readFile(WINDOW *map_win,int row,int col,char map[row][col], char s[],char file_name[],struct pacman *my_pacman,int*ghost_row,int *ghost_col){
    wclear(map_win);
    FILE *f ;
    char full_path[100]="" ;

    // strcat(full_path,PATH);
    strcat(full_path,file_name);
    strcat(full_path,".pac");
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
                *ghost_row=r;
                *ghost_col=c;
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
                my_pacman->pac_col=c;
                my_pacman->pac_row=r;
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

    refresh();
    wrefresh(map_win);

}

void updateMap(WINDOW *map_win,int row,int col,char map[row][col]){
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
// void initialize_dist_array(int row,int col,long dist[row][col],int border_row,int border_col,char map[border_row][border_col]){
//     // col *(r-1)+c
//     for(int r=1;r<=row;r++){
//         for(int c=1;c<=col;c++){
//             // if(r==1&&c==1){
//             //     dist[1][2]= checkWall(1,2,boder_row,boder_col,map);
//             //     dist[1][col+1]=checkWall(2,1,boder_row,boder_col,map);
//             // }else if(r==1 && c==col){
//             //     dist[c][c-1]= checkWall(r,c-1,boder_row,boder_col,map);
//             //     dist[c][c* (r+1)]= checkWall(r+1,c,boder_row,boder_col,map)
//             // }else if(r==row && c==1){
//             //     dist[col*(row-1)+1 ][col*(row-2)+1 ]
//             //     dist[col*(row-1)+1 ][col*(row-1)+2]
//             // }
//             dist[col *(r-1)+c][col *(r-1)+(c+1)]=checkWall(r,c+1,border_row,border_col,map);
//             dist[col *(r-1)+c][col *(r-1)+(c-1)]=checkWall(r,c-1,border_row,border_col,map);
//             if(r-2>0){
//             dist[col *(r-1)+c][col *(r-2)+c]=checkWall(r-1,c,border_row,border_col,map);
//             }
//             dist[col *(r-1)+c][col *(r)+c]=checkWall(r+1,c,border_row,border_col,map);
//         }
//     }  

// }
// int checkWall(int row,int col,int border_row,int border_col,char map[border_row][border_col]){
//     if(map[row][col]=='s'|| map[row][col]=='S' ||
//         map[row][col]=='f' || map[row][col]=='F' ||
//         map[row][col]==' ' ){
//         return 1;
//     }
//     return 999;
// }