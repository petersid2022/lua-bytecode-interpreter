#ifndef UTILS_H__
#define UTILS_H__

#include "file.h"

/* Simple malloc wrapper that returns a void pointer and if an error occurs it
 * aborts execution */
void *safe_malloc(size_t n);

/* Simple printing utilities for printing out the hexbytes of the binary and for
 * properly printing out the binary representation of a number. */
void print_hexdump(const s_Filebytes *file_bytes);
void print_binary(const unsigned long value, const int width);

/* Moves the cursor n amount, relative to it's current position. */
uint8_t *poke_bytes(s_Filebytes *file_bytes, const int amount);

/* Returns next byte, relative to it's current position. */
uint8_t poke_next_byte(s_Filebytes *file_bytes);

/* Returns n amount of bytes, relative to it's current position. */
void skip_bytes(s_Filebytes *file_bytes, const size_t n);

#endif // UTILS_H__
