#ifndef FILE_H__
#define FILE_H__

#include <stddef.h>
#include <stdint.h>

typedef struct {
    int     length;
    int     cursor;
    uint8_t bytes[];
} s_Filebytes;

/* Reads file bytes to memory */
s_Filebytes *read_file_bytes(const char *file_name);

#endif // FILE_H__
