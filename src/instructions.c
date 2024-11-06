#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "instructions.h"
#include "utils.h"

const size_t header_signature_len = 4;
const size_t luac_data_len        = 6;
const size_t luac_int_len         = 8;
const size_t luac_num_len         = 8;

s_Instruction **decode_instructions(s_Func_Prototype *prototype) {
    s_Instruction **instruction = calloc((*prototype).sizecode, sizeof(s_Instruction *));
    if (instruction == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < (*prototype).sizecode; ++i) {
        instruction[i] = calloc(1, sizeof(s_Instruction));
        if (instruction[i] == NULL) {
            fprintf(stderr, "error: calloc() failed\n");
            exit(EXIT_FAILURE);
        }

        uint8_t opcode = prototype->code[i] & 0x7F;

        // switch (opcode) {
        // OP_MOVE:
        //     GET_A(prototype->code[i]);
        //     break;
        // OP_LOADI:
        //     break;
        // OP_LOADF:
        //     break;
        // OP_LOADK:
        //     break;
        // OP_LOADKX:
        //     break;
        // OP_LOADFALSE:
        //     break;
        // OP_LFALSESKIP:
        //     break;
        // OP_LOADTRUE:
        //     break;
        // OP_LOADNIL:
        //     break;
        // OP_GETUPVAL:
        //     break;
        // OP_SETUPVAL:
        //     break;
        // OP_GETTABUP:
        //     break;
        // OP_GETTABLE:
        //     break;
        // OP_GETI:
        //     break;
        // OP_GETFIELD:
        //     break;
        // OP_SETTABUP:
        //     break;
        // OP_SETTABLE:
        //     break;
        // OP_SETI:
        //     break;
        // OP_SETFIELD:
        //     break;
        // OP_NEWTABLE:
        //     break;
        // OP_SELF:
        //     break;
        // OP_ADDI:
        //     break;
        // OP_ADDK:
        //     break;
        // OP_SUBK:
        //     break;
        // OP_MULK:
        //     break;
        // OP_MODK:
        //     break;
        // OP_POWK:
        //     break;
        // OP_DIVK:
        //     break;
        // OP_IDIVK:
        //     break;
        // OP_BANDK:
        //     break;
        // OP_BORK:
        //     break;
        // OP_BXORK:
        //     break;
        // OP_SHRI:
        //     break;
        // OP_SHLI:
        //     break;
        // OP_ADD:
        //     break;
        // OP_SUB:
        //     break;
        // OP_MUL:
        //     break;
        // OP_MOD:
        //     break;
        // OP_POW:
        //     break;
        // OP_DIV:
        //     break;
        // OP_IDIV:
        //     break;
        // OP_BAND:
        //     break;
        // OP_BOR:
        //     break;
        // OP_BXOR:
        //     break;
        // OP_SHL:
        //     break;
        // OP_SHR:
        //     break;
        // OP_MMBIN:
        //     break;
        // OP_MMBINI:
        //     break;
        // OP_MMBINK:
        //     break;
        // OP_UNM:
        //     break;
        // OP_BNOT:
        //     break;
        // OP_NOT:
        //     break;
        // OP_LEN:
        //     break;
        // OP_CONCAT:
        //     break;
        // OP_CLOSE:
        //     break;
        // OP_TBC:
        //     break;
        // OP_JMP:
        //     break;
        // OP_EQ:
        //     break;
        // OP_LT:
        //     break;
        // OP_LE:
        //     break;
        // OP_EQK:
        //     break;
        // OP_EQI:
        //     break;
        // OP_LTI:
        //     break;
        // OP_LEI:
        //     break;
        // OP_GTI:
        //     break;
        // OP_GEI:
        //     break;
        // OP_TEST:
        //     break;
        // OP_TESTSET:
        //     break;
        // OP_CALL:
        //     break;
        // OP_TAILCALL:
        //     break;
        // OP_RETURN:
        //     break;
        // OP_RETURN0:
        //     break;
        // OP_RETURN1:
        //     break;
        // OP_FORLOOP:
        //     break;
        // OP_FORPREP:
        //     break;
        // OP_TFORPREP:
        //     break;
        // OP_TFORCALL:
        //     break;
        // OP_TFORLOOP:
        //     break;
        // OP_SETLIST:
        //     break;
        // OP_CLOSURE:
        //     break;
        // OP_VARARG:
        //     break;
        // OP_VARARGPREP:
        //     break;
        // OP_EXTRAARG:
        //     break;
        // default:
        //     fprintf(stderr, "Failed to match opcode\n");
        //     return NULL;
        // };

        memcpy(&instruction[i]->value, &prototype->code[i], sizeof(uint32_t));
        memcpy(&instruction[i]->opcode, &opcode, sizeof(uint8_t));
    }

    return instruction;
}

