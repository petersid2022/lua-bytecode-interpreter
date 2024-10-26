#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// typedef enum {
// 	STRING,
// 	NUMBER,
// 	FUNCTION,
// 	BOOLEAN,
// 	NIL,
// } TOKEN_TYPE;
// 
// typedef struct TOKEN {
// 	char *value;
// 	struct TOKEN *next;
// 	TOKEN_TYPE type;
// } TOKEN;
// 
// Token tokenize_bytecode(char *contents) {
// 	assert(strlen(contents) != 0);
// 
// }

uint8_t *read_file_bytes(char *file_name) {
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
		fprintf(stderr, "error: malloc() failed\n");
		return NULL;
	}

	fread(buffer, sizeof(uint8_t), length, fp);

	fclose(fp);

	return buffer;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "error: You need to specify a luac file\n");
		return -1;
	}

	uint8_t *contents = read_file_bytes(argv[1]);

	printf("sizeof: %lu\n", sizeof(contents));

	for (size_t i = 0; i < sizeof(contents); ++i) {
		printf("%hhn\n", contents);
	}

	// Token *tokens = tokenize_bytecode(contents);

	free(contents);

	return 0;
}
