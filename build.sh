#!/bin/sh

set -xe

CC="clang"
CFLAGS="-std=c99 -pedantic -Werror -Wall -Wextra -O0 -ggdb3 -fsanitize=address,undefined"
IFLAGS="-Isrc"
SRC="./src/vm.c ./src/util.c ./src/main.c"
OUTPUT="./lbi"
# INPUT="./fibonacci/luac.out"
INPUT="./lua/luac.out"

$CC $IFLAGS $CFLAGS -o $OUTPUT $SRC

if [ "$1" = "debug" ]; then
    gdb -tui --args "$OUTPUT" "$INPUT"
else
    $OUTPUT $INPUT
fi
