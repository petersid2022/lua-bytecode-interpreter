#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "instructions.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "error: You need to specify a luac file\n");
        return -1;
    }

    s_Filebytes      **file_bytes;
    s_Func_Prototype **prototypes;
    s_Instruction    **instructions;

    file_bytes = read_file_bytes(argv[1]);

    // Bad way of checking if the file the user provided is a valid Lua bytecode
    // binary but it is what it is. Basically, the first field of the header is
    // the LUA_SIGNATURE i.e. "\x1bLua" So, I just check if the second byte of the
    // file is equal to 'L'
    assert((*file_bytes)->bytes[1] == '\x4c');

    print_full_hex(file_bytes);

    parse_header(*file_bytes);

    prototypes = parse_function(*file_bytes);

    instructions = decode_instructions(*prototypes);

    // for (size_t i = 0; i < (*prototypes)->sizecode; ++i) {
    //     print_binary(instructions[i]->value, 32);
    // }

    for (int i = 0; i < (*prototypes)->sizecode; ++i)
        free(instructions[i]);

    free(instructions);

    free((*file_bytes)->bytes);
    free(*file_bytes);
    free(file_bytes);

    free((*prototypes)->code);
    free((*prototypes)->upvalues);
    free(*prototypes);
    free(prototypes);

    return 0;
}
