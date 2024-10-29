CC = gcc
CFLAGS = -Wall -Wextra -O0 -ggdb3 -fsanitize=address
OUTFILE = luac.out

all: compile

compile:
	$(CC) $(CFLAGS) main.c

debug:
	gdb -tui a.out

luac:
	gdb -tui lua-5.4.7/src/luac
