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
	size_t cursor;
	uint8_t *bytes;
} FILE_BYTES;

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

void parse_header(FILE_BYTES *file_bytes) {
	size_t header_signature_len = 4;
	size_t luac_data_len = 6;
	size_t luac_int_len = 8;
	size_t luac_num_len = 8;

	printf(GREEN "\n=======\n" RESET);

	// dumpLiteral(D, LUA_SIGNATURE) (aka magic bytes) sizeof = 4 + 1
	// since it's a string literal the last byte is a null terminator, which we strip
	printf(GREEN "LUA_SIGNATURE: " RESET);
	uint8_t *header_signature = poke_bytes(file_bytes, header_signature_len);
	for (size_t i = 0; i < header_signature_len; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");

		printf(RED "%.2x" RESET, header_signature[i]);
	}

	// dumpByte(D, LUAC_VERSION) used for detecting version mismatch
	printf(GREEN "\nLUAC_VERSION: " RED "%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, LUAC_FORMAT) used for detecting format mismatch
	printf(GREEN "\nLUAC_FORMAT version (0=official version): " RED "%.2x" RESET, poke_next_byte(file_bytes));

	// dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for error correction
	printf(GREEN "\nLUAC_DATA: " RESET);
	uint8_t *luac_data = poke_bytes(file_bytes, luac_data_len);
	for (size_t i = 0; i < luac_data_len; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");

		printf(RED "%.2x" RESET, luac_data[i]);
	}

	// check for any size mismatch
	printf(GREEN "\nsizeof(Instruction): " RED "%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nsizeof(lua_Integer): " RED "%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nsizeof(lua_Number): " RED "%.2x" RESET, poke_next_byte(file_bytes));

	// dumpInteger(D, LUAC_INT) used for detecting integer format mismatch
	printf(GREEN "\nLUAC_INT: " RESET);
	uint8_t *luac_int = poke_bytes(file_bytes, luac_int_len);
	for (int i = luac_int_len - 1; i >= 0; i--) {
		printf(RED "%.2x" RESET, luac_int[i]);
		if (i % 2 == 0 && i != 0)
			printf(" ");
	}


	printf(RED "\t(little-endian)" RESET); // endianness is native to the platform

	// dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
	printf(GREEN "\nLUAC_NUM: " RESET);
	uint8_t *luac_num = poke_bytes(file_bytes, luac_num_len);
	for (int i = luac_num_len - 1; i >= 0; i--) {
		printf(RED "%.2x" RESET, luac_num[i]);
		if (i % 2 == 0 && i != 0)
			printf(" ");
	}

	printf(RED "\t(little-endian)" RESET); // endianness is native to the platform

	// dumpByte(&D, f->sizeupvalues) used for Lua closures
	printf(GREEN "\nf->sizeupvalues: " RED "%.2x" RESET, poke_next_byte(file_bytes));

	printf(GREEN "\n=======\n" RESET);

	free(luac_data);
	free(luac_int);
	free(luac_num);
	free(header_signature);
}

void parse_function(FILE_BYTES *file_bytes) {
	size_t proto_source_len = 8;
	size_t proto_linedefined_len = 4;
	size_t proto_lastlinedefined_len = 4;

	/*
	   dumpString(D, f->source);
	   dumpInt(D, f->linedefined);
	   dumpInt(D, f->lastlinedefined);
	   dumpByte(D, f->numparams);
	   dumpByte(D, f->is_vararg);
	   dumpByte(D, f->maxstacksize);
	   dumpCode(D, f);
	   dumpConstants(D, f);
	   dumpUpvalues(D, f);
	   dumpProtos(D, f);
	   dumpDebug(D, f);
	   */


	// dumpString(D, f->source) used for debugging
	printf(GREEN "f->source: " RESET);
	uint8_t *proto_source = poke_bytes(file_bytes, proto_source_len);
	for (size_t i = 0; i < proto_source_len; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");
		printf(RED "%.2x" RESET, proto_source[i]);
	}

	// dumpInt(D, f->linedefined) used for debugging
	printf(GREEN "\nf->linedefined: " RESET);
	uint8_t *proto_linedefined = poke_bytes(file_bytes, proto_linedefined_len);
	for (size_t i = 0; i < proto_linedefined_len; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");
		printf(RED "%.2x" RESET, proto_linedefined[i]);
	}

	// dumpInt(D, f->lastlinedefined) used for debugging
	printf(GREEN "\nf->lastlinedefined: " RESET);
	uint8_t *proto_lastlinedefined = poke_bytes(file_bytes, proto_lastlinedefined_len);
	for (size_t i = 0; i < proto_lastlinedefined_len; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");
		printf(RED "%.2x" RESET, proto_lastlinedefined[i]);
	}

	printf(GREEN "\nf->numparams: " RED "%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nf->is_vararg: " RED "%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nf->maxstacksize: " RED "%.2x" RESET, poke_next_byte(file_bytes));

	printf(GREEN "\n=======\n" RESET);

	free(proto_source);
	free(proto_linedefined);
	free(proto_lastlinedefined);
}

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
	(*file_bytes)->cursor = 0;

	free(buffer);
	fclose(fp);

	return file_bytes;
}

void print_full_hex(FILE_BYTES **file_bytes) {
	for (size_t i = 0; i < (*file_bytes)->length; ++i) {
		if (i % 2 == 0 && i != 0)
			printf(" ");

		if (i % 22 == 0 && i != 0)
			printf("\n");


		printf(RED "%.2x" RESET, (*file_bytes)->bytes[i]);
	}
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

	print_full_hex(file_bytes);
	parse_header(*file_bytes);
	parse_function(*file_bytes);

	free((*file_bytes)->bytes);
	free(*file_bytes);
	free(file_bytes);

	return 0;
}
