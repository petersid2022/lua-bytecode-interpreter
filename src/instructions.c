#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "instructions.h"
#include "utils.h"

static inline s_Func_Prototype *new_luac_prototype(void) {
  return arena_calloc_array(&arena, 1, sizeof(s_Func_Prototype));
}

static inline const char *print_constant(s_Func_Prototype *p, int i) {
  (void)p;
  (void)i;
  const s_TValue *o = (s_TValue *)&(p->locvars[i]);
  switch (ttypetag(o)) {
  case LUA_VNUMFLT:
  case LUA_VNUMINT:
    return "LUA_VNUMINT or LUA_VNUMFLT";
    break;
  case LUA_VSHRSTR:
  case LUA_VLNGSTR:
    return "LUA_VLNGSTR or LUA_VSHRSTR";
    break;
  default:
    return NULL;
    break;
  }
}

static inline void print_instruction(const char *opname, int count, const char *comment, ...) {
  va_list args;
  va_start(args, comment);

  fprintf(stdout, "%-10s\t", opname);

  for (int i = 0; i < count; i++) {
    fprintf(stdout, "%-2d", va_arg(args, int));
  }

  fprintf(stdout, "\t%s\n", comment ? comment : "");

  va_end(args);
}

static inline void dump_luac_instructions(s_Func_Prototype *p) {
  for (int i = 0; i < p->sizecode; i++) {
    s_Upvalue_Desc *upvalues = p->upvalues;

    uint32_t code = p->code[i];
    uint8_t opcode = code & 0x7F;

    char *comment = arena_malloc(&arena, 1024);

    if (i > 0) {
      printf("\t%d\t", i);
    }

    switch (opcode) {
    case OP_MOVE:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_LOADI:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_sBx(code));
      break;
    case OP_LOADF:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_sBx(code));
      break;
    case OP_LOADK:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_Bx(code));
      break;
    case OP_LOADKX:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_LOADFALSE:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_LFALSESKIP:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_LOADTRUE:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_LOADNIL:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_GETUPVAL:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_SETUPVAL:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_GETTABUP:
      sprintf(comment, "; %s %s", upvalues[GET_B(code)].name, print_constant(p, GET_C(code)));
      print_instruction(opnames[opcode], 3, comment, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_GETTABLE:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_GETI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_GETFIELD:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SETTABUP:
      sprintf(comment, "; %s %s", UPVALNAME(GET_A(code)), print_constant(p, GET_C(code)));
      print_instruction(opnames[opcode], 3, comment, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SETTABLE:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SETI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SETFIELD:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_NEWTABLE:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SELF:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_ADDI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_sC(code));
      break;
    case OP_ADDK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SUBK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MULK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MODK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_POWK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_DIVK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_IDIVK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BANDK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BORK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BXORK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SHRI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_sC(code));
      break;
    case OP_SHLI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_sC(code));
      break;
    case OP_ADD:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SUB:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MUL:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MOD:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_POW:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_DIV:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_IDIV:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BAND:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BOR:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_BXOR:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SHL:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_SHR:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MMBIN:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_MMBINI:
      sprintf(comment, "; %d", GET_k(code));
      print_instruction(opnames[opcode], 3, (const char *)comment, GET_A(code), GET_sB(code), GET_C(code));
      break;
    case OP_MMBINK:
      sprintf(comment, "; %d", GET_k(code));
      print_instruction(opnames[opcode], 3, (const char *)comment, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_UNM:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_BNOT:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_NOT:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_LEN:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_CONCAT:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_B(code));
      break;
    case OP_CLOSE:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_TBC:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_JMP:
      sprintf(comment, "; to %d", GET_sJ(code) + i + 1);
      print_instruction(opnames[opcode], 1, comment, GET_sJ(code));
      break;
    case OP_EQ:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_k(code));
      break;
    case OP_LT:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_k(code));
      break;
    case OP_LE:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_k(code));
      break;
    case OP_EQK:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_k(code));
      break;
    case OP_EQI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_sB(code), GET_k(code));
      break;
    case OP_LTI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_sB(code), GET_k(code));
      break;
    case OP_LEI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_sB(code), GET_k(code));
      break;
    case OP_GTI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_sB(code), GET_k(code));
      break;
    case OP_GEI:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_sB(code), GET_k(code));
      break;
    case OP_TEST:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_k(code));
      break;
    case OP_TESTSET:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_k(code));
      break;
    case OP_CALL:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_TAILCALL:
      sprintf(comment, "; %d", GET_k(code));
      print_instruction(opnames[opcode], 3, (const char *)comment, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_RETURN:
      print_instruction(opnames[opcode], 3, NULL, GET_A(code), GET_B(code), GET_C(code));
      break;
    case OP_RETURN0:
      print_instruction(opnames[opcode], 0, NULL);
      break;
    case OP_RETURN1:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_FORLOOP:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_Bx(code));
      break;
    case OP_FORPREP:
      sprintf(comment, "; exit to %d", i + GET_Bx(code) + 2);
      print_instruction(opnames[opcode], 2, comment, GET_A(code), GET_Bx(code));
      break;
    case OP_TFORPREP:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_Bx(code));
      break;
    case OP_TFORCALL:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_C(code));
      break;
    case OP_TFORLOOP:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_Bx(code));
      break;
    case OP_SETLIST:
      print_instruction(opnames[opcode], 4, NULL, GET_A(code), GET_B(code), GET_C(code), GET_k(code));
      break;
    case OP_CLOSURE:
      sprintf(comment, "; %p", (const void *)(p)->p[GET_Bx(code)]);
      print_instruction(opnames[opcode], 2, comment, GET_A(code), GET_Bx(code));
      break;
    case OP_VARARG:
      print_instruction(opnames[opcode], 2, NULL, GET_A(code), GET_C(code));
      break;
    case OP_VARARGPREP:
      print_instruction(opnames[opcode], 1, NULL, GET_A(code));
      break;
    case OP_EXTRAARG:
      print_instruction(opnames[opcode], 1, NULL, GET_Ax(code));
      break;
    case OP_NULL:
      sprintf(comment, "<%s:%d,%d> (%d instructions at %p)", (p)->source, (p)->linedefined, (p)->lastlinedefined, (p)->sizecode - 1, (void *)(p));
      printf("\n%s %s\n", ((p)->linedefined == 0) ? "main" : "function", comment);
      printf("%d%s param%s, %d slot%s, %d upvalue%s, ", (int)((p)->numparams), (p)->is_vararg ? "+" : "", SS((p)->numparams), S((p)->maxstacksize), S((p)->sizeupvalues));
      printf("%d local%s, %d constant%s, %d function%s\n", S((p)->sizelocvars), S((p)->sizek), S((p)->sizep));
      break;
    default:
      printf("Failed to match opcode: %s\n", opnames[opcode]);
      break;
    };
  }
}

