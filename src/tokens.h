#ifndef TOKENS_H_
#define TOKENS_H_

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

#endif // TOKENS_H_
