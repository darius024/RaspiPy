#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Type declarations
typedef enum {
    imm, // immediate
    reg, // register
} dpType;
// Only the third token of each DP instruction is needed to identify this

// Function Prototypes
extern int getMode(char *rd);

extern int getInt(char *val);

extern int getImmediate(char *imm);

extern int getRegister(char *rd);

extern int getOperand(char *op, bool rOrImm);

extern int getLiteral(char *literal);

extern int getShift(char *shift);

extern int maskBetweenBits(int upp, int low);

extern dpType getOpType(char **tokens);

extern void insertNewToken(char **tokens, char *insertingToken, int numTokens, int index);

#endif
