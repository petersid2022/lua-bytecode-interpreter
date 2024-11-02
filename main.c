#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef enum {
	OP_MOVE,/*	A B	R[A] := R[B]					*/
	OP_LOADI,/*	A sBx	R[A] := sBx					*/
	OP_LOADF,/*	A sBx	R[A] := (lua_Number)sBx				*/
	OP_LOADK,/*	A Bx	R[A] := K[Bx]					*/
	OP_LOADKX,/*	A	R[A] := K[extra arg]				*/
	OP_LOADFALSE,/*	A	R[A] := false					*/
	OP_LFALSESKIP,/*A	R[A] := false; pc++	(*)			*/
	OP_LOADTRUE,/*	A	R[A] := true					*/
	OP_LOADNIL,/*	A B	R[A], R[A+1], ..., R[A+B] := nil		*/
	OP_GETUPVAL,/*	A B	R[A] := UpValue[B]				*/
	OP_SETUPVAL,/*	A B	UpValue[B] := R[A]				*/
	OP_GETTABUP,/*	A B C	R[A] := UpValue[B][K[C]:shortstring]		*/
	OP_GETTABLE,/*	A B C	R[A] := R[B][R[C]]				*/
	OP_GETI,/*	A B C	R[A] := R[B][C]					*/
	OP_GETFIELD,/*	A B C	R[A] := R[B][K[C]:shortstring]			*/
	OP_SETTABUP,/*	A B C	UpValue[A][K[B]:shortstring] := RK(C)		*/
	OP_SETTABLE,/*	A B C	R[A][R[B]] := RK(C)				*/
	OP_SETI,/*	A B C	R[A][B] := RK(C)				*/
	OP_SETFIELD,/*	A B C	R[A][K[B]:shortstring] := RK(C)			*/
	OP_NEWTABLE,/*	A B C k	R[A] := {}					*/
	OP_SELF,/*	A B C	R[A+1] := R[B]; R[A] := R[B][RK(C):string]	*/
	OP_ADDI,/*	A B sC	R[A] := R[B] + sC				*/
	OP_ADDK,/*	A B C	R[A] := R[B] + K[C]:number			*/
	OP_SUBK,/*	A B C	R[A] := R[B] - K[C]:number			*/
	OP_MULK,/*	A B C	R[A] := R[B] * K[C]:number			*/
	OP_MODK,/*	A B C	R[A] := R[B] % K[C]:number			*/
	OP_POWK,/*	A B C	R[A] := R[B] ^ K[C]:number			*/
	OP_DIVK,/*	A B C	R[A] := R[B] / K[C]:number			*/
	OP_IDIVK,/*	A B C	R[A] := R[B] // K[C]:number			*/
	OP_BANDK,/*	A B C	R[A] := R[B] & K[C]:integer			*/
	OP_BORK,/*	A B C	R[A] := R[B] | K[C]:integer			*/
	OP_BXORK,/*	A B C	R[A] := R[B] ~ K[C]:integer			*/
	OP_SHRI,/*	A B sC	R[A] := R[B] >> sC				*/
	OP_SHLI,/*	A B sC	R[A] := sC << R[B]				*/
	OP_ADD,/*	A B C	R[A] := R[B] + R[C]				*/
	OP_SUB,/*	A B C	R[A] := R[B] - R[C]				*/
	OP_MUL,/*	A B C	R[A] := R[B] * R[C]				*/
	OP_MOD,/*	A B C	R[A] := R[B] % R[C]				*/
	OP_POW,/*	A B C	R[A] := R[B] ^ R[C]				*/
	OP_DIV,/*	A B C	R[A] := R[B] / R[C]				*/
	OP_IDIV,/*	A B C	R[A] := R[B] // R[C]				*/
	OP_BAND,/*	A B C	R[A] := R[B] & R[C]				*/
	OP_BOR,/*	A B C	R[A] := R[B] | R[C]				*/
	OP_BXOR,/*	A B C	R[A] := R[B] ~ R[C]				*/
	OP_SHL,/*	A B C	R[A] := R[B] << R[C]				*/
	OP_SHR,/*	A B C	R[A] := R[B] >> R[C]				*/
	OP_MMBIN,/*	A B C	call C metamethod over R[A] and R[B]	(*)	*/
	OP_MMBINI,/*	A sB C k	call C metamethod over R[A] and sB	*/
	OP_MMBINK,/*	A B C k		call C metamethod over R[A] and K[B]	*/
	OP_UNM,/*	A B	R[A] := -R[B]					*/
	OP_BNOT,/*	A B	R[A] := ~R[B]					*/
	OP_NOT,/*	A B	R[A] := not R[B]				*/
	OP_LEN,/*	A B	R[A] := #R[B] (length operator)			*/
	OP_CONCAT,/*	A B	R[A] := R[A].. ... ..R[A + B - 1]		*/
	OP_CLOSE,/*	A	close all upvalues >= R[A]			*/
	OP_TBC,/*	A	mark variable A "to be closed"			*/
	OP_JMP,/*	sJ	pc += sJ					*/
	OP_EQ,/*	A B k	if ((R[A] == R[B]) ~= k) then pc++		*/
	OP_LT,/*	A B k	if ((R[A] <  R[B]) ~= k) then pc++		*/
	OP_LE,/*	A B k	if ((R[A] <= R[B]) ~= k) then pc++		*/
	OP_EQK,/*	A B k	if ((R[A] == K[B]) ~= k) then pc++		*/
	OP_EQI,/*	A sB k	if ((R[A] == sB) ~= k) then pc++		*/
	OP_LTI,/*	A sB k	if ((R[A] < sB) ~= k) then pc++			*/
	OP_LEI,/*	A sB k	if ((R[A] <= sB) ~= k) then pc++		*/
	OP_GTI,/*	A sB k	if ((R[A] > sB) ~= k) then pc++			*/
	OP_GEI,/*	A sB k	if ((R[A] >= sB) ~= k) then pc++		*/
	OP_TEST,/*	A k	if (not R[A] == k) then pc++			*/
	OP_TESTSET,/*	A B k	if (not R[B] == k) then pc++ else R[A] := R[B] (*) */
	OP_CALL,/*	A B C	R[A], ... ,R[A+C-2] := R[A](R[A+1], ... ,R[A+B-1]) */
	OP_TAILCALL,/*	A B C k	return R[A](R[A+1], ... ,R[A+B-1])		*/
	OP_RETURN,/*	A B C k	return R[A], ... ,R[A+B-2]	(see note)	*/
	OP_RETURN0,/*		return						*/
	OP_RETURN1,/*	A	return R[A]					*/
	OP_FORLOOP,/*	A Bx	update counters; if loop continues then pc-=Bx; */
	OP_FORPREP,/*	A Bx	<check values and prepare counters>; if not to run then pc+=Bx+1; */
	OP_TFORPREP,/*	A Bx	create upvalue for R[A + 3]; pc+=Bx		*/
	OP_TFORCALL,/*	A C	R[A+4], ... ,R[A+3+C] := R[A](R[A+1], R[A+2]);	*/
	OP_TFORLOOP,/*	A Bx	if R[A+2] ~= nil then { R[A]=R[A+2]; pc -= Bx }	*/
	OP_SETLIST,/*	A B C k	R[A][C+i] := R[A+i], 1 <= i <= B		*/
	OP_CLOSURE,/*	A Bx	R[A] := closure(KPROTO[Bx])			*/
	OP_VARARG,/*	A C	R[A], R[A+1], ..., R[A+C-2] = vararg		*/
	OP_VARARGPREP,/*A	(adjust vararg parameters)			*/
	OP_EXTRAARG/*	Ax	extra (larger) argument for previous opcode	*/
} OPCODES;

