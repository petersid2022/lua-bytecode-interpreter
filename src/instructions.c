#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "utils.h"
#include "colors.h"
#include "instructions.h"

const size_t header_signature_len = 4;
const size_t luac_data_len = 6;
const size_t luac_int_len = 8;
const size_t luac_num_len = 8;

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

	free(luac_int);
	free(luac_num);
	free(luac_data);
	free(header_signature);
}

void dumpFunction(FILE_BYTES *file_bytes, PROTO **proto) {
	// Get filename's length
	uint8_t file_name_len = poke_next_byte(file_bytes) & ~0x80; /* dumpSize marks the last byte using |= 0x80 */

	// dumpString(D, f->source) used for debug information
	printf(GREEN "f->source: " RESET);
	uint8_t *file_name = poke_bytes(file_bytes, (int)file_name_len - 1); /* @main.lua */
	for (int i = 1; i < (int)file_name_len - 1; ++i) {
		printf(RED "%c" RESET, file_name[i]);
	}

	// dumpInt(D, f->linedefined)
	printf(GREEN "\nf->linedefined: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpInt(D, f->lastlinedefined)
	printf(GREEN "\nf->lastlinedefined: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, f->numparams)
	printf(GREEN "\nf->numparams: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, f->is_vararg)
	printf(GREEN "\nf->is_vararg: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpByte(D, f->maxstacksize)
	printf(GREEN "\nf->maxstacksize: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

	// dumpInt(D, f->sizecode)
	uint8_t sizecode = poke_next_byte(file_bytes) & ~0x80;
	(*proto)->code_size = (int)sizecode * 4; /* 4 bytes is the size of each Instruction */

	// dumpVector(D, f->code, f->sizecode);
	printf(GREEN "\nf->code: " RESET);
	uint8_t *code = poke_bytes(file_bytes, (*proto)->code_size);
	for (size_t i = 0; i < (*proto)->code_size; i += 4) {
		uint32_t instruction = (uint32_t)code[i] |
			((uint32_t)code[i + 1] << 8) |
			((uint32_t)code[i + 2] << 16) |
			((uint32_t)code[i + 3] << 24);

		memcpy(&instruction, &code[i], sizeof(uint32_t));

		(*proto)->code[i / 4] = instruction;

		printf(RED);
		printf("\n");
		print_binary(instruction, 32);
		printf(RESET);
	}

	free(code);
	free(file_name);
}

void dumpConstants(FILE_BYTES *file_bytes) {
	/* dumpInt(D, f->sizek) */
	uint8_t sizek = poke_next_byte(file_bytes) & ~0x80;

	for (size_t i = 0; i < sizek; ++i) {
		uint8_t size;
		uint8_t *lua_string, *lua_int, *lua_num;
		uint8_t tt = poke_next_byte(file_bytes); /* type tag of a TValue */

		switch (tt) {
			/*
			 * There's a really annoying way to calculate these
			 * Check lobject.h and ldump.c
			 */
			case 19: /* LUA_VNUMFLT = float numbers */
				printf(GREEN "\nLUA_VNUMFLT: " RESET);
				lua_num = poke_bytes(file_bytes, luac_num_len);
				for (size_t i = 0; i < luac_num_len; ++i) {
					if (i % 1 == 0 && i != 0) printf(" ");
					printf(RED "%.2x" RESET, lua_num[i]);
				}
				free(lua_num);
				break;
			case 3: /* LUA_VNUMINT = integer numbers */
				printf(GREEN "\nLUA_VNUMINT: " RESET);
				lua_int = poke_bytes(file_bytes, luac_int_len);
				for (size_t i = 0; i < luac_int_len; ++i) {
					if (i % 1 == 0 && i != 0) printf(" ");
					printf(RED "%.2x" RESET, lua_int[i]);
				}
				free(lua_int);
				break;
			case 20: /* LUA_VSHRSTR = short strings */
			case 4: /* LUA_VLNGSTR = long strings */
				size = poke_next_byte(file_bytes) & ~0x80;

				printf(GREEN "\nLUA_VLNGSTR: " RESET);
				lua_string = poke_bytes(file_bytes, size - 1);
				for (int i = 0; i < size - 1; ++i) {
					printf(RED "%c" RESET, lua_string[i]);
				}
				free(lua_string);
				break;
			default:
				/* lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE) */
				assert(tt == 0 || tt == 1 || tt == 17);
		}
	}

	printf(GREEN "\n=======\n" RESET);
}

void dumpUpvalues(FILE_BYTES *file_bytes) {
	/* dumpInt(D, f->sizeupvalues) */
	uint8_t sizek = poke_next_byte(file_bytes) & ~0x80;
	for (int i = 0; i < sizek; i++) {
		/* dumpByte(D, f->upvalues[i].instack) wether or not it's stack allocated (register) */
		printf(GREEN "f->upvalues[%d].instack: " RED "0x%.2x" RESET, i, poke_next_byte(file_bytes));

		/* dumpByte(D, f->upvalues[i].idx) the index of upvalue (in stack or in outer function's list) */
		printf(GREEN "\nf->upvalues[%d].idx: " RED "0x%.2x" RESET, i, poke_next_byte(file_bytes));

		/* dumpByte(D, f->upvalues[i].kind) the kind of corresponding variable */
		printf(GREEN "\nf->upvalues[%d].kind: " RED "0x%.2x" RESET, i, poke_next_byte(file_bytes));
	}

	printf(GREEN "\n=======\n" RESET);
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

	dumpFunction(file_bytes, proto);
	dumpConstants(file_bytes);
	dumpUpvalues(file_bytes);

	/* dumpDebug which I probably won't need */
	skip_bytes(file_bytes, 66); 

	return proto;
}
