#ifndef INSTRUCTIONS_H__
#define INSTRUCTIONS_H__

#include <stdbool.h>

#include "file.h"

#define HEADER_SIGNATURE_LEN 4
#define LUAC_DATA_LEN 6
#define LUAC_INT_LEN 8
#define LUAC_NUM_LEN 8

#define OP_SIZE 7
#define A_SIZE 8
#define K_SIZE 1
#define B_SIZE 8
#define C_SIZE 8
#define Bx_SIZE 17
#define sBx_SIZE 17
#define Ax_SIZE 25
#define sJ_SIZE 25

#define MAXARG_C ((1 << C_SIZE) - 1)
#define MAXARG_sJ ((1 << sJ_SIZE) - 1)
#define OFFSET_sC (MAXARG_C >> 1)
#define OFFSET_sJ (MAXARG_sJ >> 1)
#define int2sC(i) ((i) + OFFSET_sC)
#define sC2int(i) ((i) - OFFSET_sC)

#define GET_A(t) ((t >> OP_SIZE) & 0xff)
#define GET_K(t) ((t >> (OP_SIZE + A_SIZE)) & 0x1)
#define GET_B(t) ((t >> (OP_SIZE + A_SIZE + K_SIZE)) & 0xff)
#define GET_C(t) ((t >> (OP_SIZE + A_SIZE + K_SIZE + B_SIZE)) & 0xff)
#define GET_Bx(t) ((t >> (OP_SIZE + A_SIZE)) & 0x1ffff)
#define GET_sBx(t) (((t >> (OP_SIZE + A_SIZE)) & 0x1ffff) - (((1 << Bx_SIZE) - 1) >> 1))
#define GET_Ax(t) ((t >> OP_SIZE) & 0x7FFFFF)
#define GET_sJ(t) (((t >> OP_SIZE) & 0x1ffffff) - OFFSET_sJ)
#define GET_sC(t) sC2int(GET_C(t))
#define GET_sB(t) sC2int(GET_B(t))
#define GET_k(t) (((t >> (OP_SIZE + A_SIZE)) & 0x1))
#define UPVALNAME(x) (((*p)->upvalues[x].name) ? (*p)->upvalues[x].name : "-")

/* Type tag of a TValue (bits 0-3 for tags + variant bits 4-5)
 * See lua/lua-5.4.7/src/lobject.h
 * */
#define rawtt(o) ((o)->tt_)
#define withvariant(t) ((t) & 0x3F)
#define ttypetag(o) withvariant(rawtt(o))

static const char *const opnames[] = {"MOVE", "LOADI", "LOADF", "LOADK", "LOADKX", "LOADFALSE", "LFALSESKIP",
    "LOADTRUE", "LOADNIL", "GETUPVAL", "SETUPVAL", "GETTABUP", "GETTABLE", "GETI", "GETFIELD", "SETTABUP", "SETTABLE",
    "SETI", "SETFIELD", "NEWTABLE", "SELF", "ADDI", "ADDK", "SUBK", "MULK", "MODK", "POWK", "DIVK", "IDIVK", "BANDK",
    "BORK", "BXORK", "SHRI", "SHLI", "ADD", "SUB", "MUL", "MOD", "POW", "DIV", "IDIV", "BAND", "BOR", "BXOR", "SHL",
    "SHR", "MMBIN", "MMBINI", "MMBINK", "UNM", "BNOT", "NOT", "LEN", "CONCAT", "CLOSE", "TBC", "JMP", "EQ", "LT", "LE",
    "EQK", "EQI", "LTI", "LEI", "GTI", "GEI", "TEST", "TESTSET", "CALL", "TAILCALL", "RETURN", "RETURN0", "RETURN1",
    "FORLOOP", "FORPREP", "TFORPREP", "TFORCALL", "TFORLOOP", "SETLIST", "CLOSURE", "VARARG", "VARARGPREP", "EXTRAARG",
    NULL};

/* From: https://www.lua.org/doc/jucs05.pdf "Section 7 The Virtual Machine"
 * R(X) means the Xth register.
 * K(X) means the Xth constant.
 * RK(X) means either R(X) or K(X-k), depending on the value of X — it is R(X) for values of X smaller than k (a build
 * parameter, typically 250). G[X] means the field X in the table of globals. U[X] means the Xth upvalue.
 * */

/*
 *         3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 *         1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * iABC          C(8)     |      B(8)     |k|     A(8)      |   Op(7)     |
 * iABx                Bx(17)               |     A(8)      |   Op(7)     |
 * iAsBx              sBx (signed)(17)      |     A(8)      |   Op(7)     |
 * iAx                           Ax(25)                     |   Op(7)     |
 * isJ                           sJ (signed)(25)            |   Op(7)     |
 * */

