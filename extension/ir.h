#ifndef IR_H
#define IR_H

#include <stdint.h>
#include "state.h"

#define NUM_REGISTERS 31
#define RP 30 // return pointer
#define ZR 31
#define SP 32 // stack pointer
#define X0 0

/*
 * Register state - shared by the whole program
*/
extern int registers[NUM_REGISTERS];

/*
 * Program Intermediate Representation
 * - represented as a linked list of instructions
 * - each instruction has a type and fields
*/
typedef enum {
    IR_ADD, IR_SUB,
    // adds/subs can be derived
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

    IR_DIR
} IRType;

typedef enum {
    EQ, NE, GE, GT, LE, LT
} BranchConditional;

typedef enum {
    IMM, REG, LABEL
} TokenType;

typedef struct {
    uint8_t reg; // register
    TokenType type;
} Token;

typedef struct {
    IRType type; // mnemonic
    Token *dest; // 1st token
    Token *src1; // 2nd token
    Token *src2; // 3rd token
    Token *src3; // 4th token
    int line;    // instruction position
    int count;   // number of types executed
    struct IRInstruction *next;
} IRInstruction;

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
 *      : map of all hotspots
 * - each instruction has a type and fields
*/

IRProgram* create_ir_program(void);
void free_ir_program(IRProgram *program);

Token *create_token(uint8_t value);
IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line);
void free_ir_instruction(IRInstruction *instruction);

#endif
