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

    s_Filebytes *file_bytes = NULL;
    s_Func_Prototype **prototypes = NULL;

    file_bytes = read_file_bytes(argv[1]);

    if (file_bytes->bytes[1] != '\x4c') {
        fprintf(stderr, "ERROR: You need to specify a valid luac file.\n");
        free(file_bytes);
        return -1;
    }

    print_hexdump(file_bytes);
    parse_hexdump(file_bytes, prototypes);

    free(file_bytes);

    return 0;
}
