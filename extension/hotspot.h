#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <stdlib.h>
#include <stdint.h>

struct IRInstruction;
typedef struct IRInstruction IRInstruction;
struct IRProgram;
typedef struct IRProgram IRProgram;
struct Token;
typedef struct Token Token;

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
    B_EQ, B_NE, B_GE, B_GT, B_LE, B_LT
} BranchConditional;

typedef enum {
    IMM, REG, LABEL
} TokenType;

typedef struct Token {
    uint8_t reg; // register
    TokenType type;
} Token;

typedef struct IRInstruction {
    IRType type; // mnemonic
    Token *dest; // 1st token
    Token *src1; // 2nd token
    Token *src2; // 3rd token
    Token *src3; // 4th token
    int line;    // instruction position
    int count;   // number of types executed
    IRInstruction *next;
} IRInstruction;

typedef struct IRProgram {
    IRInstruction *head;
    IRInstruction *tail;
} IRProgram;

// Prototypes
uint32_t getHotSpot(IRInstruction *ir_instr);
uint32_t returnHotSpot(int line);
void addHotSpot(uint32_t instrBin, int line);

#endif
