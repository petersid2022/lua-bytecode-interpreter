# Lua 5.4 Bytecode Interpreter written in C.

To better understand how the `luac` compiler works, I found it invaluable to run and step through the compiler using a provided `main.lua` script (also included in this repository) inside `gdb`. Thus, I've included a slightly modified version of `lua-5.4.7` compiled with the `-ggdb` flag for debugging, with the default optimizations `-O2` disabled, which you can build using `make`.

* Online Lua Bytecode Explorer: https://www.luac.nl/
* `luac` compiles Lua source code to Lua bytecode: https://www.lua.org/source/5.4/luac.c.html
* All of the Lua virtual machine opcodes: https://www.lua.org/source/5.4/lopcodes.h.html
* How luac saves precompiled Lua chunks: https://www.lua.org/source/5.4/ldump.c.html
