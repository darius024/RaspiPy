#include <stdio.h>

// Function Prototypes
void disassembleDP(char *instrname, char **tokens, int numTokens, FILE *outputFile);

void disassembleLS(char *instrname, char **tokens, int numTokens, FILE *outputFile, int PC);

void disassembleB(char *instrname, char *token, FILE *outputFile, int PC);
