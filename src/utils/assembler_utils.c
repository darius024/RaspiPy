#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "assembler_utils.h"
#include "../assemble.h"

extern struct symbolTable symtable[MAX_INSTRS];
extern int numLabel = 0;

// Utility Tools For Assembler

const char *shifts[] = {
    "lsl", "lsr", "asr", "ror"};

// Decode 32-bit (0) or 64-bit mode (1)
int getMode(char *rd)
{
    return *rd == 'x';
}

// Decode from int or hex to int
int getInt(char *val)
{
    // Check if value is in hex or dec and parse it
    char *endptr;
    if (strchr(val, 'x') != NULL)
        // Hex case
        return (int)strtol(val, &endptr, 16);
    // Dec case
    return (int)strtol(val, &endptr, 10);
}

// Read immediate value
int getImmediate(char *imm)
{
    return getInt(imm + 1); // remove #
}

// Decode register
int getRegister(char *rd)
{
    char *p = rd + 1;
    if (!strcmp(p, "sp") || !strcmp(p, "zr"))
        return 31;
    return atoi(p);
}

// Decode <operand>
int getOperand(char *op, bool *rOrImm)
{
    // 0 for register, 1 for immediate
    *rOrImm = (strchr(op, '#') != NULL);
    return (*rOrImm) ? getImmediate(op) : getRegister(op);
}

// Decode <literal>
int getLiteral(char *literal)
{
    // Check for immediate value
    if (*literal == '#')
        return atoi(literal + 1);
    // Find the address of the label in the symbol table
    for (int i = 0; i < numLabel; i++)
    {
        if (!strcmp(literal, symtable[i].label))
            return symtable[i].address;
    }
    // Assume that a label matches
    fprintf(stderr, "Symbol table is not complete. Label %s not supported.\n", literal);
    exit(EXIT_FAILURE);
}

// Decode <shift>
int getShift(char *shift)
{
    // Cases: lsl - 00, lsr - 01, asr - 10, ror - 11
    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(shift, shifts[i]))
            return i;
    }
    perror("Shift mode not supported.\n");
    exit(EXIT_FAILURE);
}

// Get a mask with 1s between the values (inclusive) and 0 everywhere else
int maskBetweenBits(int upp, int low)
{
    return ((1 << (upp + 1)) - 1) ^ ((1 << low) - 1);
}