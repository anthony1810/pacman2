#ifndef HUNTER_GHOST_H
#define HUNTER_GHOST_H
enum Hunter_Directions{
	Up,
	Down,
	Left,
	Right
};

enum Hunter_Directions get_hunter_direction();

int is_wall_ahead(WINDOW *game_window, int x_pos, int y_pos);

void start_hunter(WINDOW *game_window, struct ghost_char *my_ghost_char, struct pacman_char *my_pacman_char);
#endif