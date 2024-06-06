#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Type declarations
typedef enum {
    regOp,
    immOp
} opType;

// Function Prototypes
extern int getMode(char *rd);

extern int getInt(char *val);

extern int getImmediate(char *imm);

extern int getRegister(char *rd);

extern int getOperand(char *op, bool *rOrImm);

extern int getLiteral(char *literal);

extern int getShift(char *shift);

extern int maskBetweenBits(int upp, int low);

extern opType getOpType(char **tokens, int numTokens);

#endif