void parse_header(s_Filebytes *file_bytes) {
    printf(GREEN "\n=======\n" RESET);

    // dumpLiteral(D, LUA_SIGNATURE) (aka magic bytes) sizeof = 5
    // since it's a string literal the last byte is a null terminator,
    // which we strip
    printf(GREEN "LUA_SIGNATURE: " RESET);
    uint8_t *header_signature = poke_bytes(file_bytes, header_signature_len);
    for (size_t i = 0; i < header_signature_len; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf(RED "%.2x" RESET, header_signature[i]);
    }

    // dumpByte(D, LUAC_VERSION) used for detecting version mismatch
    printf(GREEN "\nLUAC_VERSION: " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

    // dumpByte(D, LUAC_FORMAT) used for detecting format mismatch
    printf(GREEN "\nLUAC_FORMAT VERSION (0=official version): " RED "0x%.2x" RESET,
        poke_next_byte(file_bytes));

    // dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for error correction
    printf(GREEN "\nLUAC_DATA: " RESET);
    uint8_t *luac_data = poke_bytes(file_bytes, luac_data_len);
    for (size_t i = 0; i < luac_data_len; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf(RED "%.2x" RESET, luac_data[i]);
    }

    // check for any size mismatch
    uint8_t size_instr = poke_next_byte(file_bytes);
    printf(GREEN "\nsizeof(Instruction): " RED "0x%.2x" RESET, size_instr);

    uint8_t size_lua_int = poke_next_byte(file_bytes);
    printf(GREEN "\nsizeof(lua_Integer): " RED "0x%.2x" RESET, size_lua_int);

    uint8_t size_lua_num = poke_next_byte(file_bytes);
    printf(GREEN "\nsizeof(lua_Number): " RED "0x%.2x" RESET, size_lua_num);

    // dumpInteger(D, LUAC_INT) used for detecting integer format mismatch
    printf(GREEN "\nLUAC_INT: " RESET);
    uint8_t *luac_int = poke_bytes(file_bytes, luac_int_len);
    for (size_t i = 0; i < luac_int_len; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf(RED "%.2x" RESET, luac_int[i]);
    }

    // dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
    printf(GREEN "\nLUAC_NUM: " RESET);
    uint8_t *luac_num = poke_bytes(file_bytes, luac_num_len);
    for (size_t i = 0; i < luac_num_len; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf(RED "%.2x" RESET, luac_num[i]);
    }

    // dumpByte(&D, f->sizeupvalues) upvalues are equivalent of C static variables
    // Used for Lua closures. READ: https://www.lua.org/pil/27.3.3.html
    printf(GREEN "\nsizeof(upvalues): " RED "0x%.2x" RESET, poke_next_byte(file_bytes));

    printf(GREEN "\n=======\n" RESET);

    free(luac_int);
    free(luac_num);
    free(luac_data);
    free(header_signature);
}

void dump_function(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    /* Get filename's length. dumpSize marks the last byte using |= 0x80 */
    int file_name_len = poke_next_byte(file_bytes) & ~0x80;

    // dumpString(D, f->source) used for debug information
    printf(GREEN "proto->source: " RESET);
    uint8_t *file_name = poke_bytes(file_bytes, (size_t) (file_name_len - 1)); /* @main.lua */
    for (size_t i = 1; i < (size_t) (file_name_len - 1); ++i) {
        printf(RED "%c" RESET, file_name[i]);
    }

    // dumpInt(D, f->linedefined)
    uint8_t linedefined = poke_next_byte(file_bytes);
    printf(GREEN "\nproto->linedefined: " RED "0x%.2x" RESET, linedefined);

    // dumpInt(D, f->lastlinedefined)
    uint8_t lastlinedefined = poke_next_byte(file_bytes);
    printf(GREEN "\nproto->lastlinedefined: " RED "0x%.2x" RESET, lastlinedefined);

    // dumpByte(D, f->numparams)
    uint8_t numparams = poke_next_byte(file_bytes);
    printf(GREEN "\nproto->numparams: " RED "0x%.2x" RESET, numparams);

    // dumpByte(D, f->is_vararg)
    uint8_t vararg = poke_next_byte(file_bytes);
    printf(GREEN "\nproto->is_vararg: " RED "0x%.2x" RESET, vararg);

    // dumpByte(D, f->maxstacksize)
    uint8_t maxstacksize = poke_next_byte(file_bytes);
    printf(GREEN "\nproto->maxstacksize: " RED "0x%.2x" RESET, maxstacksize);

    // dumpInt(D, f->sizecode)
    (*prototype)->sizecode = poke_next_byte(file_bytes) & ~0x80;

    // dumpVector(D, f->code, f->sizecode);
    printf(GREEN "\nproto->code:\n" RESET);
    uint8_t *code = poke_bytes(file_bytes, (*prototype)->sizecode * 4);
    for (int i = 0; i < (*prototype)->sizecode * 4; i += 4) {
        uint32_t instruction = (uint32_t) code[i] | ((uint32_t) code[i + 1] << 8) |
            ((uint32_t) code[i + 2] << 16) | ((uint32_t) code[i + 3] << 24);

        (*prototype)->code[i / 4] = instruction;

        printf(RED);
        printf("%-15s\t", opnames[(*prototype)->code[i / 4] & 0x7F]);
        print_binary(instruction, 32);
        printf(RESET);
    }

    free(code);
    free(file_name);
}

void dump_constants(s_Filebytes *file_bytes) {
    /* dumpInt(D, f->sizek) */
    int sizek = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < sizek; ++i) {
        int      size;
        uint8_t *lua_string, *lua_int, *lua_num;
        uint8_t  tt = poke_next_byte(file_bytes); /* type tag of a TValue */

        switch (tt) {
        /*
         * There's a really annoying way to calculate these
         * Check lobject.h and ldump.c
         */
        case 19: /* LUA_VNUMFLT = float numbers */
            printf(GREEN "LUA_VNUMFLT: " RESET);
            lua_num = poke_bytes(file_bytes, luac_num_len);
            for (size_t i = 0; i < luac_num_len; ++i) {
                if (i % 1 == 0 && i != 0)
                    printf(" ");
                printf(RED "%.2x" RESET, lua_num[i]);
            }
            free(lua_num);
            break;
        case 3: /* LUA_VNUMINT = integer numbers */
            printf(GREEN "LUA_VNUMINT: " RESET);
            lua_int = poke_bytes(file_bytes, luac_int_len);
            for (size_t i = 0; i < luac_int_len; ++i) {
                if (i % 1 == 0 && i != 0)
                    printf(" ");
                printf(RED "%.2x" RESET, lua_int[i]);
            }
            free(lua_int);
            break;
        case 20: /* LUA_VSHRSTR = short strings */
        case 4:  /* LUA_VLNGSTR = long strings */
            size = poke_next_byte(file_bytes) & ~0x80;

            printf(GREEN "LUA_VLNGSTR: " RESET);
            lua_string = poke_bytes(file_bytes, size - 1);
            for (int i = 0; i < size - 1; ++i) {
                printf(RED "%c" RESET, lua_string[i]);
            }
            printf("\n");
            free(lua_string);
            break;
        default:
            /* lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE) */
            assert(tt == 0 || tt == 1 || tt == 17);
        }
    }

    printf(GREEN "=======\n" RESET);
}

