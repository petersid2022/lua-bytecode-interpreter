#!/bin/sh

set -xe

LUAC_PATH="./lua/lua-5.4.7/src/luac"

build_luac() {
  mkdir -p ./lua/lua-5.4.7
  tar -xvzf ./lua/lua-5.4.7.tar.gz  -C ./lua
  sed -i 's/^CFLAGS=.*/CFLAGS= -ggdb3 -O0 -Wall -Wextra -DLUA_COMPAT_5_3 $(SYSCFLAGS) $(MYCFLAGS)/' ./lua/lua-5.4.7/src/Makefile
  make -C ./lua/lua-5.4.7 -j$(nproc) -B
}

if [ ! -f "$LUAC_PATH" ]; then
  echo "$LUAC_PATH does not exist. Building..."
  build_luac
fi

gdb -tui "$LUAC_PATH"

rm luac.out
