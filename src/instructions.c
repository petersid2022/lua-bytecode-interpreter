#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#include "utils.h"

void match_instructions(s_Func_Prototype **p) {
    int amount = (*p)->sizecode;
    bool is_nested = (*p)->nested;

    if (is_nested) {
        amount += (*(*p)->p)->sizecode;
    }

    uint32_t *instructions = malloc(amount * sizeof(uint32_t));
    s_Upvalue_Desc *upvalues = malloc(amount * sizeof(s_Upvalue_Desc));

    memcpy(instructions, (*p)->code, (*p)->sizecode * sizeof(uint32_t));
    memcpy(upvalues, (*p)->upvalues, (*p)->sizeupvalues * sizeof(s_Upvalue_Desc));

    if (is_nested) {
        memcpy(instructions + (*p)->sizecode, (*(*p)->p)->code, (*(*p)->p)->sizecode * sizeof(uint32_t));
        memcpy(upvalues + (*p)->sizeupvalues, (*(*p)->p)->upvalues, (*(*p)->p)->sizeupvalues * sizeof(s_Upvalue_Desc));
    }

    for (int i = 0; i < amount; i++) {
        uint32_t code = instructions[i];
        uint8_t opcode = code & 0x7F;

        switch (opcode) {
        case OP_MOVE:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_LOADI:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_sBx(code));
            break;
        case OP_LOADF:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_sBx(code));
            break;
        case OP_LOADK:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_LOADKX:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADFALSE:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LFALSESKIP:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADTRUE:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADNIL:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_GETUPVAL:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_SETUPVAL:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_GETTABUP:
            printf("%-10s %d %d %d\t; %-*s\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code), 5,
                upvalues[GET_B(code)].name);
            break;
        case OP_GETTABLE:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_GETI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_GETFIELD:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETTABUP:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETTABLE:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETFIELD:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_NEWTABLE:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SELF:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_ADDI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_sC(code));
            break;
        case OP_ADDK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SUBK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MULK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MODK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_POWK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_DIVK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_IDIVK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BANDK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BORK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BXORK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SHRI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_sC(code));
            break;
        case OP_SHLI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_sC(code));
            break;
        case OP_ADD:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SUB:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MUL:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MOD:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_POW:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_DIV:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_IDIV:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BAND:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BOR:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BXOR:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SHL:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SHR:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MMBIN:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MMBINI:
            printf("%-10s %d %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_C(code), GET_k(code));
            break;
        case OP_MMBINK:
            printf("%-10s %d %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code), GET_k(code));
            break;
        case OP_UNM:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_BNOT:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_NOT:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_LEN:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_CONCAT:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_CLOSE:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_TBC:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        // this is wrong. need to take into account closures
        case OP_JMP:
            printf("%-10s %d\t; to %-.*d\n", opnames[opcode], GET_sJ(code), 2, GET_sJ(code) + i + 2);
            break;
        case OP_EQ:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_k(code));
            break;
        case OP_LT:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_k(code));
            break;
        case OP_LE:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_k(code));
            break;
        case OP_EQK:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_k(code));
            break;
        case OP_EQI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_k(code));
            break;
        case OP_LTI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_k(code));
            break;
        case OP_LEI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_k(code));
            break;
        case OP_GTI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_k(code));
            break;
        case OP_GEI:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_sB(code), GET_k(code));
            break;
        case OP_TEST:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_k(code));
            break;
        case OP_TESTSET:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_k(code));
            break;
        case OP_CALL:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_TAILCALL:
            printf("%-10s %d %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code), GET_k(code));
            break;
        case OP_RETURN:
            printf("%-10s %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_RETURN0:
            printf("%-10s\n", opnames[opcode]);
            break;
        case OP_RETURN1:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_FORLOOP:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_FORPREP:
            printf("%-10s %d %d\t; exit to %-10d\n", opnames[opcode], GET_A(code), GET_Bx(code), i + GET_Bx(code) + 3);
            break;
        case OP_TFORPREP:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_TFORCALL:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_C(code));
            break;
        case OP_TFORLOOP:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_SETLIST:
            printf("%-10s %d %d %d %d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code), GET_k(code));
            break;
        case OP_CLOSURE:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_VARARG:
            printf("%-10s %d %d\n", opnames[opcode], GET_A(code), GET_C(code));
            break;
        case OP_VARARGPREP:
            printf("%-10s %d\n", opnames[opcode], GET_A(code));
            break;
        case OP_EXTRAARG:
            printf("%-10s %d\n", opnames[opcode], GET_Ax(code));
            break;
        default:
            printf("Failed to match opcode: %s\n", opnames[opcode]);
            break;
        };
    }

    free(instructions);
    free(upvalues);
}

