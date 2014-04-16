CC=clang
CFLAGS=-Wall 
SOURCES = src/pacman.c src/write_file.c src/read_file.c src/command.c src/utility.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = src/write_file.h src/read_file.h src/command.h src/constant.h src/utility.h

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
