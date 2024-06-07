#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "assemble.h"
#include "utils.h"

#define ZR_SP 31

// Utility Tools For Assembler
static const char *shifts[] = {
    "lsl", "lsr", "asr", "ror"};

// Decode 32-bit (0) or 64-bit mode (1)
int getMode(char *rd)
{
    return (*rd == 'x');
}

// Decode from int or hex to int
int getInt(char *val)
{
    // Check if value is in hex or dec and parse it
    char *endptr;
    if (strcasechr(val, 'x') != NULL)
        return (int)strtol(val, &endptr, 16); // hex
    return (int)strtol(val, &endptr, 10); // dec
}

// Read immediate value
int getImmediate(char *imm)
{
    return getInt(imm + 1); // remove #
}

// Decode <register>
int getRegister(char *rd)
{
    char *p = rd + 1; // remove 'w' or 'x'
    if (!strncasecmp(p, "sp", 2) || !strncasecmp(p, "zr", 2))
        return ZR_SP;
    return atoi(p);
}

// Decode <operand>
int getOperand(char *op, bool rOrImm)
{
    // 0 for register, 1 for immediate
    return (rOrImm) ? getImmediate(op) : getRegister(op);
}

// Decode <literal>
int getLiteral(char *literal)
{
    // Check for immediate value
    if (*literal == '#')
        return atoi(literal + 1); // remove #
    
    // Find the address of the label in the symbol table
    for (int i = 0; i < symtable->currentSize; i++)
    {
		struct symbolTable *symtableEntry = getFromVector(symtable, i);
    	if (strcasecmp(literal, symtableEntry->label) == 0) {
			return symtableEntry->address;
		}
	}

    // Label not found
    return INT32_MIN; 
}

// Decode <shift>
int getShift(char *shift)
{
    // Cases: lsl - 00, lsr - 01, asr - 10, ror - 11
    for (int i = 0; i < 4; i++)
    {
        if (strcasecmp(shift, shifts[i]) == 0)
            return i;
    }
    perror("Shift mode not supported.\n");
    exit(EXIT_FAILURE);
}

// Get a mask with 1s between the parameters (inclusive) and 0 everywhere else
int maskBetweenBits(int upp, int low)
{
    return ((1 << (upp + 1)) - 1) ^ ((1 << low) - 1);
}

// Checks if operation includes an immediate value or register for operand
dpType getOpType(char **tokens) {
    if (*tokens[2] == '#') {
        return imm;
    }
    return reg;
}

// Shifts all tokens from a position, inserting new token at new vacant position
void insertNewToken(char **tokens, char *insertingToken, int numTokens, int index) {
    assert(numTokens <  NUM_TOKENS);
    for (int i = numTokens ; i > index ; i--) {
        strcpy(tokens[i], tokens[i - 1]);
    }
    char *newToken = (char *)malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
    strcpy(newToken, insertingToken);
    tokens[index] = newToken;
}
