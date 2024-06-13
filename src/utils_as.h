// Utility functions for the assembler

#ifndef UTILS_AS_H
#define UTILS_AS_H

#include <stdbool.h>
#include "vector.h"


// Prototypes
extern int getMode(char *rd);
extern int getInt(char *val);
extern int getImmediate(char *imm);
extern int getRegister(char *rd);
extern int getLiteral(char *literal, vector *symtable);
extern int getShift(char *shift);

extern enum type identifyType(char *instrname);
extern enum dpType getOpType(char **tokens, int numTokens);

extern void putBits(uint32_t *instr, void *value, int start, int nbits);

extern void insertNewToken(char **tokens, char *insertingToken, int *numTokens, int index);
extern int getPositionInArray(char *word,const char **words, int num);
extern bool checkWordInArray(char *word,const char **words, int num);

extern InstructionParse *initializeInstructionParse(void);
extern void freeInstructionParse(InstructionParse *instr);

#endif
