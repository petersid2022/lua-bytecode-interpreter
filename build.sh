#!/bin/sh

set -xe

CC="clang"
CFLAGS="-std=c99 -pedantic -Werror -Wall -Wextra -O0 -ggdb3 -fsanitize=address,undefined"
IFLAGS="-Isrc"
SRC="./src/instructions.c ./src/utils.c ./src/main.c"
OUTPUT="./lbi"

$CC $IFLAGS $CFLAGS -o $OUTPUT $SRC

if [ "$1" = "debug" ]; then
    gdb -tui --args "$OUTPUT" fibonacci/luac.out
else
    $OUTPUT fibonacci/luac.out
fi
