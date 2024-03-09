CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC = clang

SOURCE_FILES = main.c tasks.c queue.c scheduler.c globals.c workers.c dispatcher.c

.PHONY: clean

all: main

main: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f main