typedef enum {
    OP_MOVE,       /*	A B	R[A] := R[B]					*/
    OP_LOADI,      /*	A sBx	R[A] := sBx					*/
    OP_LOADF,      /*	A sBx	R[A] := (lua_Number)sBx				*/
    OP_LOADK,      /*	A Bx	R[A] := K[Bx]					*/
    OP_LOADKX,     /*	A	R[A] := K[extra arg]				*/
    OP_LOADFALSE,  /*	A	R[A] := false  */
    OP_LFALSESKIP, /*A	R[A] := false; pc++	(*)			*/
    OP_LOADTRUE,   /*	A	R[A] := true   */
    OP_LOADNIL,    /*	A B	R[A], R[A+1], ..., R[A+B] := nil */
    OP_GETUPVAL,   /*	A B	R[A] := UpValue[B]   */
    OP_SETUPVAL,   /*	A B	UpValue[B] := R[A]   */
    OP_GETTABUP,   /*	A B C	R[A] := UpValue[B][K[C]:shortstring] */
    OP_GETTABLE,   /*	A B C	R[A] := R[B][R[C]]   */
    OP_GETI,       /*	A B C	R[A] := R[B][C]					*/
    OP_GETFIELD,   /*	A B C	R[A] := R[B][K[C]:shortstring]   */
    OP_SETTABUP,   /*	A B C	UpValue[A][K[B]:shortstring] := RK(C) */
    OP_SETTABLE,   /*	A B C	R[A][R[B]] := RK(C)   */
    OP_SETI,       /*	A B C	R[A][B] := RK(C)				*/
    OP_SETFIELD,   /*	A B C	R[A][K[B]:shortstring] := RK(C)   */
    OP_NEWTABLE,   /*	A B C k	R[A] := {}   */
    OP_SELF,       /*	A B C	R[A+1] := R[B]; R[A] := R[B][RK(C):string]	*/
    OP_ADDI,       /*	A B sC	R[A] := R[B] + sC				*/
    OP_ADDK,       /*	A B C	R[A] := R[B] + K[C]:number			*/
    OP_SUBK,       /*	A B C	R[A] := R[B] - K[C]:number			*/
    OP_MULK,       /*	A B C	R[A] := R[B] * K[C]:number			*/
    OP_MODK,       /*	A B C	R[A] := R[B] % K[C]:number			*/
    OP_POWK,       /*	A B C	R[A] := R[B] ^ K[C]:number			*/
    OP_DIVK,       /*	A B C	R[A] := R[B] / K[C]:number			*/
    OP_IDIVK,      /*	A B C	R[A] := R[B] // K[C]:number			*/
    OP_BANDK,      /*	A B C	R[A] := R[B] & K[C]:integer			*/
    OP_BORK,       /*	A B C	R[A] := R[B] | K[C]:integer			*/
    OP_BXORK,      /*	A B C	R[A] := R[B] ~ K[C]:integer			*/
    OP_SHRI,       /*	A B sC	R[A] := R[B] >> sC				*/
    OP_SHLI,       /*	A B sC	R[A] := sC << R[B]				*/
    OP_ADD,        /*	A B C	R[A] := R[B] + R[C]				*/
    OP_SUB,        /*	A B C	R[A] := R[B] - R[C]				*/
    OP_MUL,        /*	A B C	R[A] := R[B] * R[C]				*/
    OP_MOD,        /*	A B C	R[A] := R[B] % R[C]				*/
    OP_POW,        /*	A B C	R[A] := R[B] ^ R[C]				*/
    OP_DIV,        /*	A B C	R[A] := R[B] / R[C]				*/
    OP_IDIV,       /*	A B C	R[A] := R[B] // R[C]				*/
    OP_BAND,       /*	A B C	R[A] := R[B] & R[C]				*/
    OP_BOR,        /*	A B C	R[A] := R[B] | R[C]				*/
    OP_BXOR,       /*	A B C	R[A] := R[B] ~ R[C]				*/
    OP_SHL,        /*	A B C	R[A] := R[B] << R[C]				*/
    OP_SHR,        /*	A B C	R[A] := R[B] >> R[C]				*/
    OP_MMBIN,      /*	A B C	call C metamethod over R[A] and R[B]	(*)	*/
    OP_MMBINI,     /*	A sB C k	call C metamethod over R[A] and sB	*/
    OP_MMBINK,     /*	A B C k		call C metamethod over R[A] and K[B]	*/
    OP_UNM,        /*	A B	R[A] := -R[B]					*/
    OP_BNOT,       /*	A B	R[A] := ~R[B]					*/
    OP_NOT,        /*	A B	R[A] := not R[B]				*/
    OP_LEN,        /*	A B	R[A] := #R[B] (length operator)			*/
    OP_CONCAT,     /*	A B	R[A] := R[A].. ... ..R[A + B - 1]		*/
    OP_CLOSE,      /*	A	close all upvalues >= R[A]			*/
    OP_TBC,        /*	A	mark variable A "to be closed"			*/
    OP_JMP,        /*	sJ	pc += sJ					*/
    OP_EQ,         /*	A B k	if ((R[A] == R[B]) ~= k) then pc++		*/
    OP_LT,         /*	A B k	if ((R[A] <  R[B]) ~= k) then pc++		*/
    OP_LE,         /*	A B k	if ((R[A] <= R[B]) ~= k) then pc++		*/
    OP_EQK,        /*	A B k	if ((R[A] == K[B]) ~= k) then pc++		*/
    OP_EQI,        /*	A sB k	if ((R[A] == sB) ~= k) then pc++		*/
    OP_LTI,        /*	A sB k	if ((R[A] < sB) ~= k) then pc++			*/
    OP_LEI,        /*	A sB k	if ((R[A] <= sB) ~= k) then pc++		*/
    OP_GTI,        /*	A sB k	if ((R[A] > sB) ~= k) then pc++			*/
    OP_GEI,        /*	A sB k	if ((R[A] >= sB) ~= k) then pc++		*/
    OP_TEST,       /*	A k	if (not R[A] == k) then pc++			*/
    OP_TESTSET,    /*	A B k	if (not R[B] == k) then pc++ else R[A] := R[B] (*) */
    OP_CALL,       /*	A B C	R[A], ... ,R[A+C-2] := R[A](R[A+1], ... ,R[A+B-1]) */
    OP_TAILCALL,   /*	A B C k	return R[A](R[A+1], ... ,R[A+B-1]) */
    OP_RETURN,     /*	A B C k	return R[A], ... ,R[A+B-2]	(see note)	*/
    OP_RETURN0,    /*		return    */
    OP_RETURN1,    /*	A	return R[A]    */
    OP_FORLOOP,    /*	A Bx	update counters; if loop continues then pc-=Bx; */
    OP_FORPREP,    /*	A Bx	<check values and prepare counters>; if not to run then pc+=Bx+1; */
    OP_TFORPREP,   /*	A Bx	create upvalue for R[A + 3]; pc+=Bx */
    OP_TFORCALL,   /*	A C	R[A+4], ... ,R[A+3+C] := R[A](R[A+1], R[A+2]); */
    OP_TFORLOOP,   /*	A Bx	if R[A+2] ~= nil then { R[A]=R[A+2]; pc -= Bx } */
    OP_SETLIST,    /*	A B C k	R[A][C+i] := R[A+i], 1 <= i <= B */
    OP_CLOSURE,    /*	A Bx	R[A] := closure(KPROTO[Bx])    */
    OP_VARARG,     /*	A C	R[A], R[A+1], ..., R[A+C-2] = vararg		*/
    OP_VARARGPREP, /*A	(adjust vararg parameters)			*/
    OP_EXTRAARG,   /*	Ax	extra (larger) argument for previous opcode */
    OP_NULL
} e_Opcodes;

