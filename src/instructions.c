#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"
#include "utils.h"

const size_t header_signature_len = 4;
const size_t luac_data_len        = 6;
const size_t luac_int_len         = 8;
const size_t luac_num_len         = 8;

void decode_instructions(s_Func_Prototype *prototype) {
    printf("\nBYTECODE:\n");

    for (int i = 0; i < prototype->sizecode; ++i) {
        uint32_t code   = prototype->code[i];
        uint8_t  opcode = code & 0x7F;

        switch (opcode) {
        case OP_MOVE:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_LOADI:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_sBx(code));
            break;
        case OP_LOADF:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_sBx(code));
            break;
        case OP_LOADK:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_LOADKX:
            printf("%-10s %-5d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADFALSE:
            printf("%-10s %-5d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LFALSESKIP:
            printf("%-10s %-5d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADTRUE:
            printf("%-10s %-5d\n", opnames[opcode], GET_A(code));
            break;
        case OP_LOADNIL:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_GETUPVAL:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_SETUPVAL:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_GETTABUP:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_GETTABLE:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_GETI:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_GETFIELD:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETTABUP:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETTABLE:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETI:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SETFIELD:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_NEWTABLE:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SELF:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        // case OP_ADDI:
        //     printf("matched OP_ADDI\n");
        //     break;
        case OP_ADDK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SUBK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MULK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MODK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_POWK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_DIVK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_IDIVK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BANDK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BORK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BXORK:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        // case OP_SHRI:
        //     printf("matched OP_SHRI\n");
        //     break;
        // case OP_SHLI:
        //     printf("matched OP_SHLI\n");
        //     break;
        case OP_ADD:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SUB:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MUL:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MOD:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_POW:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_DIV:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_IDIV:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BAND:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BOR:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_BXOR:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SHL:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_SHR:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        case OP_MMBIN:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        // case OP_MMBINI:
        //     printf("matched OP_MMBINI\n");
        //     break;
        // case OP_MMBINK:
        //     printf("matched OP_MMBINK\n");
        //     break;
        case OP_UNM:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_BNOT:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_NOT:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_LEN:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        case OP_CONCAT:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code));
            break;
        // case OP_CLOSE:
        //     printf("matched OP_CLOSE\n");
        //     break;
        // case OP_TBC:
        //     printf("matched OP_TBC\n");
        //     break;
        // case OP_JMP:
        //     printf("matched OP_JMP\n");
        //     break;
        // case OP_EQ:
        //     printf("matched OP_EQ\n");
        //     break;
        // case OP_LT:
        //     printf("matched OP_LT\n");
        //     break;
        // case OP_LE:
        //     printf("matched OP_LE\n");
        //     break;
        // case OP_EQK:
        //     printf("matched OP_EQK\n");
        //     break;
        // case OP_EQI:
        //     printf("matched OP_EQI\n");
        //     break;
        // case OP_LTI:
        //     printf("matched OP_LTI\n");
        //     break;
        // case OP_LEI:
        //     printf("matched OP_LEI\n");
        //     break;
        // case OP_GTI:
        //     printf("matched OP_GTI\n");
        //     break;
        // case OP_GEI:
        //     printf("matched OP_GEI\n");
        //     break;
        // case OP_TEST:
        //     printf("matched OP_TEST\n");
        //     break;
        // case OP_TESTSET:
        //     printf("matched OP_TESTSET\n");
        //     break;
        case OP_CALL:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        // case OP_TAILCALL:
        //     printf("matched OP_TAILCALL\n");
        //     break;
        case OP_RETURN:
            printf(
                "%-10s %-5d %-5d %-5d\n", opnames[opcode], GET_A(code), GET_B(code), GET_C(code));
            break;
        // case OP_RETURN0:
        //     printf("matched OP_RETURN0\n");
        //     break;
        // case OP_RETURN1:
        //     printf("matched OP_RETURN1\n");
        //     break;
        case OP_FORLOOP:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        case OP_FORPREP:
            printf("%-10s %-5d %-5d\n", opnames[opcode], GET_A(code), GET_Bx(code));
            break;
        // case OP_TFORPREP:
        //     printf("matched OP_TFORPREP\n");
        //     break;
        // case OP_TFORCALL:
        //     printf("matched OP_TFORCALL\n");
        //     break;
        // case OP_TFORLOOP:
        //     printf("matched OP_TFORLOOP\n");
        //     break;
        // case OP_SETLIST:
        //     printf("matched OP_SETLIST\n");
        //     break;
        // case OP_CLOSURE:
        //     printf("matched OP_CLOSURE\n");
        //     break;
        // case OP_VARARG:
        //     printf("matched OP_VARARG\n");
        //     break;
        case OP_VARARGPREP:
            printf("%-10s %-5d\n", opnames[opcode], GET_A(code));
            break;
        // case OP_EXTRAARG:
        //     printf("matched OP_EXTRAARG\n");
        //     break;
        default:
            printf("Failed to match opcode: %s\n", opnames[opcode]);
        };
    }
}

