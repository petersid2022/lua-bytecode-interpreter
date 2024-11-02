#!/bin/sh

set -xe

CC="gcc"
CFLAGS="-std=c99 -pedantic -Wall -Wextra -O0 -ggdb3 -fsanitize=address"
IFLAGS="-Isrc"
SRC="./src/main.c"

$CC $IFLAGS $CFLAGS $SRC

./a.out lua/luac.out

rm ./a.out
