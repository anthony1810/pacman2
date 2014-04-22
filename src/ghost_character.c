#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "ghost_character.h"
#define INFINITY 999
struct ghost_char *create_ghost_char(){
    struct ghost_char *my_ghost_char= malloc(sizeof(struct ghost_char));
    return my_ghost_char;
}

void translate_from_1_number(int input,int translate_row_col[],int map_row,int map_col){
    //row
    if(input % (map_col-2) ==0){
        translate_row_col[0]= (input/(map_col-2));    
    }else{
        translate_row_col[0]=(input/(map_col-2))+1;
    }
    //col
    translate_row_col[1]=input-((map_col-2)*(translate_row_col[0]-1));
}

int transte_from_row_col(int row,int col,int map_col){
    return (map_col-2)*(row-1)+col;
}


void dijkstra(int s,int map_row,int map_col,long d[],long dist[(map_row+2)*(map_col+2)][(map_row+2)*(map_col+2)],int n,int prev[]) {
    int i, k, mini;
    int visited[(map_row+2)*(map_col+2)];
 
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

void initialize_dist_array(int row,int col,int map_row,int map_col,long dist [][(map_row+2)*(map_col+2)],char map[][map_col+1]) {
    // col *(r-1)+c
    for(int r=1;r<=row;r++){
        for(int c=1;c<=col+1;c++){
            // if(checkWall(r,c+1)==1){
                dist[col *(r-1)+c][col *(r-1)+(c+1)]=checkWall(r,c+1,map_col,map);
                // if(col *(r-1)+(c+1)==64){
                // printf("%d \n", col *(r-1)+(c+1));
                // wrefresh(&game_window);
                // return;}
            // }
            // if(checkWall(r,c-1)==1){
                dist[col *(r-1)+c][col *(r-1)+(c-1)]=checkWall(r,c-1,map_col,map);
            // }
            // if(checkWall(r-1,c)==1){
                if(r-2>=0){
                dist[col *(r-1)+c][col *(r-2)+c]=checkWall(r-1,c,map_col,map);
                }
            // }
            // if(checkWall(r+1,c)==1){
                dist[col *(r-1)+c][col *(r)+c]=checkWall(r+1,c,map_col,map);
            // }
        }

    }
}
void printPath(int dest,int prev[],int ghost_path[],int *current_path) {
    if (prev[dest] != -1)
        printPath(prev[dest],prev,ghost_path,current_path);
    ghost_path[(*current_path)++]=dest;
}

void ghost_move(int ghost_path[],int *current_num,int translate_row_col[],int map_row,int map_col,char map[][map_col+1],struct ghost_char *my_ghost_char){
    translate_from_1_number(ghost_path[(*current_num)++],translate_row_col,map_row,map_col);
    char temp;
    if(map[translate_row_col[0]][translate_row_col[1]]!='P'){
        temp=map[translate_row_col[0]][translate_row_col[1]];
    }else{
        temp=' ';
    }
    map[translate_row_col[0]][translate_row_col[1]]='g';
    map[my_ghost_char->ghost_row][my_ghost_char->ghost_col]=temp;
    my_ghost_char->ghost_row=translate_row_col[0];
    my_ghost_char->ghost_col=translate_row_col[1];
}

