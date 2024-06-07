#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "constants.h"
#include "datatypes_as.h"
#include "disassembler.h"
#include "instructions.h"
#include "onepass.h"
#include "utils_as.h"

#define CMP 0
#define CMN 1
#define NEG 2
#define NEGS 3
#define TST 4
#define MVN 5
#define MOV 6
#define MUL 7
#define MNEG 8

int disassembleDPI(InstructionParse *instr, Instruction *instruction)
{
    struct DPI *dpi = &(instruction->dpi);

    dpi->sf = getMode(instr->tokens[0]);
    dpi->rd = getRegister(instr->tokens[0]);

    int arithmPos = getPositionInArray(instr->instrname, arithmetics, SIZE_ARITHMETICS);
    if (arithmPos != NOT_FOUND) { // Arithmetics
        dpi->opc = arithmPos;
        dpi->opi = ARITHMETIC;
        dpi->rn = getRegister(instr->tokens[1]);
        dpi->imm12 = getImmediate(instr->tokens[2]);
        dpi->sh = (instr->numTokens > 3) ? (getImmediate(instr->tokens[4]) / ARITHMETIC_SHIFT) : 0;
    } else { // Wide Moves
        dpi->opc = getPositionInArray(instr->instrname, wideMoves, SIZE_WIDEMOVES);
        dpi->opi = WIDEMOVE;
        dpi->hw = (instr->numTokens > 2) ? (getImmediate(instr->tokens[3]) / WIDEMOVE_SHIFT) : 0;
        dpi->imm16 = getImmediate(instr->tokens[1]);
    }
    return EXIT_SUCCESS;
}

int disassembleDPR(InstructionParse *instr, Instruction *instruction)
{
    struct DPR *dpr = &(instruction->dpr);

    dpr->sf = getMode(instr->tokens[0]);
    dpr->rm = getRegister(instr->tokens[2]);
    dpr->rn = getRegister(instr->tokens[1]);
    dpr->rd = getRegister(instr->tokens[0]);

    int mulPos = getPositionInArray(instr->instrname, multiply, SIZE_MULTIPLY);
    if (mulPos != NOT_FOUND) { // Multiply
        dpr->m = 1;
        dpr->opr = DPR_MUL; // only this supported
        dpr->x = mulPos;
        dpr->ra = getRegister(instr->tokens[3]);
        return;
    }

    // Arithmetic / Bit-logic
    dpr->m = 0;
    dpr->shift = (instr->numTokens > 3) ? getShift(instr->tokens[3]) : 0;
    dpr->operand = (instr->numTokens > 3) ? getImmediate(instr->tokens[4]) : 0;

    int arithmPos = getPositionInArray(instr->instrname, arithmetics, SIZE_ARITHMETICS);
    if (arithmPos != NOT_FOUND) { // Arithmetic
        dpr->opc = arithmPos;
        dpr->armOrLog = 1;
        dpr->n = 0;
    } else { // Logical
        dpr->opc = getPositionInArray(instr->instrname, logical, SIZE_LOGICAL) / 2;
        dpr->armOrLog = 0;
        dpr->n = getPositionInArray(instr->instrname, logical, SIZE_LOGICAL) % 2;
    }
    return EXIT_SUCCESS;
}

// 2.3.2 Single Data Transfer Instructions
int disassembleSDT(InstructionParse *instr, Instruction *instruction, FILE *outputFile, int PC)
{
    struct SDT *sdt = &(instruction->sdt);

    sdt->sf = getMode(instr->tokens[0]);
    sdt->rt = getRegister(instr->tokens[0]);

    // Check address: register or literal
    if (hasOpenBracket(instr->tokens[1])) { // Single Data Transfer
        // Common features for all
        sdt->mode = 1;
        sdt->l = !strcasecmp(instr->instrname, "ldr");
        sdt->xn = getRegister(instr->tokens[1] + 1); // remove [

        if (instr->numTokens == 2) { // Zero Unsigned Offset
            sdt->u = 1;
            return;
        }

        // There is an immediate value or register provided
        char *cb2 = strchr(instr->tokens[2], ']');
        char *exM = strchr(instr->tokens[2], '!');
        if (cb2 != NULL && exM == NULL) {
            if (strchr(instr->tokens[2], '#') != NULL) { // Unsigned Immediate Offset
                sdt->u = 1;
                sdt->imm12 = getImmediate(instr->tokens[2]) / (sdt->mode ?  8 : 4);
            } else { // Register Offset
                sdt->offmode = 1;

                // Set 1s
                // TODO
                sdt->xm = getRegister(instr->tokens[2]);
            }
            return;
        }
        // Pre-Indexed & Post-Indexed
        sdt->offmode = 0;

        // Set 1s
        // TODO
        sdt->i = (cb2 != NULL);
        sdt->simm9 = getImmediate(instr->tokens[2]);
    } else { // Load Literal
        int literal = getLiteral(instr->tokens[1]);
        if (literal == INT32_MIN) {
            updateUndefTable(ll, instr->tokens[1]);
        }
        sdt->simm19 = (literal - PC * INSTR_BYTES) >> 2;
    }
    return EXIT_SUCCESS;
}

