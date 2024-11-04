#ifndef FILE_H__
#define FILE_H__

#include <stddef.h>
#include <stdint.h>

typedef struct {
	size_t length;
	size_t cursor;
	uint8_t *bytes;
} FILE_BYTES;

FILE_BYTES **read_file_bytes(const char *file_name);

#endif      /*   FILE_H__   */