static inline void dump_luac_function(s_String_View *contents, s_Func_Prototype *prototype, bool is_nested) {
  s_String_View sv;

  prototype->nested = is_nested;

  // get filename's length. dumpSize marks the last byte using |= 0x80
  int file_name_len = poke_next_byte(contents) & ~0x80;

  if (file_name_len > 0) {
    // dumpString(D, f->source) used for debug information
    sv = poke_next_bytes(contents, (size_t)(file_name_len - 1)); /* @main.lua */

    printf("prototype->source: ");
    for (size_t i = 1; i < sv.count; ++i) {
      printf("%c", sv.data[i]);
    }
    printf("\n");

    prototype->source = arena_calloc_array(&arena, sv.count + 1, sizeof(*prototype->source));
    memcpy(prototype->source, sv.data, sv.count);
  }

  // dumpInt(D, f->linedefined)
  uint8_t linedefined = poke_next_byte(contents) & ~0x80;
  printf("prototype->linedefined: 0x%.2d", linedefined);
  prototype->linedefined = linedefined;

  // dumpInt(D, f->lastlinedefined)
  uint8_t lastlinedefined = poke_next_byte(contents) & ~0x80;
  printf("\nprototype->lastlinedefined: 0x%.2d", lastlinedefined);
  prototype->lastlinedefined = lastlinedefined;

  // dumpByte(D, f->numparams)
  uint8_t numparams = poke_next_byte(contents);
  printf("\nprototype->numparams: 0x%.2d", (int)numparams);
  prototype->numparams = (int)numparams;

  // dumpByte(D, f->is_vararg)
  printf("\nprototype->is_vararg: 0x%.2x", poke_next_byte(contents));

  // dumpByte(D, f->maxstacksize)
  uint8_t maxstacksize = poke_next_byte(contents);
  printf("\nprototype->maxstacksize: 0x%.2d", (int)maxstacksize);
  prototype->maxstacksize = (int)maxstacksize;

  // dumpInt(D, f->sizecode)
  prototype->sizecode = poke_next_byte(contents) & ~0x80;

  prototype->code = arena_calloc_array(&arena, (size_t)prototype->sizecode, sizeof(*prototype->code));

  // dumpVector(D, f->code, f->sizecode);
  sv = poke_next_bytes(contents, (size_t)prototype->sizecode * sizeof(*prototype->code));

  printf("\nprototype->code:\n");
  for (size_t i = 0; i < (size_t)prototype->sizecode; ++i) {
    if (i == 0) {
      prototype->code[i] = OP_NULL;
      continue;
    }
    memcpy(&prototype->code[i], sv.data + i * sizeof(prototype->code[i]), sizeof(prototype->code[i]));
    printf("%zu:\t", i);
    print_binary(prototype->code[i]);
  }
}