// 2.3.3 Branching Instructions
int disassembleB(InstructionParse *instr, Instruction *instruction, FILE *outputFile, int PC)
{
    struct B *b = &(instruction->b);

    if (!strcmp(instr->instrname, "b")) { // Unconditional
        b->type = 0;
        int literal = getLiteral(instr->tokens[0]);
        if (literal == INT32_MIN) {
            updateUndefTable(bu, instr->tokens[0]);
        }
        b->simm26 = (literal - PC * INSTR_BYTES) >> 2;
    } else if (!strcmp(instr->instrname, "br")) { // Register
        b->type = 3;
        // Set 1s
        // TODO
        b->xn = getRegister(instr->tokens[0]);
    } else { // Conditional
        b->type = 1;

        // Set 1s
        // TODO

        // Take by cases regarding the mnemonic
        char *p = instr->instrname + 2;
        switch (*p) {
            case 'e': // eq - 0000
                b->cond.tag = EQ_NE_TAG;
                b->cond.neg = EQ_NEG;
                break;
            case 'n': // ne - 0001
                b->cond.tag = EQ_NE_TAG;
                b->cond.neg = NE_NEG;
                break;
            case 'g': // ge - 1010 and gt - 1100
                b->cond.tag = (*(p + 1) == 'e') ? GE_LT_TAG : GT_LE_TAG;
                b->cond.neg = GE_NEG;
                break;
            case 'l': // lt - 1011 and le - 1101
                b->cond.tag = (*(p + 1) == 't') ? GE_LT_TAG : GT_LE_TAG;
                b->cond.neg = LT_NEG;
                break;
            case 'a': // al - 1110
                b->cond.tag = ALWAYS_TAG;
                b->cond.neg = ALWAYS_NEG;
                break;
            default:
                perror("Unrecognized conditional branch.\n");
                return EXIT_FAILURE;
        }
        int literal = getLiteral(instr->tokens[0]);
        if (literal == INT32_MIN) {
            updateUndefTable(bc, instr->tokens[0]);
        }
        b->simm19 = (literal - PC * INSTR_BYTES) >> 2;
    }
    return EXIT_SUCCESS;
}

// Disassemble Aliases
// Rephrase the instruction and delegate behaviour to the corresponding disassembler
int disassembleAlias(InstructionParse *instr, Instruction *instruction)
{
    // Get mode: 0 - w, 1 - x
    int mode = getMode(instr->tokens[0]);
    int idx = getPositionInArray(instr->instrname, aliases, SIZE_ALS);

    // Change type to dp
    instr->type = dp;
    
    strcpy(instr->instrname, aliasesName[idx]);

    if (idx == CMP || idx == CMN || idx == TST) {
        // Add rzr as 1st token - cmp, cmn, tst
        insertNewToken(instr->tokens, mode ? "xzr" : "wzr", instr->numTokens, 0);
    } else if (idx == NEG || idx == NEGS || idx == MVN || idx == MOV) {
        // Add rzr as 2nd token - neg, negs, mvn, mov
        insertNewToken(instr->tokens, mode ? "xzr" : "wzr", instr->numTokens, 1);
    } else {
        // Add rzr as 4th token - mul, mneg
        insertNewToken(instr->tokens, mode ? "xzr" : "wzr", instr->numTokens, 4);
    }
    return EXIT_SUCCESS;
}
