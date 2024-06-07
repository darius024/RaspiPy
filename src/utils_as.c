#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "datatypes_as.h"
#include "utils_as.h"
#include "vector.h"

// Utility Tools For Assembler

// Decode 32-bit (0) or 64-bit mode (1)
int getMode(char *rd)
{
    return (*rd == 'x');
}

// Decode from hex or dec to int
int getInt(char *val)
{
    // Check if value is in hex or dec and parse it
    char *endptr;
    if (strcasechr(val, 'x') != NULL) {
        return (int)strtol(val, &endptr, 16); // hex
    }
    return (int)strtol(val, &endptr, 10);     // dec
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
    if (!strncasecmp(p, "sp", 2) || !strncasecmp(p, "zr", 2)) {
        return ZR_SP;
    }
    return atoi(p);
}

// Decode <operand>
int getOperand(char *op, bool rOrImm)
{
    // 0 for register, 1 for immediate
    return (rOrImm) ? getImmediate(op) : getRegister(op);
}

// Decode <literal>
int getLiteral(char *literal, vector *symtable)
{
    // Check for immediate value
    if (*literal == '#') {
        return atoi(literal + 1); // remove #
    }

    // Find the address of the label in the symbol table
    for (int i = 0; i < symtable->currentSize; i++) {
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
        if (strcasecmp(shift, shifts[i]) == 0) {
            return i;
        }
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
enum dpType getOpType(char **tokens)
{
    if (*tokens[2] == '#') {
        return imm;
    }
    return reg;
}

// Shifts all tokens from a position, inserting new token at vacant position
void insertNewToken(char **tokens, char *insertingToken, int numTokens, int index)
{
    assert(numTokens < NUM_TOKENS);
    for (int i = numTokens; i > index; i--) {
        strcpy(tokens[i], tokens[i - 1]);
    }
    strcpy(tokens[index], insertingToken);
}

int getPositionInArray(char *word, const char **words, int num)
{
    for (int i = 0; i < num; i++) {
        if (strcasecmp(word, words[i]) == 0) {
            return i;
        }
    }
    return NOT_FOUND;
}

bool checkWordInArray(char *word, const char **words, int num)
{
    return (getPositionInArray(word, words, num) != NOT_FOUND);
}

void setOnes(uint32_t *instruction, const int *bits, int num)
{
    for (int i = 0; i < num; i++) {
        *instruction |= 1 << bits[i];
    }
}

bool hasOpenBracket(char *token)
{
    return (strchr(token, '[') != NULL);
}

void putBits(uint32_t *instr, void *value, int start, int nbits)
{
    uint32_t mask = ((1 << nbits) - 1);
    if (nbits == 1) {
        *instr |= (*(bool *)value & mask) << start;
    } else if (nbits <= 8) {
        *instr |= (*(uint8_t *)value & mask) << start;
    } else if (nbits <= 16) {
        *instr |= (*(uint16_t *)value & mask) << start;
    } else {
        *instr |= (*(uint32_t *)value & mask) << start;
    }
}
