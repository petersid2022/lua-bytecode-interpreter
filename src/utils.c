#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "utils.h"

void *safe_malloc(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate %zu bytes.\n", n);
        abort();
    }

    return p;
}

void skip_bytes(s_Filebytes *file_bytes, const size_t n) {
    file_bytes->cursor += n;
}

uint8_t poke_next_byte(s_Filebytes *file_bytes) {
    uint8_t out = file_bytes->bytes[file_bytes->cursor];
    file_bytes->cursor++;
    return out;
}

uint8_t *poke_bytes(s_Filebytes *file_bytes, const int amount) {
    assert(amount > 0);

    // Check for out-of-border access
    if ((file_bytes->cursor + amount) > file_bytes->length) {
        fprintf(stderr, "ERROR: Trying to access out-of-border bytes.\n");
        return NULL;
    }

    uint8_t *bytes = safe_malloc(amount * sizeof(uint8_t));

    memcpy(bytes, file_bytes->bytes + file_bytes->cursor, amount);

    file_bytes->cursor += amount;

    return bytes;
}

void print_binary(const unsigned long value, const int width) {
    for (int i = 0; i < width; ++i)
        putchar((value & (1UL << (width - i - 1))) ? '1' : '0');
    printf("\n");
}

void print_full_hexcode(const s_Filebytes *file_bytes) {
    printf("\nHEXDUMP:\n");
    for (int i = 0; i <= file_bytes->length; i += 16) {
        printf("%.8x  ", i);
        for (int j = 0; j < 16; ++j) {
            if (i + j < file_bytes->length) {
                printf("%.2x ", file_bytes->bytes[i + j]);
            } else {
                printf(" ");
            }

            if ((j + 1) % 8 == 0)
                printf(" ");
        }
        printf("\n");
    }
}
