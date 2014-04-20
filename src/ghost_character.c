#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "ghost_character.h"
#define MAP_ROW 35
#define MAP_COL 65
#define INFINITY 999
struct ghost_char *create_ghost_char(){
    struct ghost_char *my_ghost_char= malloc(sizeof(struct ghost_char));
    return my_ghost_char;
}

void translate_from_1_number(int input,int translate_row_col[]){
    //row
    if(input % (MAP_COL-2) ==0){
        translate_row_col[0]= (input/(MAP_COL-2));    
    }else{
        translate_row_col[0]=(input/(MAP_COL-2))+1;
    }
    //col
    translate_row_col[1]=input-((MAP_COL-2)*(translate_row_col[0]-1));
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