void dump_upvalues(s_Filebytes *file_bytes, s_Func_Prototype **proto) {
    /* dumpInt(D, f->sizeupvalues) */
    (*proto)->sizeupvalues = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*proto)->sizeupvalues; i++) {
        /* dumpByte(D, f->upvalues[i].instack) wether or not it's stack allocated
         * (register) */
        uint8_t instack = poke_next_byte(file_bytes);
        printf(GREEN "proto->upvalues[%d].instack: " RED "0x%.2x" RESET, i, instack);
        (*proto)->upvalues[i].instack = instack;

        /* dumpByte(D, f->upvalues[i].idx) the index of upvalue (in stack or in
         * outer function's list) */
        uint8_t idx = poke_next_byte(file_bytes);
        printf(GREEN "\nproto->upvalues[%d].idx: " RED "0x%.2x" RESET, i, idx);
        (*proto)->upvalues[i].idx = idx;

        /* dumpByte(D, f->upvalues[i].kind) the kind of corresponding variable */
        uint8_t kind = poke_next_byte(file_bytes);
        printf(GREEN "\nproto->upvalues[%d].kind: " RED "0x%.2x" RESET, i, kind);
        (*proto)->upvalues[i].kind = kind;
    }

    printf(GREEN "\n=======\n" RESET);
}

s_Func_Prototype **parse_function(s_Filebytes *file_bytes) {
    s_Func_Prototype **proto = calloc(file_bytes->length, sizeof(uint8_t));
    if (proto == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    *proto = calloc(file_bytes->length, sizeof(s_Func_Prototype));
    if (*proto == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    (*proto)->code = calloc(file_bytes->length, sizeof(uint32_t));
    if ((*proto)->code == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    (*proto)->upvalues = calloc(file_bytes->length, sizeof(s_Upvalue_Desc));
    if ((*proto)->code == NULL) {
        fprintf(stderr, "error: calloc() failed\n");
        exit(EXIT_FAILURE);
    }

    dump_function(file_bytes, proto);
    dump_constants(file_bytes);
    dump_upvalues(file_bytes, proto);

    /* dumpDebug which I probably won't need */
    skip_bytes(file_bytes, 66);

    return proto;
}
