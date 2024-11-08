#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "instructions.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: You need to specify a luac file.\n");
        return -1;
    }

    s_Filebytes       *file_bytes;
    s_Func_Prototype **prototypes;

    file_bytes = read_file_bytes(argv[1]);

    // The first field of the header is the LUA_SIGNATURE i.e. "\x1bLua"
    // I just check if the second byte of the file is equal to 'L'
    assert(file_bytes->bytes[1] == '\x4c');

    print_full_hex(file_bytes);

    parse_header(file_bytes);

    prototypes = parse_function(file_bytes);

    decode_instructions(*prototypes);

    free((*prototypes)->upvalues);
    free(*prototypes);
    free(prototypes);
    free(file_bytes);

    return 0;
}
