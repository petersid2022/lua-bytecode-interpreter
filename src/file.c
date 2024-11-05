#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

s_Filebytes **read_file_bytes(const char *file_name) {
    assert(strlen(file_name) != 0);

    FILE    *fp;
    uint8_t *buffer;
    long int length;

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "error: fopen() failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);

    length = ftell(fp);
    if (length == 0) {
        fprintf(stderr, "error: ftell() failed\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_SET);

    buffer = calloc((size_t) length, sizeof(uint8_t));
    if (buffer == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, sizeof(uint8_t), (size_t) length, fp);

    s_Filebytes **file_bytes = calloc((size_t) length, sizeof(s_Filebytes *));
    if (file_bytes == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    *file_bytes = calloc((size_t) length, sizeof(s_Filebytes));
    if (*file_bytes == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    (*file_bytes)->bytes = calloc((size_t) length, sizeof(uint8_t));
    if ((*file_bytes)->bytes == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy((*file_bytes)->bytes, buffer, (size_t) length);
    (*file_bytes)->length = (size_t) length;
    (*file_bytes)->cursor = 0;

    free(buffer);
    fclose(fp);

    return file_bytes;
}
