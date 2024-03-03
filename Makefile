CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC = clang

.PHONY: clean

all: main

main: main.c tasks.c queue.c
	clang -Wall -Wextra -Wpedantic -Werror -g main.c tasks.c queue.c -o main


clean:
	rm -f main
