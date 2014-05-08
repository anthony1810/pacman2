#ifndef STRUCT_H
#define STRUCT_H
struct pacman_char
{
	int live;
	int pac_row;
	int pac_col;
	int initial_pac_row;
	int initial_pac_col;
	int current_direction;
	int score;
	int pac_state;
	int invulrable_duration;
};
struct map{
	double total_pellet;
	double remain_pellet;
};
struct ghost_char
{
	int ghost_row;
	int ghost_col;
	int initial_ghost_row;
	int initial_ghost_col;
	double speed_multiplier;
	int current_path;
	int ghost_num;
};
struct map_status{
	int invulrable_duration;
	int score;
	int live;
};

struct ghost_char *create_ghost_char();
struct pacman_char *create_pacman_char();
struct map *create_map();
struct map_status *create_map_status();
#endif