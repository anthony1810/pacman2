#ifndef LEE_ALGORITHM
#define LEE_ALGORITHM
#define N_COLS 65
#define N_ROWS 35
int path[N_ROWS * N_COLS];
int temp[N_ROWS][N_COLS];
void expand(int cost, int pacman_x, int pacman_y, char a[][N_COLS+1]);
void traceBack(int x ,int y, char a[][N_COLS+1]);
void lee(int start_x, int start_y, int end_x, int end_y, char a[][N_COLS+1],WINDOW *game_window);
int checkCollision(int x, int y, char a[][N_COLS+1]);
#endif