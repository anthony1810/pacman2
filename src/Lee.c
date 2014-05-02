#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "Lee.h"

  
    // int path[N_ROWS * N_COLS];
    int count = 0;
    // int temp[N_ROWS][N_COLS];
    // char a[N_ROWS][N_COLS];

  // Expand Algorithm from one point with cost, starting x (cols), starting y (rows)  
    void expand(int cost, int x, int y, char a[][N_COLS+1]){
        //Down
        if (y + 1 < N_ROWS){
            if (a[y+1][x] == 'G') return;
            if(temp[y+1][x] > cost+1 /*&& (a[y+1][x] == '.')*/){
                temp[y+1][x] = cost+1;

                expand(cost+1, x, y+1, a);
            }
        }

//        UP
        if (y - 1 >= 0){
            if (a[y-1][x] == 'G') return;
            if(temp[y-1][x] > cost+1/* && a[y-1][x] == '.'*/){
                temp[y-1][x] = cost+1;

                expand(cost+1, x, y-1, a);
            }
        }
        // Right
        if (x + 1 < N_COLS){
            if (a[y][x+1] == 'G') return;
            if(temp[y][x+1] > cost+1){
                temp[y][x+1] = cost+1;

                expand(cost+1, x+1, y, a);
            }
        }
        //Left
        if (x - 1 >= 0){
            if (a[y][x-1] == 'G') return;
            if(temp[y][x-1] > cost+1){
                temp[y][x-1] = cost+1;

                expand(cost+1, x-1, y, a);
            }
        }
    }

    //Trace back from destination to find shortest path
    void traceBack(int x, int y, char a[][N_COLS+1]){

        if (x+1 < N_COLS && temp[y][x+1] == 0 && a[y][x+1] != 'P') temp[y][x+1] = 1000;
        if (x-1 >=0 && temp[y][x-1] == 0 && a[y][x-1] != 'P') temp[y][x-1] = 1000;
        if (y+1 < N_ROWS && temp[y+1][x] == 0 && a[y+1][x] != 'P') temp[y+1][x] = 1000;
        if (y-1 >=0 && temp[y-1][x] == 0 && a[y-1][x] != 'P') temp[y-1][x] = 1000;

        if (a[y][x] == 'P') return;
        if (x + 1 < N_COLS && a[y][x+1] != 'G' && checkCollision(x+1, y,a) && temp[y][x+1] <= temp[y][x-1] && temp[y][x+1] <= temp[y+1][x] && temp[y][x+1] <= temp[y-1][x] ){       //Right
            path[count] = y *N_COLS + x + 1;
            count++;
            traceBack(x+1,y, a);
        } else if (x - 1 >= 0  && a[y][x-1] != 'G' && checkCollision(x-1, y,a)  && temp[y][x-1] <= temp[y][x+1] && temp[y][x-1] <= temp[y+1][x] && temp[y][x-1] <= temp[y-1][x] )  {  //Left
            path[count] = y * N_COLS + x - 1;
            count++;
            traceBack(x-1,y, a);

        } else if (y + 1 < N_ROWS && a[y+1][x] != 'G'&& checkCollision(x, y+1,a) && temp[y+1][x] <= temp[y][x+1] && temp[y+1][x] <= temp[y][x-1] && temp[y+1][x] <= temp[y-1][x] ){    //Down
            path[count] = (y+1) * N_COLS + x;
            count++;
            traceBack(x,y+1,a);
        }  else if (y - 1 >= 0 && a[y-1][x] != 'G'&& checkCollision(x, y-1,a) && temp[y-1][x] <= temp[y][x-1] && temp[y-1][x] <= temp[y][x+1] && temp[y-1][x] <= temp[y+1][x] ) {    //Up
            path[count] = (y-1) * N_COLS + x;
            count++;
            traceBack(x,y-1, a);
        }
    }

    // Check if ghost or pacman can move through one pointdist
    int checkCollision(int x, int y, char a[][N_COLS+1]){
        if (a[y][x] != 'W' || a[y][x] != 'w' || a[y][x] != 'q' || a[y][x] != 'Q' || a[y][x] != 'e' || a[y][x] != 'E' || a[y][x] != 'a' || a[y][x] != 'A' 
            || a[y][x] != 'd' || a[y][x] != 'D' || a[y][x] != 'z' || a[y][x] != 'Z' || a[y][x] != 'x' || a[y][x] != 'X' || a[y][x] != 'c' || a[y][x] != 'C')
            return 1;       // No collision
        else return 0;          
    }

    void lee(int start_x, int start_y, int end_x, int end_y, char a[][N_COLS+1],WINDOW *game_window){
        count = 0;

        for (int i = 0; i < N_ROWS * N_COLS; i++){
            path[i] = 0;
        }
        
        wclear(&game_window);
        wprintw(&game_window,"%s","asddddddddd");
        wrefresh(&game_window);
        // for (int i = 0; i < N_ROWS; i++){
        //     for (int j = 0; j < N_COLS; j++){
        //         if (a[i][j] == '.' || a[i][j] == 'f' || a[i][j] == 'F' || a[i][j] == 'G' || a[i][j] == ' '){
        //             temp[i][j] = 9999;
        //         } else temp [i][j] = 0;
        //     }
        // }
        // expand(0, start_x, start_y, a);
        // traceBack(end_x, end_y, a);
    }