static inline void dump_luac_constants(s_String_View *contents, s_Func_Prototype *prototype) {
  /* dumpInt(D, f->sizek) */
  int sizek = poke_next_byte(contents) & ~0x80;

  prototype->sizek = sizek;
  prototype->tagval = arena_calloc_array(&arena, (size_t)prototype->sizek, sizeof(*prototype->tagval));

  for (int i = 0; i < sizek; ++i) {
    int size;
    s_String_View sv;

    uint8_t tt = poke_next_byte(contents); /* type tag of a TValue */
    prototype->tagval[i].tt_ = tt;

    switch (tt) {
    /*
     * There's a really annoying way to calculate these
     * Check lobject.h and ldump.c
     */
    case LUA_VNUMFLT: /* LUA_VNUMFLT = float numbers */
      printf("LUA_VNUMFLT: ");
      sv = poke_next_bytes(contents, LUAC_NUM_LEN);
      for (size_t i = 0; i < sv.count; ++i) {
        if (i % 1 == 0 && i != 0) {
          printf(" ");
        }
        printf("%.2x", sv.data[i]);
      }
      prototype->tagval[i].value_.n = 5.0;
      break;
    case LUA_VNUMINT: /* LUA_VNUMINT = integer numbers */
      printf("LUA_VNUMINT: ");
      sv = poke_next_bytes(contents, LUAC_INT_LEN);
      for (size_t i = 0; i < sv.count; ++i) {
        if (i % 1 == 0 && i != 0) {
          printf(" ");
        }
        printf("%.2x", sv.data[i]);
      }
      prototype->tagval[i].value_.n = 5.0;
      break;
    case LUA_VSHRSTR: /* LUA_VSHRSTR = short strings */
    case LUA_VLNGSTR: /* LUA_VLNGSTR = long strings */
      size = poke_next_byte(contents) & ~0x80;
      printf("LUA_VLNGSTR: ");
      sv = poke_next_bytes(contents, size - 1);
      for (size_t i = 0; i < sv.count; ++i) {
        printf("%c", sv.data[i]);
      }
      prototype->tagval[i].value_.p = (void *)"Hello";
      break;
    default:
      /* lua_assert(tt == LUA_VNIL || tt == LUA_VFALSE || tt == LUA_VTRUE) */
      // continue;
      assert(tt == 0 || tt == 1 || tt == 17);
    }

    printf("\n");
  }
}

