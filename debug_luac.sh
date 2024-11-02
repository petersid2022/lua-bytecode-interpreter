#!/bin/sh

set -xe

LUAC_PATH="./lua/lua-5.4.7/src/luac"

build_luac() {
    make -C ./lua/lua-5.4.7 -j$(nproc) -B
}

if [ ! -f "$LUAC_PATH" ]; then
    echo "$LUAC_PATH does not exist. Building..."
    build_luac
fi

gdb -tui "$LUAC_PATH"

rm luac.out
