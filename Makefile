CC = gcc
CFLAGS = -Wall -Wextra -ggdb -fsanitize=address
OUTFILE = luac.out

all: compile

compile:
	$(CC) $(CFLAGS) main.c

debug:
	gdb -out a.out