void dump_header(s_Filebytes *file_bytes) {
    printf("\n");

    /* dumpLiteral(D, LUA_SIGNATURE) since it's a string literal the last byte is a null terminator,
     * which we strip */
    printf("LUA_SIGNATURE: ");
    uint8_t *header_signature = poke_bytes(file_bytes, HEADER_SIGNATURE_LEN);
    for (size_t i = 0; i < HEADER_SIGNATURE_LEN; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf("0x%.2x", header_signature[i]);
    }

    // dumpByte(D, LUAC_VERSION) used for detecting version mismatch
    printf("\nLUAC_VERSION: 0x%.2x", poke_next_byte(file_bytes));

    // dumpByte(D, LUAC_FORMAT) used for detecting format mismatch
    printf("\nLUAC_FORMAT: 0x%.2x", poke_next_byte(file_bytes));

    // dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for ERROR correction
    printf("\nLUAC_DATA: ");
    uint8_t *luac_data = poke_bytes(file_bytes, LUAC_DATA_LEN);
    for (size_t i = 0; i < LUAC_DATA_LEN; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf("0x%.2x", luac_data[i]);
    }

    // check for any size mismatch
    uint8_t size_instr = poke_next_byte(file_bytes);
    printf("\nsizeof(Instruction): 0x%.2x", size_instr);

    uint8_t size_lua_int = poke_next_byte(file_bytes);
    printf("\nsizeof(lua_Integer): 0x%.2x", size_lua_int);

    uint8_t size_lua_num = poke_next_byte(file_bytes);
    printf("\nsizeof(lua_Number): 0x%.2x", size_lua_num);

    // dumpInteger(D, LUAC_INT) used for detecting integer format mismatch
    printf("\nLUAC_INT: ");
    uint8_t *luac_int = poke_bytes(file_bytes, LUAC_INT_LEN);
    for (size_t i = 0; i < LUAC_INT_LEN; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf("0x%.2x", luac_int[i]);
    }

    // dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
    printf("\nLUAC_NUM: ");
    uint8_t *luac_num = poke_bytes(file_bytes, LUAC_NUM_LEN);
    for (size_t i = 0; i < LUAC_NUM_LEN; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf("0x%.2x", luac_num[i]);
    }

    // dumpByte(&D, f->sizeupvalues) upvalues are equivalent of C static variables
    // Used for Lua closures. READ: https://www.lua.org/pil/27.3.3.html
    uint8_t sizeupvalues = poke_next_byte(file_bytes);
    printf("\nsizeof(upvalues): 0x%.2x\n", sizeupvalues);

    free(luac_int);
    free(luac_num);
    free(luac_data);
    free(header_signature);
}

