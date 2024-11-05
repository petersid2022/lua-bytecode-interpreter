#ifndef UTILS_H__
#define UTILS_H__

#include "file.h"

void print_full_hex(s_Filebytes *const *file_bytes);
void print_binary(const unsigned long value, const int width);

uint8_t  poke_next_byte(s_Filebytes *file_bytes);
void     skip_bytes(s_Filebytes *file_bytes, const size_t n);
uint8_t *poke_bytes(s_Filebytes *file_bytes, const size_t amount);

#endif /*   UTILS_H__   */
