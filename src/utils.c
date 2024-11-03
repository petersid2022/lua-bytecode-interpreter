#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "file.h"
#include "colors.h"

uint8_t *poke_bytes(FILE_BYTES *file_bytes, uint8_t amount) {
	assert(amount > 0);

	uint8_t *bytes = calloc(amount, sizeof(uint8_t));
	if (bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	memcpy(bytes, file_bytes->bytes + file_bytes->cursor, amount);

	file_bytes->cursor += amount;

	return bytes;
}

uint8_t poke_next_byte(FILE_BYTES *file_bytes) {
	uint8_t out = (*file_bytes).bytes[file_bytes->cursor];
	file_bytes->cursor++;
	return out;
}

void skip_bytes(FILE_BYTES *file_bytes, size_t n) {
	(*file_bytes).cursor += n;
}

void print_binary(unsigned long value, int width) {
	for (int i = width - 1; i >= 0; --i) {
		putchar((value & (1UL << i)) ? '1' : '0');
	}
	putchar('\n');
}

void print_full_hex(FILE_BYTES **file_bytes) {
	for (size_t i = 0; i < (*file_bytes)->length; ++i) {
		if (i % 2 == 0 && i != 0) printf(" ");
		if (i % 16 == 0 && i != 0) printf("\n");
		printf(RED "%.2x" RESET, (*file_bytes)->bytes[i]);
	}
}
