#ifndef IR_H
#define IR_H

#include <stdint.h>

#define NUM_REGISTERS 31
#define RP 30 // return pointer
#define ZR 31
#define SP 32 // stack pointer
#define X0 0

#define MAX_NAMES 16
#define MAX_VARS 64
#define MAX_FUNCS 16
#define MAX_LABELS 32
#define MAX_DIRECTIVES 32
#define NOT_USED -1
#define MAX_MOVE_VALUE (1 << 16)

#define MEMORY_CAPACITY (1024 * 1024 * 32)
#define STACK_OFFSET 2048
#define MAX_STACK_SIZE 32

#define MOVZ_MAX (1 << 21)

typedef struct State State;

typedef struct {
    char name[MAX_NAMES];
    int64_t value;
    uint8_t reg;
} Entry;

typedef struct {
    char name[MAX_NAMES];
    int64_t address;
    State *state;
} Func;

typedef struct {
    char name[MAX_NAMES];
    int64_t address;
} Label;

typedef struct {
    char name[MAX_NAMES];
    int64_t value;
} Directive;

struct State {
    int map_size;
    Entry **map;

    int funcs_size;
    Func **funcs;

    int symbol_table_size;
    Label **symbol_table;

    int directives_size;
    Directive **directives;

    int stack_size;
    int64_t stack_pointer;
    int64_t *stack;
};

/*
 * Program Intermediate Representation
 * - represented as a linked list of instructions
 * - each instruction has a type and fields
*/
typedef enum {
    IR_ADD, IR_SUB,
    IR_ADDS, IR_SUBS,
    IR_CMP,
    // cmn can be derived
    IR_NEG,
    // negs can be derived
    IR_AND, IR_EOR, IR_ORR,
    // ands, bic(s), eon, orn can be derived
    IR_TST,
    IR_MOV,
    IR_MOVZ,
    IR_MVN,
    // movn, movk can be derived
    IR_MADD,
    // msub, mneg can be derived
    IR_MUL,

    IR_B,
    IR_BR,
    IR_BCOND,

    IR_LDR,
    IR_STR,

    IR_DIR,
    IR_LABEL
} IRType;

typedef enum {
    B_EQ, B_NE, B_GE, B_GT, B_LE, B_LT
} BranchConditional;

typedef enum {
    IMM,    // immediate value
    REGX,   // register 64-bit
    REGW,   // register 32-bit
    BC,     // branch conditional type
    DIR,    // directive
    LABEL   // label address
} TokenType;

typedef struct {
    int value; // register, immediate, branch type, address
    TokenType type;
} Token;

typedef struct IRInstruction IRInstruction;

struct IRInstruction {
    IRType type; // mnemonic
    Token *dest; // 1st token
    Token *src1; // 2nd token
    Token *src2; // 3rd token
    Token *src3; // 4th token
    int line;    // instruction position
    int count;   // number of types executed
    IRInstruction *next;
};

typedef struct {
    IRInstruction *head;
    IRInstruction *tail;
} IRProgram;

/*
 * Program State
 * - includes:
 *      : map of all variables with their associate value and register
 *      : map of all function definitions and their address
 *      : map of all directives that should be stored in binary under program
 *      : stack
 * - each instruction has a type and fields
*/

IRProgram* create_ir_program(void);
void free_ir_program(IRProgram *program);

Token *create_token(uint8_t value);
IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line);
void free_ir_instruction(IRInstruction *instruction);

State *create_state(void);
void free_state(State *state);

#endif