typedef struct {
	OPCODES opcode;
	uint32_t value;
} INSTRUCTION;

typedef struct {
	size_t code_size;
	uint32_t *code;
} PROTO; /* Function Prototypes */

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

void skip_bytes(FILE_BYTES *file_bytes, size_t n) {
	(*file_bytes).cursor += n;
}

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

		memcpy(&intr[i]->value, &proto->code[i], sizeof(uint32_t));
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
	// dumpInt(D, n);
	skip_bytes(file_bytes, 8);

	// dumpByte(D, tt);
	skip_bytes(file_bytes, 1);

	// dumpString(D, tt);
	skip_bytes(file_bytes, 1); /* size */

	// dumpVector(D, str, size);
	printf(GREEN "\nstr: " RESET);
	uint8_t *str = poke_bytes(file_bytes, 12);
	for (size_t i = 0; i < 12; ++i) {
		// if (i % 2 == 0 && i != 0) printf(" ");
		// printf(RED "0x%.2x" RESET, str[i]);
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
	// free(proto_linedefined);
	// free(proto_lastlinedefined);

	return proto;
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
		if (i % 2 == 0 && i != 0) printf(" ");
		if (i % 16 == 0 && i != 0) printf("\n");
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
	PROTO **proto = parse_function(*file_bytes);

	size_t num_of_instructions = (*proto)->code_size / 4;
	INSTRUCTION **instruction = decode_instructions(*proto);

	// for (size_t i = 0; i < num_of_instructions; ++i) {
	// 	printf("[%lu] %.8x\n", i, instruction[i]->value);
	// }

	for (size_t i = 0; i < num_of_instructions; ++i)
		free(instruction[i]);

	free(instruction);

	free((*file_bytes)->bytes);
	free(*file_bytes);
	free(file_bytes);

	free((*proto)->code);
	free(*proto);
	free(proto);

	return 0;
}