void dump_function(s_Filebytes *file_bytes, s_Func_Prototype **prototype, bool is_nested) {
    if (!is_nested) {
        /* Get filename's length. dumpSize marks the last byte using |= 0x80 */
        int file_name_len = poke_next_byte(file_bytes) & ~0x80;

        // dumpString(D, f->source) used for debug information
        printf("prototype->source: ");
        uint8_t *file_name = poke_bytes(file_bytes, (size_t) (file_name_len - 1)); /* @main.lua */
        for (size_t i = 1; i < (size_t) (file_name_len - 1); ++i) {
            printf("%c", file_name[i]);
        }
        memcpy((*prototype)->source, (char *) file_name, file_name_len - 1);

        free(file_name);

        printf("\n");
    }

    // dumpInt(D, f->linedefined)
    uint8_t linedefined = poke_next_byte(file_bytes);
    printf("prototype->linedefined: 0x%.2d", linedefined & ~0x80);
    (*prototype)->linedefined = linedefined & ~0x80;

    // dumpInt(D, f->lastlinedefined)
    uint8_t lastlinedefined = poke_next_byte(file_bytes);
    printf("\nprototype->lastlinedefined: 0x%.2d", lastlinedefined & ~0x80);
    (*prototype)->lastlinedefined = lastlinedefined & ~0x80;

    // dumpByte(D, f->numparams)
    uint8_t numparams = poke_next_byte(file_bytes);
    printf("\nprototype->numparams: 0x%.2d", (int) numparams);
    (*prototype)->numparams = (int) numparams;

    // dumpByte(D, f->is_vararg)
    uint8_t vararg = poke_next_byte(file_bytes);
    printf("\nprototype->is_vararg: 0x%.2x", vararg);

    // dumpByte(D, f->maxstacksize)
    uint8_t maxstacksize = poke_next_byte(file_bytes);
    printf("\nprototype->maxstacksize: 0x%.2d", (int) maxstacksize);
    (*prototype)->maxstacksize = (int) maxstacksize;

    // dumpInt(D, f->sizecode)
    (*prototype)->sizecode = poke_next_byte(file_bytes) & ~0x80;

    // dumpVector(D, f->code, f->sizecode);
    printf("\nprototype->code:\n");
    uint8_t *code = poke_bytes(file_bytes, (*prototype)->sizecode * 4);
    for (int i = 0; i < (*prototype)->sizecode * 4; i += 4) {
        uint32_t instruction = (uint32_t) code[i] | ((uint32_t) code[i + 1] << 8) | ((uint32_t) code[i + 2] << 16) |
            ((uint32_t) code[i + 3] << 24);
        (*prototype)->code[i / 4] = instruction;
        print_binary(instruction, 32);
    }

    free(code);
}

void dump_constants(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    /* dumpInt(D, f->sizek) */
    int sizek = poke_next_byte(file_bytes) & ~0x80;
    (*prototype)->sizek = sizek;

    for (int i = 0; i < sizek; ++i) {
        int size;
        uint8_t *lua_string, *lua_int, *lua_num;
        uint8_t tt = poke_next_byte(file_bytes); /* type tag of a TValue */

        switch (tt) {
        /*
         * There's a really annoying way to calculate these
         * Check lobject.h and ldump.c
         */
        case 19: /* LUA_VNUMFLT = float numbers */
            printf("LUA_VNUMFLT: ");
            lua_num = poke_bytes(file_bytes, LUAC_NUM_LEN);
            for (size_t i = 0; i < LUAC_NUM_LEN; ++i) {
                if (i % 1 == 0 && i != 0)
                    printf(" ");
                printf("%.2x", lua_num[i]);
            }
            free(lua_num);
            break;
        case 3: /* LUA_VNUMINT = integer numbers */
            printf("LUA_VNUMINT: ");
            lua_int = poke_bytes(file_bytes, LUAC_INT_LEN);
            for (size_t i = 0; i < LUAC_INT_LEN; ++i) {
                if (i % 1 == 0 && i != 0)
                    printf(" ");
                printf("%.2x", lua_int[i]);
            }
            free(lua_int);
            break;
        case 20: /* LUA_VSHRSTR = short strings */
        case 4:  /* LUA_VLNGSTR = long strings */
            size = poke_next_byte(file_bytes) & ~0x80;

            printf("LUA_VLNGSTR: ");
            lua_string = poke_bytes(file_bytes, size - 1);
            for (int i = 0; i < size - 1; ++i) {
                printf("%c", lua_string[i]);
            }
            free(lua_string);
            break;
        default:
            /* lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE) */
            // continue;
            assert(tt == 0 || tt == 1 || tt == 17);
        }

        printf("\n");
    }
}

