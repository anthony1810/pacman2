CC=clang
CFLAGS=-Wall 
SOURCES = src/pacman.c src/write_file.c src/read_file.c src/command.c src/utility.c src/new_game_read_file.c src/pacman_character.c src/ghost_character.c src/struct.c src/hunter_ghost.c src/high_score.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = src/write_file.h src/read_file.h src/command.h src/constant.h src/utility.h src/new_game_read_file.h src/pacman_character.h src/ghost_character.h src/struct.h src/hunter_ghost.h src/high_score.h

pacman: $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o src/pacman $(OBJS) -lncurses
	
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm src/*.o 
	
.PHONY: cleanall
cleanall:
	rm src/*.o src/pacman
