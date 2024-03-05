CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC = clang

SOURCE_FILES = main.c tasks.c queue.c scheduler.c globals.c workers.c dispatcher.c

.PHONY: clean

all: main test

main: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $^ -o $@

test: test.c tasks.c queue.c scheduler.c globals.c
	clang -Wall -Wextra -Wpedantic -Werror -g test.c tasks.c queue.c scheduler.c globals.c -o test

clean:
	rm -f main test
