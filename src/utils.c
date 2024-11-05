#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "file.h"
#include "utils.h"

uint8_t *poke_bytes(s_Filebytes *file_bytes, const size_t amount) {
    assert(amount > 0);

    // Check that we have enough bytes left to read
    if (file_bytes->cursor + amount > file_bytes->length) {
        fprintf(stderr, "error: not enough bytes to read\n");
        return NULL;
    }

    uint8_t *bytes = calloc(amount, sizeof(uint8_t));
    if (bytes == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        return NULL;
    }

    memcpy(bytes, file_bytes->bytes + file_bytes->cursor, amount);
    file_bytes->cursor += amount;
    return bytes;
}

uint8_t poke_next_byte(s_Filebytes *file_bytes) {
    uint8_t out = (*file_bytes).bytes[file_bytes->cursor];
    file_bytes->cursor++;
    return out;
}

void skip_bytes(s_Filebytes *file_bytes, const size_t n) {
    (*file_bytes).cursor += n;
}

void print_binary(const unsigned long value, const int width) {
    for (int i = 0; i < width; ++i)
        putchar((value & (1UL << (width - i - 1))) ? '1' : '0');
    printf("\n");
}

void print_full_hex(s_Filebytes *const *file_bytes) {
    for (size_t i = 0; i < (*file_bytes)->length; ++i) {
        if (i % 2 == 0 && i != 0)
            printf(" ");
        if (i % 16 == 0 && i != 0)
            printf("\n");
        printf(RED "%.2x" RESET, (*file_bytes)->bytes[i]);
    }
}
