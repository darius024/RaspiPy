#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "disassembler.h"
#include "utils.h"
#include "assemble.h"

// Global keywords
//static const char *singleOperand[] = {
//     "mov", "mul", "mneg", "neg", "negs", "mvn", "movz", "movn", "movk"};
// static const char *twoOperandNoDest[] = {
//     "cmp", "cmn", "tst"};
static const char *wideMoves[] = {
    "movn", "", "movz", "movk"};
//static const char *multiply[] = {
//    "madd", "msub"};
static const char *arithmetics[] = {
    "add", "adds", "sub", "subs"};
static const char *logical[] = {
    "and", "bic", "orr", "orn", "eor", "eon", "ands", "bics"};
static const char *multiply[] = {
    "madd", "msub"};
static const char *aliases[] = {
    "cmp", "cmn", "neg", "negs", "tst", "mvn", "mov", "mul", "mneg"};

static const int dpiOnes[] = {28};
static const int dprOnes[] = {25, 27};
static const int lsOnes[] = {27, 28};
static const int sdtOnes[] = {29};
static const int sdtRegOnes[] = {11, 13, 14};
static const int bOnes[] = {26, 28};


// Function Prototype
//static void disassembleAliases(char *instrname, char **tokens, int numTokens, FILE *outputFile);

static void updateUndefTable(FILE *outputFile, undefType type, uint32_t instr, char *labelName)
{
	struct undefTable *newEntry = (struct undefTable *)malloc(sizeof(struct undefTable));
	newEntry -> offset = ftell(outputFile);
	newEntry -> type = type;
	newEntry -> instr = instr;
	strcpy(newEntry -> label, labelName);
	addToVector(undeftable, newEntry);
}

static void handleUndefLabel(FILE *outputFile, int i)
{
	struct undefTable *entry = (struct undefTable *)getFromVector(undeftable, i);
	uint32_t PC = entry->offset;
	uint32_t instruction = entry->instr;

    int literal = getLiteral(entry->label);
    int offset = (literal - PC) >> 2;

    switch (entry->type)
	{
    case ll: // Load Literal
    case bc: // Branch Conditional
        putBits(&instruction, &offset, 5, 19);
        break;
    case bu: // Branch Unconditional
        putBits(&instruction, &offset, 0, 26);
        break;
    }
    
    // Write to output file
    fseek(outputFile, entry->offset, SEEK_SET);
    fwrite(&instruction, sizeof(int), 1, outputFile);
}

// Iterates through vector to handle all the undefined lables
void handleUndefTable(FILE *outputFile)
{   
    for (int i = 0; i < undeftable->currentSize; i++) {
        handleUndefLabel(outputFile, i);
    }
}

