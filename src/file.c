#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "utils.h"

s_Filebytes *read_file_bytes(const char *file_name) {
    assert(strlen(file_name) != 0);

    FILE    *fp;
    uint8_t *buffer;
    long int length;

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: fopen() failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);

    length = ftell(fp);
    if (length == 0) {
        fprintf(stderr, "ERROR: ftell() failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_SET);

    buffer = safe_malloc((size_t) length * sizeof(uint8_t));

    fread(buffer, sizeof(uint8_t), (size_t) length, fp);

    s_Filebytes *file_bytes = safe_malloc(sizeof(s_Filebytes) + length * sizeof(uint8_t));

    memcpy(file_bytes->bytes, buffer, (size_t) length);
    file_bytes->length = length;
    file_bytes->cursor = 0;

    free(buffer);
    fclose(fp);

    return file_bytes;
}