void parse_header(s_Filebytes *file_bytes) {
    printf("\nHEADER: ");

    /* dumpLiteral(D, LUA_SIGNATURE) since it's a string literal the last byte is a null terminator,
     * which we strip */
    printf("\nLUA_SIGNATURE: ");
    uint8_t *header_signature = poke_bytes(file_bytes, header_signature_len);
    for (size_t i = 0; i < header_signature_len; ++i) {
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
    uint8_t *luac_data = poke_bytes(file_bytes, luac_data_len);
    for (size_t i = 0; i < luac_data_len; ++i) {
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
    uint8_t *luac_int = poke_bytes(file_bytes, luac_int_len);
    for (size_t i = 0; i < luac_int_len; ++i) {
        if (i % 1 == 0 && i != 0)
            printf(" ");
        printf("0x%.2x", luac_int[i]);
    }

    // dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
    printf("\nLUAC_NUM: ");
    uint8_t *luac_num = poke_bytes(file_bytes, luac_num_len);
    for (size_t i = 0; i < luac_num_len; ++i) {
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

void dump_function(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    printf("\nFUNCTION:\n");

    /* Get filename's length. dumpSize marks the last byte using |= 0x80 */
    int file_name_len = poke_next_byte(file_bytes) & ~0x80;

    // dumpString(D, f->source) used for debug information
    printf("proto->source: ");
    uint8_t *file_name = poke_bytes(file_bytes, (size_t) (file_name_len - 1)); /* @main.lua */
    for (size_t i = 1; i < (size_t) (file_name_len - 1); ++i) {
        printf("%c", file_name[i]);
    }
    (*prototype)->source = (char *) file_name;

    // dumpInt(D, f->linedefined)
    uint8_t linedefined = poke_next_byte(file_bytes);
    printf("\nproto->linedefined: 0x%.2x", linedefined);
    (*prototype)->linedefined = (int) linedefined;

    // dumpInt(D, f->lastlinedefined)
    uint8_t lastlinedefined = poke_next_byte(file_bytes);
    printf("\nproto->lastlinedefined: 0x%.2x", lastlinedefined);
    (*prototype)->lastlinedefined = (int) lastlinedefined;

    // dumpByte(D, f->numparams)
    uint8_t numparams = poke_next_byte(file_bytes);
    printf("\nproto->numparams: 0x%.2x", numparams);
    (*prototype)->numparams = (int) numparams;

    // dumpByte(D, f->is_vararg)
    uint8_t vararg = poke_next_byte(file_bytes);
    printf("\nproto->is_vararg: 0x%.2x", vararg);

    // dumpByte(D, f->maxstacksize)
    uint8_t maxstacksize = poke_next_byte(file_bytes);
    printf("\nproto->maxstacksize: 0x%.2x", maxstacksize);
    (*prototype)->maxstacksize = (int) maxstacksize;

    // dumpInt(D, f->sizecode)
    (*prototype)->sizecode = poke_next_byte(file_bytes) & ~0x80;

    // dumpVector(D, f->code, f->sizecode);
    printf("\nproto->code:\n");
    uint8_t *code = poke_bytes(file_bytes, (*prototype)->sizecode * 4);
    for (int i = 0; i < (*prototype)->sizecode * 4; i += 4) {
        uint32_t instruction = (uint32_t) code[i] | ((uint32_t) code[i + 1] << 8) |
            ((uint32_t) code[i + 2] << 16) | ((uint32_t) code[i + 3] << 24);

        (*prototype)->code[i / 4] = instruction;

        print_binary(instruction, 32);
    }

    free(code);
    free(file_name);
}

void dump_constants(s_Filebytes *file_bytes) {
    printf("\nCONSTANTS:\n");

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
            printf("LUA_VNUMFLT: ");
            lua_num = poke_bytes(file_bytes, luac_num_len);
            for (size_t i = 0; i < luac_num_len; ++i) {
                if (i % 1 == 0 && i != 0)
                    printf(" ");
                printf("%.2x", lua_num[i]);
            }
            free(lua_num);
            break;
        case 3: /* LUA_VNUMINT = integer numbers */
            printf("LUA_VNUMINT: ");
            lua_int = poke_bytes(file_bytes, luac_int_len);
            for (size_t i = 0; i < luac_int_len; ++i) {
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
            printf("\n");
            free(lua_string);
            break;
        default:
            /* lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE) */
            assert(tt == 0 || tt == 1 || tt == 17);
        }
    }
}

void dump_upvalues(s_Filebytes *file_bytes, s_Func_Prototype **prototype) {
    printf("\nUPVALUES:\n");

    /* dumpInt(D, f->sizeupvalues) */
    (*prototype)->sizeupvalues = poke_next_byte(file_bytes) & ~0x80;

    for (int i = 0; i < (*prototype)->sizeupvalues; i++) {
        /* dumpByte(D, f->upvalues[i].instack) wether or not it's stack allocated
         * (register) */
        uint8_t instack = poke_next_byte(file_bytes);
        printf("proto->upvalues[%d].instack: 0x%.2x", i, instack);
        (*prototype)->upvalues[i].instack = instack;

        /* dumpByte(D, f->upvalues[i].idx) the index of upvalue (in stack or in
         * outer function's list) */
        uint8_t idx = poke_next_byte(file_bytes);
        printf("\nproto->upvalues[%d].idx: 0x%.2x", i, idx);
        (*prototype)->upvalues[i].idx = idx;

        /* dumpByte(D, f->upvalues[i].kind) the kind of corresponding variable */
        uint8_t kind = poke_next_byte(file_bytes);
        printf("\nproto->upvalues[%d].kind: 0x%.2x", i, kind);
        (*prototype)->upvalues[i].kind = kind;
    }

    printf("\n");
}

s_Func_Prototype **parse_function(s_Filebytes *file_bytes) {
    s_Func_Prototype **prototype = safe_malloc(file_bytes->length * sizeof(s_Func_Prototype *));

    *prototype = safe_malloc(file_bytes->length * sizeof(s_Func_Prototype));

    (*prototype)->upvalues = safe_malloc(file_bytes->length * sizeof(s_Upvalue_Desc));

    dump_function(file_bytes, prototype);
    dump_constants(file_bytes);
    dump_upvalues(file_bytes, prototype);

    /* dumpDebug which I probably won't need */
    skip_bytes(file_bytes, 66);

    return prototype;
}