int getPositionInArray(char *word, const char **words, int num)
{
    for (int i = 0; i < num; i++) {
        if (strcasecmp(word, words[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool checkWordInArray(char *word, const char **words, int num)
{
    return (getPositionInArray(word, words, num) != -1);
}

bool checkForAliases(char *instrname)
{
    return checkWordInArray(instrname, aliases, SIZE_ALIASES);
}

void setOnes(uint32_t *instruction, const int *bits, int num)
{
    for (int i = 0; i < num; i++) {
        *instruction |= 1 << bits[i];
    }
}

bool hasOpenBracket(char *token) {
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

void disassembleDPI(char *instrname, char **tokens, int numTokens, Instruction *instruction)
{
    DPI *dpi = &(instruction->dpi);

    // Set sf
    dpi->sf = getMode(tokens[0]);
    // Set rd
    dpi->rd = getRegister(tokens[0]);

    int arithmPos = getPositionInArray(instrname, arithmetics, SIZE_ARITHMETICS);
    if (arithmPos + 1) { // Arithmetics
        // Set opc
        dpi->opc = arithmPos;
        // Set opi
        dpi->opi = 2;
        // Set rn
        dpi->rn = getRegister(tokens[1]);
        // Set imm12
        dpi->imm12 = getImmediate(tokens[2]);
        // Set sh
        dpi->sh = (numTokens > 3) ? (getImmediate(tokens[4]) / 12) : 0;
    } else { // Wide Moves
        // Set opc
        dpi->opc = getPositionInArray(instrname, wideMoves, SIZE_WIDEMOVES);
        // Set opi
        dpi->opi = 5;
        // Set hw
        dpi->hw = (numTokens > 2) ? (getImmediate(tokens[3]) / 16) : 0;
        // Set imm16
        dpi->imm16 = getImmediate(tokens[1]);
    }
}

void disassembleDPR(char *instrname, char **tokens, int numTokens, Instruction *instruction)
{
    DPR *dpr = &(instruction->dpr);

    // Set sf
    dpr->sf = getMode(tokens[0]);
    // Set rm
    dpr->rm = getRegister(tokens[2]);
    // Set rn
    dpr->rn = getRegister(tokens[1]);
    // Set rd
    dpr->rd = getRegister(tokens[0]);

    int mulPos = getPositionInArray(instrname, multiply, SIZE_MULTIPLY);
    if (mulPos + 1) { // Multiply
        // Set m
        dpr->m = 1;
        // Set opr
        dpr->opr = 8; // only this supported
        // Set x
        dpr->x = mulPos;
        // Set ra
        dpr->ra = getRegister(tokens[3]);
        return;
    }

    // Arithmetic / Bit-logic
    // Set m
    dpr->m = 0;
    // Set shift
    dpr->shift = (numTokens > 3) ? getShift(tokens[3]) : 0;
    // Set operand
    dpr->operand = (numTokens > 3) ? getImmediate(tokens[4]) : 0;

    int arithmPos =  getPositionInArray(instrname, arithmetics, SIZE_ARITHMETICS);
    if (arithmPos + 1) { // Arithmetic
        // Set opc
        dpr->opc = arithmPos;
        // Set armOrLog
        dpr->armOrLog = 1;
        // Set n
        dpr->n = 0;
    } else { // Logical
        // Set opc
        dpr->opc = getPositionInArray(instrname, logical, SIZE_LOGICAL) / 2;
        // Set armOrLog
        dpr->armOrLog = 0;
        // Set n
        dpr->n = getPositionInArray(instrname, logical, SIZE_LOGICAL) % 2;
    }
}


// Assemble aliases
// Static because we don't call it explicitly
// static void disassembleAliases(char *instrname, char **tokens, int numTokens, Instruction *decoded)
// {
//     // 9 cases
//     // 0 - mov, 1 - cmn, 2(3) - neg(s), 4 - tst,
//     // 5 - mvn, 6 - mov, 7 - mul, 8 - mneg
//     // For each: set up tokens and delegate to disassembleDP again

//     // Get mode: w or x
//     int mode = getMode(tokens[0]);
//     int instr;
//     for (instr = 0; instr < 9; instr++)
//     {
//         if (!strcmp(instrname, aliases[instr]))
//             break;
//     }
//     // instr has the index of the mnemonic in aliases[]

//     // Add rzr as first token
//     // cmp, cmn, tst
//     if (instr <= 1 || instr == 4)
//     {
//         for (int i = numTokens; i > 0; i--)
//             strcpy(tokens[i], tokens[i - 1]);
//         strcpy(tokens[0], mode ? "xzr" : "wzr");
//         switch (instr)
//         {
//         case 0:
//             strcpy(instrname, "subs");
//             break;
//         case 1:
//             strcpy(instrname, "adds");
//             break;
//         case 4:
//             strcpy(instrname, "ands");
//             break;
//         }
//         disassembleDPR(instrname, tokens, ++numTokens, decoded);
//         return;
//     }

//     // Add rzr as second token
//     // neg, negs, mvn, mov
//     if (instr == 2 || instr == 3 || instr == 5 || instr == 6)
//     {
//         for (int i = numTokens; i > 1; i--)
//             strcpy(tokens[i], tokens[i - 1]);
//         strcpy(tokens[1], mode ? "xzr" : "wzr");
//         switch (instr)
//         {
//         case 2:
//             strcpy(instrname, "sub");
//             break;
//         case 3:
//             strcpy(instrname, "subs");
//             break;
//         case 5:
//             strcpy(instrname, "orn");
//             break;
//         case 6:
//             strcpy(instrname, "orr");
//             break;
//         }
//         disassembleDPR(instrname, tokens, ++numTokens, decoded);
//         return;
//     }

//     // Add rzr as last token
//     // mul, mneg
//     strcpy(tokens[numTokens], mode ? "xzr" : "wzr");
//     switch (instr)
//     {
//     case 7:
//         strcpy(instrname, "madd");
//         break;
//     case 8:
//         strcpy(instrname, "msub");
//         break;
//     }
//     disassembleDPR(instrname, tokens, ++numTokens, decoded);
// }

// 2.3.2 Single Data Transfer Instructions
void disassembleSDT(char *instrname, char **tokens, int numTokens, Instruction *instruction, FILE *outputFile, int PC)
{
    SDT *sdt = &(instruction->sdt);

    // Set sf
    sdt->sf = getMode(tokens[0]);
    // Set rt
    sdt->rt = getRegister(tokens[0]);

    // Check address: register or literal
    if (hasOpenBracket(tokens[1])) { // Single Data Transfer
        // Common features for all
        // Set mode
        sdt->mode = 1;
        // Set L: 1 -load, 0 - store
        sdt->l = !strcasecmp(instrname, "ldr");

        // Set xn
        sdt->xn = getRegister(tokens[1] + 1); // remove [ 

        if (numTokens == 2) { // Zero Unsigned Offset
            // Set U
            sdt->u = 1;
            return;
        }

        // There is an immediate value or register provided
        char *cb2 = strchr(tokens[2], ']');
        char *exM = strchr(tokens[2], '!');
        if (cb2 != NULL && exM == NULL) {
                if (strchr(tokens[2], '#') != NULL) { // Unsigned Immediate Offset
                    // Set U
                    sdt->u = 1;
                    // Set imm12
                    sdt->imm12 = getImmediate(tokens[2]) / (sdt->mode ? 8 : 4);
                }
                else { // Register Offset
                    // Set offmode
                    sdt->offmode = 1;

                    // Set 1s
                    // TODO
                    // Set xm
                    sdt->xm = getRegister(tokens[2]);

                }
                return;
        }
        // Pre-Indexed & Post-Indexed
        // Set offmode
        sdt->offmode = 0;

        // Set 1s
        // TODO
        // Set I: 1 - pre, 0 - post
        sdt->i = (cb2 != NULL);
        // Set simm9
        sdt->simm9 = getImmediate(tokens[2]);;
    }
    else { // Load Literal
        // Set simm19
        int literal = getLiteral(tokens[1]);    
        if (literal == INT32_MIN) {
            updateUndefTable(outputFile, ll, instruction, tokens[1]);
        } 
        sdt->simm19 = (literal - PC * 4) >> 2;
    }
}

// 2.3.3 Branching Instructions
void disassembleB(char *instrname, char *token, Instruction *instruction, FILE *outputFile, int PC)
{
    B *b = &(instruction->b); 

    if (!strcmp(instrname, "b")) { // Unconditional
        // Set type
        b->type = 0;
        // Set simm26
        int literal = getLiteral(token);
        if (literal == INT32_MIN) {
            updateUndefTable(outputFile, bu, instruction, token);
        }
        b->simm26 = (literal - PC * 4) >> 2;
    } else if (!strcmp(instrname, "br")) { // Register
        // Set type
        b->type = 3;
        // Set 1s
        // TODO

        // Set xn
        b->xn = getRegister(token);
    } else { // Conditional
        // Set type
        b->type = 1; 

        // Set 1s in the right places
        // TODO

        // Take by cases regarding the mnemonic
        char *p = instrname + 2;
        switch (*p) {
        case 'e': // eq - 0000
            b->cond.tag = 0;
            b->cond.neg = 0;
            break;
        case 'n': // ne - 0001
            b->cond.tag = 0;
            b->cond.neg = 1;
            break;
        case 'g': // ge - 1010 and gt - 1100
            b->cond.tag = (*(p + 1) == 'e') ? 5 : 6;
            b->cond.neg = 0;
            break;
        case 'l': // lt - 1011 and le - 1101
            b->cond.tag = (*(p + 1) == 't') ? 5 : 6;
            b->cond.neg = 1;
            break;
        case 'a': // al - 1110
            b->cond.tag = 7;
            b->cond.neg = 0;
            break;
        }
        // Set simm19
        int literal = getLiteral(token);
        if (literal == INT32_MIN) {
            updateUndefTable(outputFile, bc, instruction, token);
        }
        b->simm19 = (literal - PC * 4) >> 2;
    }
}

// 2.3.4 Special Instructions/Directives
// void disassembleDir(char *dir, int *instruction)
// {
//     int instruction = getInt(dir);
// }
