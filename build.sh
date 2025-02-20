#!/bin/sh

set -xe

CC="clang"
CFLAGS="-std=c99 -pedantic -Wall -Wextra -O0 -ggdb3 -fsanitize=address"
IFLAGS="-Isrc"
SRC="./src/file.c ./src/instructions.c ./src/utils.c ./src/main.c"
OUTPUT="./lbi"

$CC $IFLAGS $CFLAGS -o $OUTPUT $SRC

$OUTPUT fibonacci/luac.out
