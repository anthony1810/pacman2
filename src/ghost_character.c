#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "ghost_character.h"
#define INFINITY 999

void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col){
    int map_col_without_border=map_col-2;
    //row
    if(input % (map_col_without_border) ==0){
        translate_row_col[0]= (input/(map_col_without_border));    
    }else{
        translate_row_col[0]=(input/(map_col_without_border))+1;
    }
    //col
    translate_row_col[1]=input-((map_col_without_border)*(translate_row_col[0]-1));
}

int transte_from_row_col(int row,int col,int map_col){
    return (map_col-2)*(row-1)+col;
}


void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row)*(map_col)][(map_row)*(map_col)],int n,int prev[]) {
    int i, k, mini;
    int visited[(map_row)*(map_col)];
 
    for (i = 1; i <= n; ++i) {
        d[i] = INFINITY;
        prev[i] = -1; /* no path has yet been found to i */
        visited[i] = 0; /* the i-th element has not yet been visited */
    }
    d[s] = 0;
    
    for (k = 1; k <= n; ++k) {
        mini = -1;
        for (i = 1; i <= n; ++i)
            if (!visited[i] && ((mini == -1) || (d[i] < d[mini])))
                mini = i;
 
        visited[mini] = 1;
 
        for (i = 1; i <= n; ++i)
            if (dist[mini][i])
                if (d[mini] + dist[mini][i] < d[i]) {
                    d[i] = d[mini] + dist[mini][i];
                    prev[i] = mini;
                }           
    }
}
int checkWall(int row,int col,int map_col,char map[][map_col+1]){
    if(map[row][col]=='s'|| map[row][col]=='S' ||
        map[row][col]=='f' || map[row][col]=='F' ||
        map[row][col]==' ' || map[row][col]=='g'|| map[row][col]=='G' ||map[row][col]=='p'|| map[row][col]=='P'){
        return 1;
    }
    return 0;
}

void initialize_dist_array(int row,int col,int map_row,int map_col,long dist [][(map_row)*(map_col)],char map[][map_col+1]) {
    // col *(r-1)+c
    for(int r=1;r<=row;r++){
        for(int c=1;c<=col+1;c++){
                dist[col *(r-1)+c][col *(r-1)+(c+1)]=checkWall(r,c+1,map_col,map);
                dist[col *(r-1)+c][col *(r-1)+(c-1)]=checkWall(r,c-1,map_col,map);
                if(r-2>=0){
                dist[col *(r-1)+c][col *(r-2)+c]=checkWall(r-1,c,map_col,map);
                }
                dist[col *(r-1)+c][col *(r)+c]=checkWall(r+1,c,map_col,map);
        }

    }
}
void printPath(int ghost_num,int dest,int prev[],int ghost_path_size,int ghost_path[][ghost_path_size],struct ghost_char *my_ghost_char) {
    if (prev[dest] != -1)
        printPath(ghost_num,prev[dest],prev,ghost_path_size,ghost_path,my_ghost_char);
    ghost_path[ghost_num][(my_ghost_char[ghost_num].current_path)++]=dest;
}
char temp=' ';
char temp2=' ';
void ghost_move(int ghost_path_size,int ghost_path[][ghost_path_size],int translate_row_col[],int map_row,
    int map_col,char map[][map_col+1],struct ghost_char *my_ghost_char,WINDOW *game_window){
    
    // for (int i = 0; i < 4; ++i)
    // {
    //     translate_from_1_number(ghost_path[i][(my_ghost_char[i].current_path)++],translate_row_col,map_row,map_col);
    //     if(translate_row_col[0]!=0 && translate_row_col[1]!=0 ){
    //         if(temp!='b'){
    //             mvwaddch(game_window,my_ghost_char[i].ghost_row,my_ghost_char[i].ghost_col,convert_to_map_character(temp));                
    //         }
            
    //         if(map[translate_row_col[0]][translate_row_col[1]]!='P'){
    //             temp=map[translate_row_col[0]][translate_row_col[1]];
    //         }else{
    //             temp=' ';
    //         }
    //         map[translate_row_col[0]][translate_row_col[1]]='g';
    //         map[my_ghost_char[i].ghost_row][my_ghost_char[i].ghost_col]=temp;
            
    //         my_ghost_char[i].ghost_row=translate_row_col[0];
    //         my_ghost_char[i].ghost_col=translate_row_col[1];  
            
    //         wattron(game_window,COLOR_PAIR(3));
    //         mvwaddch(game_window,my_ghost_char[i].ghost_row,my_ghost_char[i].ghost_col,ACS_CKBOARD);
    //         wattron(game_window,COLOR_PAIR(1));  
    //     }  
    // }
        
        translate_from_1_number(ghost_path[0][(my_ghost_char[0].current_path)++],translate_row_col,map_row,map_col);
        wattron(game_window,COLOR_PAIR(5));
        mvwaddch(game_window,my_ghost_char[0].ghost_row,my_ghost_char[0].ghost_col,convert_to_map_character(temp));      
        wattron(game_window,COLOR_PAIR(3));
        mvwaddch(game_window,translate_row_col[0],translate_row_col[1],ACS_CKBOARD);
        wattron(game_window,COLOR_PAIR(1));
        if(map[translate_row_col[0]][translate_row_col[1]]!='G'){
            if(map[translate_row_col[0]][translate_row_col[1]]!='P'){
                temp2=map[translate_row_col[0]][translate_row_col[1]];
            }else{
                temp2=' ';
            }
            // temp2=map[translate_row_col[0]][translate_row_col[1]];
            map[translate_row_col[0]][translate_row_col[1]]='G';
            map[my_ghost_char[0].ghost_row][my_ghost_char[0].ghost_col]=temp;
            temp=temp2;
            
            my_ghost_char[0].ghost_row=translate_row_col[0];
            my_ghost_char[0].ghost_col=translate_row_col[1];        
        }      
}
chtype convert_to_map_character(char text_character){
    switch(text_character){
           // case 'G':
           //  case 'g':
           //      return ACS_CKBOARD;
           //      break;
            case 's':
                return ACS_BULLET;
                break;
            case 'S':
                return ACS_DEGREE;
                break;  
            case 'P':
            case 'p':
                 return ACS_DIAMOND;                  
                break;
            case 'f':
            case 'F':
                return ACS_STERLING;
                break; 
            case ' ':
                return ' ';
                break;
    }
    return ' ';
}

