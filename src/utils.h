#ifndef UTILS_H__
#define UTILS_H__

#include "file.h"

void print_full_hex(FILE_BYTES **file_bytes);
uint8_t poke_next_byte(FILE_BYTES *file_bytes);
void skip_bytes(FILE_BYTES *file_bytes, size_t n);
void print_binary(unsigned long value, int width);
uint8_t *poke_bytes(FILE_BYTES *file_bytes, uint8_t amount);

#endif      /*   UTILS_H__   */