static inline void dump_luac_upvalues(s_String_View *contents, s_Func_Prototype *prototype) {
  /* dumpInt(D, f->sizeupvalues) */
  prototype->sizeupvalues = poke_next_byte(contents) & ~0x80;
  prototype->upvalues = arena_calloc_array(&arena, (size_t)prototype->sizeupvalues, sizeof(*prototype->upvalues));

  for (int i = 0; i < prototype->sizeupvalues; i++) {
    /* dumpByte(D, f->upvalues[i].instack) wether or not it's stack allocated
     * (register) */
    uint8_t instack = poke_next_byte(contents);
    printf("prototype->upvalues[%d].instack: 0x%.2x", i, instack);
    prototype->upvalues[i].instack = instack;

    /* dumpByte(D, f->upvalues[i].idx) the index of upvalue (in stack or in
     * outer function's list) */
    uint8_t idx = poke_next_byte(contents);
    printf("\nprototype->upvalues[%d].idx: 0x%.2x", i, idx);
    prototype->upvalues[i].idx = idx;

    /* dumpByte(D, f->upvalues[i].kind) the kind of corresponding variable */
    uint8_t kind = poke_next_byte(contents);
    printf("\nprototype->upvalues[%d].kind: 0x%.2x", i, kind);
    prototype->upvalues[i].kind = kind;
  }

  printf("\n");
}

static inline void dump_luac_protos(s_String_View *contents, s_Func_Prototype *prototype) {
  // dumpInt(D, f->sizep)
  prototype->sizep = poke_next_byte(contents) & ~0x80;
  prototype->p = arena_calloc_array(&arena, (size_t)prototype->sizep, sizeof(s_Func_Prototype *));

  for (int i = 0; i < prototype->sizep; i++) {
    s_Func_Prototype *child = new_luac_prototype();
    child->source = prototype->source;
    child->scopecount = prototype->scopecount + 1;
    prototype->p[i] = child;
    parse_luac_bytecode(contents, child, true);
  }
}

static inline void dump_luac_debug(s_String_View *contents, s_Func_Prototype *prototype) {
  s_String_View sv;

  // Since stripping debug info is by default set to 0 (can be enabled via `-s`)
  // dumpInt(D, f->sizelineinfo)
  prototype->sizelineinfo = poke_next_byte(contents) & ~0x80;
  prototype->lineinfo = arena_calloc_array(&arena, (size_t)prototype->sizelineinfo, sizeof(*prototype->lineinfo));

  // dumpVector(D, f->lineinfo, n);
  sv = poke_next_bytes(contents, prototype->sizelineinfo);
  for (int i = 0; i < prototype->sizelineinfo; ++i) {
    prototype->lineinfo[i] = sv.data[i];
  }

  // dumpInt(D, f->sizeabslineinfo)
  prototype->sizeabslineinfo = poke_next_byte(contents) & ~0x80;
  prototype->abslineinfo = arena_calloc_array(&arena, (size_t)prototype->sizeabslineinfo, sizeof(*prototype->abslineinfo));

  for (int i = 0; i < prototype->sizeabslineinfo; ++i) {
    prototype->abslineinfo[i].pc = poke_next_byte(contents) & ~0x80;
    prototype->abslineinfo[i].line = poke_next_byte(contents) & ~0x80;
  }

  // dumpInt(D, f->sizelocvars)
  prototype->sizelocvars = poke_next_byte(contents) & ~0x80;
  prototype->locvars = arena_calloc_array(&arena, (size_t)prototype->sizelocvars, sizeof(*prototype->locvars));

  for (int i = 0; i < prototype->sizelocvars; ++i) {
    int size = poke_next_byte(contents) & ~0x80;

    if (size > 0) {
      sv = poke_next_bytes(contents, (size_t)(size - 1));
      prototype->locvars[i].varname = arena_calloc_array(&arena, (size_t)size, sizeof(*prototype->locvars[i].varname));
      memcpy(prototype->locvars[i].varname, sv.data, sv.count);
    }

    prototype->locvars[i].startpc = poke_next_byte(contents) & ~0x80;
    prototype->locvars[i].endpc = poke_next_byte(contents) & ~0x80;
  }

  // dumpInt(D, f->sizeupvalues)
  int sizeupvaluenames = poke_next_byte(contents) & ~0x80;
  assert(sizeupvaluenames <= prototype->sizeupvalues);

  for (int i = 0; i < sizeupvaluenames; ++i) {
    int size = poke_next_byte(contents) & ~0x80;

    if (size > 0) {
      sv = poke_next_bytes(contents, (size_t)(size - 1));
      prototype->upvalues[i].name = arena_calloc_array(&arena, (size_t)size, sizeof(*prototype->upvalues[i].name));
      memcpy(prototype->upvalues[i].name, sv.data, sv.count);
    }
  }
}

