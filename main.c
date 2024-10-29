#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef enum {
	STRING,
	NUMBER,
	FUNCTION,
	BOOLEAN,
	NIL,
} TOKEN_TYPE;

typedef struct TOKEN {
	char *value;
	TOKEN_TYPE type;
	struct TOKEN *next;
} TOKEN;

typedef struct {
	size_t length;
	uint8_t *bytes;
} FILE_BYTES;

uint8_t *poke_bytes(FILE_BYTES *file_bytes, uint8_t amount, uint8_t cursor) {
	assert(amount > 0);

	uint8_t *bytes = calloc(amount, sizeof(uint8_t));
	if (bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		return NULL;
	}

	memcpy(bytes, file_bytes->bytes + cursor, amount);

	return bytes;
}

void parse_header(FILE_BYTES *file_bytes) {
	printf(GREEN "\n=======\n" RESET);

	size_t header_size = 31;
	printf(GREEN "Header block: " RESET);
	for (size_t i = 0; i < header_size; ++i) {
		printf(RED "%.2x " RESET, (*file_bytes).bytes[i]);
	}

	// dumpLiteral(D, LUA_SIGNATURE) AKA magic bytes
	// sizeof(LUA_SIGNATURE) = 4 + 1 (null terminator, which we strip)
	printf(GREEN "\nLua signature: " RESET);
	size_t header_signature_len = 4;
	uint8_t *header_signature = poke_bytes(file_bytes, header_signature_len, 0);
	for (size_t i = 0; i < header_signature_len; ++i) {
		printf(RED "%.2x " RESET, header_signature[i]);
	}

	// dumpByte(D, LUAC_VERSION)
	printf(GREEN "\nLuac Version: " RED "%.2x" RESET, (*file_bytes).bytes[header_signature_len]);

	// dumpByte(D, LUAC_FORMAT) used for detecting format mismatching
	printf(GREEN "\nLuac Format version (0=official version): " RED "%.2x" RESET, (*file_bytes).bytes[header_signature_len + 1]);

	// dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for error correction
	printf(GREEN "\nLuac data : " RESET);
	size_t luac_data_len = 6;
	uint8_t *luac_data = poke_bytes(file_bytes, luac_data_len, header_signature_len + 2);
	for (size_t i = 0; i < luac_data_len; ++i) {
		printf(RED "%.2x " RESET, luac_data[i]);
	}

	printf(GREEN "\nsizeof(Instruction): " RED "%.2x" RESET, (*file_bytes).bytes[header_signature_len + luac_data_len + 2]);
	printf(GREEN "\nsizeof(lua_Integer): " RED "%.2x" RESET, (*file_bytes).bytes[header_signature_len + luac_data_len + 3]);
	printf(GREEN "\nsizeof(lua_Number): " RED "%.2x" RESET, (*file_bytes).bytes[header_signature_len + luac_data_len + 4]);

	// dumpInteger(D, LUAC_INT) 0x5678 used for detecting integer format mismatch
	printf(GREEN "\nLUAC_INT: " RESET);
	size_t luac_int_len = 2;
	uint8_t *luac_int = poke_bytes(file_bytes, luac_int_len, header_signature_len + luac_data_len + 5);
	for (int i = luac_int_len - 1; i >= 0; i--) {
		printf(RED "%.2x " RESET, luac_int[i]);
	}

	// Native endianness
	printf(RED "(little-endian)" RESET);

	// dumpInteger(D, LUAC_NUM) 0x4077280000000000 = 370.5 used for detecting float format mismatch
	printf(GREEN "\nLUAC_NUM: " RESET);
	size_t luac_num_len = 8;
	uint8_t *luac_num = poke_bytes(file_bytes, luac_num_len, luac_num_len + header_signature_len + luac_data_len + 5);
	for (int i = luac_num_len - 1; i >= 0; i--) {
		printf(RED "%.2x " RESET, luac_num[i]);
	}

	// Native endianness
	printf(RED "(little-endian)" RESET);

	printf(GREEN "\nFinal size of the header: " RED "%lu bytes" RESET, luac_num_len + luac_num_len + header_signature_len + luac_data_len + 5);

	// sizeof upvalues used for Lua closures
	printf(GREEN "\nsizeof(upvalues): " RED "%.2x" RESET, (*file_bytes).bytes[header_size]);

	printf(GREEN "\n=======\n" RESET);

	free(header_signature);
	free(luac_data);
	free(luac_int);
	free(luac_num);
}

// void parse_function(FILE_BYTES *file_bytes) {
// 	// dumpFunction
// 	// f->source (used for debug info) sizeof(f->source) is 8 and we are going to skip it
// }

FILE_BYTES **read_file_bytes(char *file_name) {
	assert(strlen(file_name) != 0);

	FILE *fp;
	size_t length;
	uint8_t *buffer;

	fp = fopen(file_name, "rb");
	if (fp == NULL) {
		fprintf(stderr, "error: fopen() failed\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0L, SEEK_END);

	length = ftell(fp);
	if (length == 0) {
		fprintf(stderr, "error: ftell() failed\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0L, SEEK_SET);

	buffer = calloc(length, sizeof(uint8_t));
	if (buffer == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	fread(buffer, sizeof(uint8_t), length, fp);

	FILE_BYTES **file_bytes = calloc(length, sizeof(FILE_BYTES *));
	if (file_bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	*file_bytes = calloc(length, sizeof(FILE_BYTES));
	if (*file_bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	(*file_bytes)->bytes = calloc(length, sizeof(uint8_t));
	if ((*file_bytes)->bytes == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
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

	// Bad way of checking if the file the user provided is a valid Lua bytecode
	// binary but it is what it is. Basically, the first field of the header is
	// the LUA_SIGNATURE i.e. "\x1bLua" So, I just check if the second byte of the
	// file is equal to 'L'
	assert((*file_bytes)->bytes[1] == '\x4c');

	for (size_t i = 0; i < (*file_bytes)->length; ++i) {
		printf(RED "%.2x " RESET, (*file_bytes)->bytes[i]);
	}
	
	// Stepping into luaU_dump
	parse_header(*file_bytes);

	free((*file_bytes)->bytes);
	free(*file_bytes);
	free(file_bytes);

	return 0;
}