void dump_upvalues(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    /* dumpInt(D, f->sizeupvalues) */
    (*prototype)->sizeupvalues = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizeupvalues; i++) {
        /* dumpByte(D, f->upvalues[i].instack) wether or not it's stack allocated
         * (register) */
        uint8_t instack = poke_next_byte(file_bytes);
        printf("prototype->upvalues[%d].instack: 0x%.2x", i, instack);
        (*prototype)->upvalues[i].instack = instack;

        /* dumpByte(D, f->upvalues[i].idx) the index of upvalue (in stack or in
         * outer function's list) */
        uint8_t idx = poke_next_byte(file_bytes);
        printf("\nprototype->upvalues[%d].idx: 0x%.2x", i, idx);
        (*prototype)->upvalues[i].idx = idx;

        /* dumpByte(D, f->upvalues[i].kind) the kind of corresponding variable */
        uint8_t kind = poke_next_byte(file_bytes);
        printf("\nprototype->upvalues[%d].kind: 0x%.2x", i, kind);
        (*prototype)->upvalues[i].kind = kind;
    }
    printf("\n");
}

void dump_protos(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    // dumpInt(D, f->sizep)
    (*prototype)->sizep = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizep; i++) {
        (*prototype)->p = safe_malloc((*prototype)->sizep * sizeof(s_Func_Prototype *));
        (*prototype)->p[i] = safe_malloc(sizeof(s_Func_Prototype));
        (*prototype)->p[i]->abslineinfo = safe_malloc(file_bytes->length * sizeof(s_AbsLineInfo));
        (*prototype)->p[i]->lineinfo = safe_malloc(file_bytes->length * sizeof(uint8_t));
        (*prototype)->p[i]->code = safe_malloc(file_bytes->length * sizeof(uint32_t));
        (*prototype)->p[i]->upvalues = safe_malloc(file_bytes->length * sizeof(s_Upvalue_Desc));
        (*prototype)->p[i]->locvars = safe_malloc(file_bytes->length * sizeof(s_LocVar));
        (*prototype)->p[i]->source = safe_malloc(file_bytes->length * sizeof(char));
    }

    for (int i = 0; i < (*prototype)->sizep; i++) {
        (*prototype)->nested = true;
        skip_bytes(file_bytes, 1);
        int scopecount = (*prototype)->scopecount + 1;
        parse_functions(file_bytes, (*prototype)->p, true, scopecount);
    }
}

