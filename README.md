# Lua 5.4 Bytecode Interpreter written in C

To better understand how the `luac` compiler works, I found it invaluable to run and step through the compiler using a provided `main.lua` script (also included in this repository) inside `gdb`. Thus, I've included a slightly modified version of `lua-5.4.7` compiled with the `-ggdb` flag for debugging, with the default optimizations `-O2` disabled, which you can build using `make`. See: [debug\_luac.sh](./debug\_luac.sh)

## Resources
* Online Lua Bytecode Explorer: https://www.luac.nl/
* `luac` compiles Lua source code to Lua bytecode: https://www.lua.org/source/5.4/luac.c.html
* All of the Lua virtual machine opcodes: https://www.lua.org/source/5.4/lopcodes.h.html
* How luac saves precompiled Lua chunks: https://www.lua.org/source/5.4/ldump.c.html
* The Implementation of Lua 5.0: https://www.lua.org/doc/jucs05.pdf
* Garbage Collection in Lua: https://www.lua.org/wshop18/Ierusalimschy.pdf
* https://en.m.wikibooks.org/wiki/Creating_a_Virtual_Machine/Register_VM_in_C
* https://en.m.wikipedia.org/wiki/Register_allocation

## License

This project is licensed under the MIT License.

(The MIT License) Copyright (c) 2025 Peter Sideris petersid2022@gmail.com Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
