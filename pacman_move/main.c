#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "new_game_read_file.h"
#include "pacman_character.h"
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define PELLET_SCORE 100
#define SUPER_PELLET_SCORE 200
#define FRUIT_SCORE 300
#define DELAY 200

#define MAP_ROW 35
#define MAP_COL 65

#define INFINITY 999
 
int e; /* The number of nonzero edges in the graph */
int n; /* The number of nodes in the graph */
long dist[(MAP_ROW+2)*(MAP_COL+2)][(MAP_ROW+2)*(MAP_COL+2)]; /* dist[i][j] is the distance between node i and j; or 0 if there is no direct connection */
long d[(MAP_ROW+2)*(MAP_COL+2)]; /* d[i] is the length of the shortest path between the source (s) and node i */
int prev [(MAP_ROW+2)*(MAP_COL+2)];
int path [(MAP_ROW+2)*(MAP_COL+2)];

WINDOW game_window;
WINDOW debug_window;
char map[MAP_ROW][MAP_COL+1];
char s[100];
int ghost_row;
int ghost_col;
int translate_row_col[2];
int current_num=0;
WINDOW create_new_win(int height, int width, int starty, int startx)
{   
    WINDOW local_win;
    local_win = *newwin(height, width, starty, startx);
    return local_win;

}
long long
timeval_diff(struct timeval *difference,
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
void printD() {
    int i;
    for (i = 1; i <= n; ++i){
        printf("%10d", i);
        wrefresh(&game_window);
    }
    printf("\n");
    for (i = 1; i <= n; ++i) {
        printf("%10ld", d[i]);
        wrefresh(&game_window);
    }
    printf("\n");

}
void printPath(int dest) {
    if (prev[dest] != -1)
        printPath(prev[dest]);
    path[current_num++]=dest;
}

void dijkstra(int s) {
    int i, k, mini;
    int visited[(MAP_ROW+2)*(MAP_COL+2)];
 
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
int checkWall(int row,int col){
    if(map[row][col]=='s'|| map[row][col]=='S' ||
        map[row][col]=='f' || map[row][col]=='F' ||
        map[row][col]==' ' || map[row][col]=='g'|| map[row][col]=='G' ||map[row][col]=='p'|| map[row][col]=='P'){
        return 1;
    }
    return 999;
}
void initialize_dist_array(int row,int col){
    // col *(r-1)+c
    for(int r=1;r<=row;r++){
        for(int c=1;c<=col+1;c++){
            // if(checkWall(r,c+1)==1){
                dist[col *(r-1)+c][col *(r-1)+(c+1)]=checkWall(r,c+1);
                // if(col *(r-1)+(c+1)==64){
                // printf("%d \n", col *(r-1)+(c+1));
                // wrefresh(&game_window);
                // return;}
            // }
            // if(checkWall(r,c-1)==1){
                dist[col *(r-1)+c][col *(r-1)+(c-1)]=checkWall(r,c-1);
            // }
            // if(checkWall(r-1,c)==1){
                if(r-2>=0){
                dist[col *(r-1)+c][col *(r-2)+c]=checkWall(r-1,c);
                }
            // }
            // if(checkWall(r+1,c)==1){
                dist[col *(r-1)+c][col *(r)+c]=checkWall(r+1,c);
            // }
        }

    }
}
void translate_from_1_number(int input){
    //row
    if(input % (MAP_COL-2) ==0){
        translate_row_col[0]= (input/(MAP_COL-2));    
    }else{
        translate_row_col[0]=(input/(MAP_COL-2))+1;
    }
    //col
    translate_row_col[1]=input-((MAP_COL-2)*(translate_row_col[0]-1));
}
int transte_from_row_col(int row,int col){
    return (MAP_COL-2)*(row-1)+col;
}
int main(){
    char ch=0;
    char k=0;
    initscr();  
    noecho();   
    timeout(DELAY);
    curs_set(0);
    game_window =create_new_win(MAP_ROW,MAP_COL+1,0,0);
    debug_window=create_new_win(30,30,0,70);

    struct pacman_char *my_pacman_char=create_pacman_char();
    my_pacman_char->score=0;


    // new_game_read_file(&game_window,12,16,map,s,"level1",&pac_row,&pac_col);
    new_game_read_file(&game_window,MAP_ROW,MAP_COL+1,map,s,"level2",my_pacman_char,&ghost_row,&ghost_col);
    wclear(&game_window);
    wrefresh(&game_window);;
    
    // n=12*15;
    // initialize_dist_array(11,14);

    n=MAP_ROW*MAP_COL;
    initialize_dist_array(MAP_ROW-2,MAP_COL-2);

    // dijkstra(transte_from_row_col(ghost_row,ghost_col)); 
    // printPath(transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col));
    //2079 the last pos of 35x65map


    dijkstra(transte_from_row_col(ghost_row,ghost_col)); 
    printPath(transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col));

 
    current_num=0;
    struct timeval earlier;
    struct timeval later;
    gettimeofday(&earlier,NULL);
    while((ch = getch()) != 'q'){ 


        //w
         if(ch==119){
            my_pacman_char->current_direction=UP;
        }
        //s
        if(ch==115){
            my_pacman_char->current_direction=DOWN;
        }
        //d
        if(ch==100){
            my_pacman_char->current_direction=RIGHT;
        }
        //a
        if(ch==97){
            my_pacman_char->current_direction=LEFT;
        }
        gettimeofday(&later,NULL);

        if(timeval_diff(NULL,&later,&earlier)>=DELAY){
            gettimeofday(&earlier,NULL);
            pacman_char_move(my_pacman_char,map);
            new_game_update_map(&game_window,36,66,map);
            wclear(&debug_window);
            wprintw(&debug_window,"%d",my_pacman_char->score);
            wrefresh(&debug_window);
            if(current_num==4){
                current_num=0;
                dijkstra(transte_from_row_col(ghost_row,ghost_col)); 
                printPath(transte_from_row_col(my_pacman_char->pac_row,my_pacman_char->pac_col));
                current_num=0;
                //to prevent the ghost flashing when calculating new path
                translate_from_1_number(path[current_num++]);
                map[translate_row_col[0]][translate_row_col[1]]='g';
                map[ghost_row][ghost_col]=' ';
                ghost_row=translate_row_col[0];
                ghost_col=translate_row_col[1];
                new_game_update_map(&game_window,MAP_ROW+1,MAP_COL+1,map);

            }
            if(path[current_num]!=0){
                 
                translate_from_1_number(path[current_num++]);
                map[translate_row_col[0]][translate_row_col[1]]='g';
                map[ghost_row][ghost_col]=' ';
                ghost_row=translate_row_col[0];
                ghost_col=translate_row_col[1];
                new_game_update_map(&game_window,MAP_ROW+1,MAP_COL+1,map);
            }
        }
 

        
    }
   
    endwin();
    return 0;
}