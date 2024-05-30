#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdio.h>

// Function Prototypes
extern void disassembleDP(char *instrname, char **tokens, int numTokens, FILE *outputFile);

extern void disassembleLS(char *instrname, char **tokens, int numTokens, FILE *outputFile, int PC);

extern void disassembleB(char *instrname, char *token, FILE *outputFile, int PC);

extern void disassembleDir(char *dir, FILE *outputFile);

extern const char *shifts[];

#endif