#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "instructions.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
        return -1;
    }

    s_Filebytes *file_bytes = read_file_bytes(argv[1]);

    if (file_bytes->bytes[1] != '\x4c') {
        fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
        free(file_bytes);
        return -1;
    }

    print_full_hexcode(file_bytes);

    s_Func_Prototype **prototypes = parse_bytecode(file_bytes);

    match_instructions(*prototypes);

    for (int i = 0; i < (*prototypes)->sizelocvars; ++i)
        free((*prototypes)->locvars[i].varname);

    for (int i = 0; i < (*prototypes)->sizeupvalues; ++i)
        free((*prototypes)->upvalues[i].name);

    free((*prototypes)->source);
    free((*prototypes)->upvalues);
    free((*prototypes)->locvars);
    free((*prototypes)->abslineinfo);
    free((*prototypes)->lineinfo);
    free((*prototypes)->code);

    free(*prototypes);
    free(prototypes);
    free(file_bytes);

    return 0;
}