void dump_debug(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    // Since stripping debug info is by default set to 0 (can be enabled via `-s`)
    // dumpInt(D, f->sizelineinfo)
    (*prototype)->sizelineinfo = poke_next_byte(file_bytes) & ~0x80;

    // dumpVector(D, f->lineinfo, n);
    uint8_t *lineinfo = poke_bytes(file_bytes, (*prototype)->sizelineinfo);
    for (int i = 0; i < (*prototype)->sizelineinfo; ++i) {
        (*prototype)->lineinfo[i] = lineinfo[i];
    }

    // dumpInt(D, f->sizeabslineinfo)
    (*prototype)->sizeabslineinfo = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizeabslineinfo; ++i) {
        (*prototype)->abslineinfo[i].pc = poke_next_byte(file_bytes) & ~0x80;
        (*prototype)->abslineinfo[i].line = poke_next_byte(file_bytes) & ~0x80;
    }

    // dumpInt(D, f->sizelocvars)
    (*prototype)->sizelocvars = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizelocvars; ++i) {
        int size = poke_next_byte(file_bytes) & ~0x80;
        uint8_t *sizelocvars = poke_bytes(file_bytes, size - 1);
        (*prototype)->locvars[i].varname = safe_malloc(size);

        for (int j = 0; j < size - 1; ++j)
            memcpy(&(*prototype)->locvars[i].varname[j], &sizelocvars[j], 1);

        (*prototype)->locvars[i].varname[size - 1] = '\0';

        free(sizelocvars);

        (*prototype)->locvars[i].startpc = poke_next_byte(file_bytes) & ~0x80;
        (*prototype)->locvars[i].endpc = poke_next_byte(file_bytes) & ~0x80;
    }

    // dumpInt(D, f->sizeupvalues)
    (*prototype)->sizeupvalues = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizeupvalues; ++i) {
        int size = poke_next_byte(file_bytes) & ~0x80;
        uint8_t *name = poke_bytes(file_bytes, size - 1);
        (*prototype)->upvalues[i].name = safe_malloc(size);

        for (int j = 0; j < size - 1; ++j)
            memcpy(&(*prototype)->upvalues[i].name[j], &name[j], 1);

        (*prototype)->upvalues[i].name[size - 1] = '\0';

        free(name);
    }

    free(lineinfo);
}

void parse_functions(s_Filebytes *file_bytes, s_Func_Prototype **prototype, bool is_nested, int scopecount) {
    printf("\n----[%d]----\n\n", scopecount);
    dump_function(file_bytes, prototype, is_nested);
    dump_constants(file_bytes, prototype);
    dump_upvalues(file_bytes, prototype);
    dump_protos(file_bytes, prototype);
    dump_debug(file_bytes, prototype);
}

void cleanup_prototypes(s_Func_Prototype **prototypes) {
    for (int i = 0; i < (*prototypes)->sizelocvars; ++i)
        free((*prototypes)->locvars[i].varname);

    for (int i = 0; i < (*prototypes)->sizeupvalues; ++i)
        free((*prototypes)->upvalues[i].name);

    free((*prototypes)->source);
    free((*prototypes)->upvalues);
    free((*prototypes)->locvars);
    free((*prototypes)->abslineinfo);
    free((*prototypes)->lineinfo);
    free((*prototypes)->code);

    for (int i = 0; i < (*prototypes)->sizep; i++) {
        s_Func_Prototype *proto = (*prototypes)->p[i];

        for (int j = 0; j < proto->sizelocvars; ++j) {
            free(proto->locvars[j].varname);
        }

        for (int k = 0; k < proto->sizeupvalues; ++k) {
            free(proto->upvalues[k].name);
        }

        free(proto->source);
        free(proto->upvalues);
        free(proto->locvars);
        free(proto->abslineinfo);
        free(proto->lineinfo);
        free(proto->code);

        free((*prototypes)->p);
        free(proto);
    }

    free(*prototypes);
    free(prototypes);
}

void parse_hexdump(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    dump_header(file_bytes);

    prototype = safe_malloc(file_bytes->length * sizeof(s_Func_Prototype *));
    *prototype = safe_malloc(file_bytes->length * sizeof(s_Func_Prototype));
    (*prototype)->abslineinfo = safe_malloc(file_bytes->length * sizeof(s_AbsLineInfo));
    (*prototype)->lineinfo = safe_malloc(file_bytes->length * sizeof(uint8_t));
    (*prototype)->code = safe_malloc(file_bytes->length * sizeof(uint32_t));
    (*prototype)->upvalues = safe_malloc(file_bytes->length * sizeof(s_Upvalue_Desc));
    (*prototype)->locvars = safe_malloc(file_bytes->length * sizeof(s_LocVar));
    (*prototype)->source = safe_malloc(file_bytes->length * sizeof(char));
    (*prototype)->nested = false;
    (*prototype)->scopecount = 1;

    parse_functions(file_bytes, prototype, false, (*prototype)->scopecount);
    match_instructions(prototype);
    cleanup_prototypes(prototype);
}
