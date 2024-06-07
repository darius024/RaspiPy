#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "onepass.h"

#define ZR_SP 31
#define NOT_FOUND -1

// Prototypes
extern int getMode(char *rd);
extern int getInt(char *val);
extern int getImmediate(char *imm);
extern int getRegister(char *rd);
extern int getOperand(char *op, bool rOrImm);
extern int getLiteral(char *literal);
extern int getShift(char *shift);

extern int maskBetweenBits(int upp, int low);
extern enum dpType getOpType(char **tokens);

extern void insertNewToken(char **tokens, char *insertingToken, int numTokens, int index);

extern int getPositionInArray(char *word,const char **words, int num);
extern bool checkWordInArray(char *word,const char **words, int num);
extern bool checkForAliases(char *instrname);

extern void setOnes(uint32_t *instruction, const int *bits, int num);

extern void putBits(uint32_t *instr, void *value, int start, int nbits);

#endif
