#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef enum {
	STRING,
	NUMBER,
	FUNCTION,
	BOOLEAN,
	NIL,
} TOKEN_TYPE;

typedef struct TOKEN {
	char *value;
	struct TOKEN *next;
	TOKEN_TYPE type;
} TOKEN;

typedef struct {
	size_t length;
	uint8_t *bytes;
} FILE_BYTES;

uint8_t *poke_4_bytes(FILE_BYTES *file_bytes) {
	uint8_t *bytes = calloc(4, sizeof(uint8_t));
	if (bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	memcpy(bytes, file_bytes->bytes, 4);

	return bytes;
}

void parse_header(FILE_BYTES *file_bytes) {
	size_t header_size = 12;

	printf("Header block: ");
	for (size_t i = 0; i < header_size; ++i) {
		printf("%.2x ", (*file_bytes).bytes[i]);
	}

	printf("\nHeader signature: ");

	uint8_t *header_signature = poke_4_bytes(file_bytes);

	size_t header_signature_len = 4;
	for (size_t i = 0; i < header_signature_len; ++i) {
		printf("%.2x ", header_signature[i]);
	}

	free(header_signature);
}

FILE_BYTES **read_file_bytes(char *file_name) {
	assert(strlen(file_name) != 0);

	FILE *fp;
	size_t length;
	uint8_t *buffer;

	fp = fopen(file_name, "rb");
	if (fp == NULL) {
		fprintf(stderr, "error: fopen() failed\n");
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);

	length = ftell(fp);
	if (length == 0) {
		fprintf(stderr, "error: ftell() failed\n");
		return NULL;
	}

	fseek(fp, 0L, SEEK_SET);

	buffer = calloc(length, sizeof(uint8_t));
	if (buffer == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	fread(buffer, sizeof(uint8_t), length, fp);

	FILE_BYTES **file_bytes = calloc(length, sizeof(FILE_BYTES *));
	if (file_bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	*file_bytes = calloc(length, sizeof(FILE_BYTES));
	if (*file_bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	(*file_bytes)->bytes = calloc(length, sizeof(uint8_t));
	if ((*file_bytes)->bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	memcpy((*file_bytes)->bytes, buffer, length);
	(*file_bytes)->length = length;

	free(buffer);
	fclose(fp);

	return file_bytes;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "error: You need to specify a luac file\n");
		return -1;
	}

	FILE_BYTES **file_bytes = read_file_bytes(argv[1]);
	parse_header(*file_bytes);

	printf("\n");

	free((*file_bytes)->bytes);
	free(*file_bytes);
	free(file_bytes);

	return 0;
}