void parse_luac_bytecode(s_String_View *contents, s_Func_Prototype *prototype, bool is_nested) {
  printf("\n----[%d]----\n\n", prototype->scopecount);
  dump_luac_function(contents, prototype, is_nested);
  dump_luac_constants(contents, prototype);
  dump_luac_upvalues(contents, prototype);
  dump_luac_protos(contents, prototype);
  dump_luac_debug(contents, prototype);
  dump_luac_instructions(prototype);
}

void parse_luac_hex_dump(s_String_View *sv) {
  s_Func_Prototype *prototype = new_luac_prototype();
  parse_luac_bytecode(sv, prototype, false);
}

void print_luac_header(s_String_View *contents) {
  s_String_View sv;

  /* dumpLiteral(D, LUA_SIGNATURE) since it's a string literal the last byte is
   * a null terminator, which we strip */
  sv = poke_next_bytes(contents, HEADER_SIGNATURE_LEN);

  printf("\nLUA_SIGNATURE: ");
  for (size_t i = 0; i < sv.count; ++i) {
    if (i % 1 == 0 && i != 0) {
      printf(" ");
    }

    printf("0x%.2x", sv.data[i]);
  }

  // dumpByte(D, LUAC_VERSION) used for detecting version mismatch
  printf("\nLUAC_VERSION: 0x%.2x", poke_next_byte(contents));

  // dumpByte(D, LUAC_FORMAT) used for detecting format mismatch
  printf("\nLUAC_FORMAT: 0x%.2x", poke_next_byte(contents));

  // dumpLiteral(D, LUAC_DATA) "\x19\x93\r\n\x1a\n" used for ERROR correction
  sv = poke_next_bytes(contents, LUAC_DATA_LEN);

  printf("\nLUAC_DATA: ");
  for (size_t i = 0; i < sv.count; ++i) {
    if (i % 1 == 0 && i != 0) {
      printf(" ");
    }

    printf("0x%.2x", (unsigned char)sv.data[i]);
  }

  // check for any size mismatch
  printf("\nsizeof(Instruction): 0x%.2x", poke_next_byte(contents));
  printf("\nsizeof(lua_Integer): 0x%.2x", poke_next_byte(contents));
  printf("\nsizeof(lua_Number): 0x%.2x", poke_next_byte(contents));

  // dumpInteger(D, LUAC_INT) used for detecting integer format mismatch
  sv = poke_next_bytes(contents, LUAC_INT_LEN);

  printf("\nLUAC_INT: ");
  for (size_t i = 0; i < sv.count; ++i) {
    if (i % 1 == 0 && i != 0) {
      printf(" ");
    }

    printf("0x%.2x", sv.data[i]);
  }

  // dumpNumber(D, LUAC_NUM) used for detecting floating format mismatch
  sv = poke_next_bytes(contents, LUAC_NUM_LEN);

  printf("\nLUAC_NUM: ");
  for (size_t i = 0; i < sv.count; ++i) {
    if (i % 1 == 0 && i != 0) {
      printf(" ");
    }

    printf("0x%.2x", sv.data[i]);
  }

  // dumpByte(&D, f->sizeupvalues) upvalues are equivalent of C variables
  // Used for Lua closures. READ: https://www.lua.org/pil/27.3.3.html
  printf("\nsizeof(upvalues): 0x%.2x\n", poke_next_byte(contents));
}
