#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "datatypes_as.h"
#include "utils_as.h"


//
// Token Manipulation Helpers
//
// Decode 32-bit (0) or 64-bit mode (1)
int getMode(char *rd)
{
    return (*rd == 'x');
}

// Decode from hex or dec to int
int getInt(char *val)
{
    char *endptr;
    if (strchr(val, 'x') != NULL) {
        return (int)strtol(val, &endptr, 16); // hex
    }
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
    if (!strncasecmp(p, RSP, 2) || !strncasecmp(p, RZR, 2)) {
        return ZR_SP;
    }
    return atoi(p);
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
    for (int i = 0; i < SHIFTS_SIZE; i++) {
        if (strcasecmp(shift, shifts[i]) == 0) {
            return i;
        }
    }
    EXIT_PROGRAM("Shift mode not supported.");
}

//
// Type Decoding Helpers
//
// Retrieve the type of instruction of an assembly line
enum type identifyType(char *instrname)
{
    if (strchr(instrname, LABEL_ID) != NULL) { // Label
        return lb;
    }
    if (checkWordInArray(instrname, directive, DIRECTIVE_SIZE)) { // Directive
        return dir;
    }
    if (checkWordInArray(instrname, aliases, ALIASES_SIZE)) { // Alias
        return als;
    }
    if (checkWordInArray(instrname, branching, BRANCHING_SIZE)) { // Branching
        return b;
    }
    if (checkWordInArray(instrname, loadAndStore, LOAD_AND_STORE_SIZE)) { // Load / Stores
        return ls;
    }
    if (checkWordInArray(instrname, dataProcessing, DATA_PROCESSING_SIZE)) { // Data Processing
        return dp;
    }
    EXIT_PROGRAM("Unsupported instruction name (mnemonic).");
}

// Checks if operation includes an immediate value or register as operand
enum dpType getOpType(char **tokens, int numTokens)
{
    if (*tokens[2] == '#' || numTokens < 3 || strcasecmp(tokens[2], shifts[0]) == 0) {
        return imm;
    }
    return reg;
}

//
// Binary Handling Helpers
//
// Puts least nbits of value in instr at position start
void putBits(uint32_t *instr, void *value, int start, int nbits)
{
    uint32_t mask = ((1 << nbits) - 1);
    *instr &= ~(mask << start); // clear specified bits
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

//
// Array Insert / Look-Up Helpers
//
// Shifts all tokens from a position, inserting new token at vacant position
void insertNewToken(char **tokens, char *insertingToken, int *numTokens, int index)
{
    assert(*numTokens < NUM_TOKENS);
    for (int i = *numTokens; i > index; i--) {
        strcpy(tokens[i], tokens[i - 1]);
    }
    strcpy(tokens[index], insertingToken);
    (*numTokens)++;
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

//
// Malloc / Free struct
//
InstructionParse *initializeInstructionParse(void)
{
    InstructionParse *instr = (InstructionParse *)malloc(sizeof(InstructionParse));
    // Allocate memory for tokens
    for (int i = 0; i < NUM_TOKENS; i++) {
        instr->tokens[i] = (char *)malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        if (instr->tokens[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(instr->tokens[j]);
            }
            EXIT_PROGRAM("Failed to allocate space for tokens.");
        }
    }
    return instr;
}

void freeInstructionParse(InstructionParse *instructionParse)
{
    for (int i = 0; i < NUM_TOKENS; i++) {
        free(instructionParse->tokens[i]);
    }
    free(instructionParse);
}
