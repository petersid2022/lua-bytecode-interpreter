#!/bin/sh

set -xe

CC="clang"
LUAC="luac"
CFLAGS="-std=c99 -pedantic -Werror -Wall -Wextra -O0 -ggdb3 -fsanitize=address,undefined"
IFLAGS="-Isrc"
SRC="./src/vm.c ./src/util.c ./src/main.c"
BIN="./lbi"

INPUT="./lua/hello.lua"
OUTPUT="./lua/luac.out"

$CC $IFLAGS $CFLAGS -o "$BIN" $SRC

if [ ! -f "$OUTPUT" ] || [ "$INPUT" -nt "$OUTPUT" ]; then
    $LUAC -l "$INPUT"
    mv luac.out "$OUTPUT"
fi

if [ "$1" = "debug" ]; then
    gdb -tui --args "$BIN" "$OUTPUT"
else
    "$BIN" "$OUTPUT"
fi