/* Description of an upvalue for function prototypes */
typedef struct s_Upvalue_Desc {
    uint8_t instack; /* whether it is in stack (register) */
    uint8_t idx;     /* index of upvalue (in stack or in outer function's list) */
    uint8_t kind;    /* kind of corresponding variable */
    char *name;      /* upvalue name (for debug information) */
} s_Upvalue_Desc;

typedef struct s_AbsLineInfo {
    int pc;
    int line;
} s_AbsLineInfo;

typedef struct s_LocVar {
    int startpc; /* first point where variable is active */
    int endpc;   /* first point where variable is dead */
    char *varname;
} s_LocVar;

/* TODO Implement this properly
// Union of all Lua values
typedef union s_Value {
    struct GCObject *gc; // collectable objects
    void *p; 		// light userdata
    lua_CFunction f;     // light C functions
    lua_Integer i;       // integer numbers
    lua_Number n;        // float numbers
    // not used, but may avoid warnings for uninitialized value
    uint8_t ub;
} s_Value;

// Tagged Values. The basic representation of values in Lua: an actual value plus a tag with its type.
typedef struct s_TValue {
    // s_Value value_;
    uint8_t tt_;
} s_TValue;
*/

/* Function Prototypes
 * encapsulate all the necessary
 * data for each compiled function
 * */
typedef struct s_Func_Prototype {
    int sizep;
    int sizek;
    int sizeupvalues;
    int sizecode;
    int numparams;
    int sizelineinfo;
    int lastlinedefined;
    int linedefined;
    int sizeabslineinfo;
    int sizelocvars;
    int scopecount;
    uint8_t maxstacksize;
    bool nested;
    s_LocVar *locvars;
    uint8_t *lineinfo;
    char *source;                /* source file name used for debugging */
    struct s_Func_Prototype **p; /* functions defined inside the function */
    s_AbsLineInfo *abslineinfo;  /* idem */
    s_Upvalue_Desc *upvalues;    /* variables captured from an enclosing scope */
    // s_TValue *k;                 /* constants used by the function */
    uint32_t *code; /* all instructions are unsigned 32-bit integers. */
} s_Func_Prototype;

/* Match instruction opcodes and print the bytecode listing */
void match_instructions(s_Func_Prototype **prototype);

/* Parse the bytecode that luac dumps as a compiled chunk from the binary file:
 * https://www.lua.org/source/5.4/ldump.c.html#luaU_dump */
void parse_functions(s_Filebytes *file_bytes, s_Func_Prototype **prototype, bool is_nested, int scopecount);

/* Entry point for the parser */
void parse_hexdump(s_Filebytes *file_bytes, s_Func_Prototype **prototype);

#endif // INSTRUCTIONS_H__
