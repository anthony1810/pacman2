#include <stdlib.h>
#include "struct.h"

struct ghost_char *create_ghost_char(){
    struct ghost_char *my_ghost_char= malloc(4*sizeof(struct ghost_char));
    return my_ghost_char;
}

struct pacman_char *create_pacman_char(){
    struct pacman_char *my_pacman_char= malloc(sizeof(struct pacman_char));
    return my_pacman_char;
}
struct map *create_map(){
    struct map *my_map=malloc(sizeof(struct map));
    return my_map;
}
struct map_status *create_map_status(){
	struct map_status *my_map_status=malloc(sizeof(struct map_status));
	return my_map_status;
}