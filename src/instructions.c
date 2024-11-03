#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "colors.h"
#include "instructions.h"

INSTRUCTION **decode_instructions(PROTO *proto) {
	size_t size = (*proto).code_size;
	size_t num_of_instructions = size / 4; /* every instruction is 4 bytes long */

	INSTRUCTION **intr = calloc(num_of_instructions, sizeof(INSTRUCTION*));
	if (intr == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < num_of_instructions; ++i) {
		intr[i] = calloc(1, sizeof(INSTRUCTION));
		if (intr[i] == NULL) {
			fprintf(stderr, "error: calloc() failed\n");
			exit(EXIT_FAILURE);
		}

		uint8_t opcode = proto->code[i] & 0x7f;

		memcpy(&intr[i]->value, &proto->code[i], sizeof(uint32_t));
		memcpy(&intr[i]->opcode, &opcode, sizeof(uint8_t));
	}

	return intr;
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
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.2x" RESET, header_signature[i]);
	}

	// dumpByte(D, LUAC_VERSION) used for detecting version mismatch
	printf(GREEN "\nLUAC_VERSION: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, LUAC_FORMAT) used for detecting format mismatch
	printf(GREEN "\nLUAC_FORMAT VERSION (0=official version): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for error correction
	printf(GREEN "\nLUAC_DATA: " RESET);
	uint8_t *luac_data = poke_bytes(file_bytes, luac_data_len);
	for (size_t i = 0; i < luac_data_len; ++i) {
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.2x" RESET, luac_data[i]);
	}

	// check for any size mismatch
	printf(GREEN "\nsizeof(Instruction): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nsizeof(lua_Integer): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));
	printf(GREEN "\nsizeof(lua_Number): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpInteger(D, LUAC_INT) used for detecting integer format mismatch
	printf(GREEN "\nLUAC_INT: " RESET);
	uint8_t *luac_int = poke_bytes(file_bytes, luac_int_len);
	for (size_t i = 0; i < luac_int_len; ++i) {
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.2x" RESET, luac_int[i]);
	}

	// dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
	printf(GREEN "\nLUAC_NUM: " RESET);
	uint8_t *luac_num = poke_bytes(file_bytes, luac_num_len);
	for (size_t i = 0; i < luac_num_len; ++i) {
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.2x" RESET, luac_num[i]);
	}

	// dumpByte(&D, f->sizeupvalues) upvalues are equivalent of C static variables
	// Used for Lua closures. READ: https://www.lua.org/pil/27.3.3.html
	printf(GREEN "\nsizeof(upvalues) (C API): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	printf(GREEN "\n=======\n" RESET);

	free(luac_data);
	free(luac_int);
	free(luac_num);
	free(header_signature);
}

PROTO **parse_function(FILE_BYTES *file_bytes) {
	PROTO **proto = calloc(file_bytes->length, sizeof(uint8_t));
	if (proto == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	*proto = calloc(file_bytes->length, sizeof(PROTO));
	if (*proto == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	(*proto)->code = calloc(file_bytes->length, sizeof(uint32_t));
	if ((*proto)->code == NULL) {
		fprintf(stderr, "error: calloc() failed\n");
		exit(EXIT_FAILURE);
	}

	// ------------------------
	//       dumpFunction
	// ------------------------
	// dumpSize(D, sizeof(f->source))
	uint8_t sizesource_x = poke_next_byte(file_bytes);
	uint8_t str_size = sizesource_x & ~0x80; /* dumpSize marks the last byte using |= 0x80 */
	printf(GREEN "sizeof(f->source): " RED "0x%.2x" RESET, sizesource_x);

	// dumpString(D, f->source) used for debug information
	printf(GREEN "\nf->source: " RESET);
	uint8_t *proto_source = poke_bytes(file_bytes, (int)str_size - 1); /* @main.lua */
	for (int i = 0; i < (int)str_size - 1; ++i) {
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.2x" RESET, proto_source[i]);
	}

	// dumpInt(D, f->linedefined)
	skip_bytes(file_bytes, 1);

	// dumpInt(D, f->lastlinedefined)
	skip_bytes(file_bytes, 1);

	// dumpByte(D, f->numparams)
	printf(GREEN "\nf->numparams: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, f->is_vararg)
	printf(GREEN "\nf->is_vararg: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, f->maxstacksize)
	printf(GREEN "\nf->maxstacksize: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpInt(D, f->sizecode)
	uint8_t sizecode_x = poke_next_byte(file_bytes);
	uint8_t proto_sizecode = sizecode_x & ~0x80; /* dumpSize marks the last byte using |= 0x80 */
	printf(GREEN "\nf->sizecode: " RED "0x%.2x" RESET, sizecode_x);
	(*proto)->code_size = (int)proto_sizecode * 4; /* 4 bytes is the size of each Instruction */

	// dumpVector(D, f->code, f->sizecode);
	printf(GREEN "\nInstructions: " RESET);
	uint8_t *proto_code = poke_bytes(file_bytes, (*proto)->code_size);
	for (size_t i = 0; i < (*proto)->code_size; i += 4) {
		uint32_t instruction = proto_code[i] | proto_code[i + 1] | proto_code[i + 2] | proto_code[i + 3];
		memcpy(&instruction, &proto_code[i], sizeof(uint32_t));
		(*proto)->code[i / 4] = instruction;
		if (i % 1 == 0 && i != 0) printf(" ");
		printf(RED "%.8x" RESET, instruction);
	}

	// -----------------------
	//      dumpConstants
	// -----------------------
	// dumpInt(D, f->sizek);
	skip_bytes(file_bytes, 8);

	// dumpByte(D, tt);
	skip_bytes(file_bytes, 1);

	// dumpString(D, tt);
	skip_bytes(file_bytes, 1); /* size */

	// dumpVector(D, str, size);
	printf(GREEN "\nstr: " RESET);
	uint8_t *str = poke_bytes(file_bytes, 12);
	for (size_t i = 0; i < 12; ++i) {
		printf(RED "%c" RESET, str[i]);
	}

	skip_bytes(file_bytes, 4);

	// -----------------------
	//       dumpProtos
	// -----------------------
	skip_bytes(file_bytes, 1);

	printf(GREEN "\n=======\n" RESET);

	free(str);
	free(proto_code);
	free(proto_source);

	return proto;
}
