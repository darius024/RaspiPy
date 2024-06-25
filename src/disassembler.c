#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes_as.h"
#include "disassembler.h"
#include "onepass.h"
#include "utils_as.h"


static int disassembleDPI(InstructionParse *instr, Instruction *instruction)
{
    instruction->instructionType = isDPI;
    struct DPI *dpi = &(instruction->dpi);

    dpi->sf = getMode(instr->tokens[0]);
    dpi->rd = getRegister(instr->tokens[0]);

    int arithmPos = getPositionInArray(instr->instrname, arithmetics, ARITHMETICS_SIZE);
    if (arithmPos != NOT_FOUND) { // Arithmetics
        dpi->opc = arithmPos;
        dpi->opi = ARITHMETIC;
        dpi->rn = getRegister(instr->tokens[1]);
        dpi->imm12 = getImmediate(instr->tokens[2]);
        dpi->sh = (instr->numTokens > NUM_EXISTS_SH) ? (getImmediate(instr->tokens[4]) / ARITHMETIC_SHIFT) : 0;
    } else { // Wide Moves
        dpi->opc = getPositionInArray(instr->instrname, wideMoves, WIDE_MOVES_SIZE);
        dpi->opi = WIDEMOVE;
        dpi->hw = (instr->numTokens > NUM_EXISTS_HW) ? (getImmediate(instr->tokens[3]) / WIDEMOVE_SHIFT) : 0;
        dpi->imm16 = getImmediate(instr->tokens[1]);
    }
    return EXIT_SUCCESS;
}

static int disassembleDPR(InstructionParse *instr, Instruction *instruction)
{
    instruction->instructionType = isDPR;
    struct DPR *dpr = &(instruction->dpr);

    dpr->sf = getMode(instr->tokens[0]);
    dpr->rm = getRegister(instr->tokens[2]);
    dpr->rn = getRegister(instr->tokens[1]);
    dpr->rd = getRegister(instr->tokens[0]);

    int mulPos = getPositionInArray(instr->instrname, multiply, MULTIPLY_SIZE);
    if (mulPos != NOT_FOUND) { // Multiply
        dpr->opc = DPR_OPC;
        dpr->m = 1;
        dpr->opr = DPR_MUL; // only this supported
        dpr->x = mulPos;
        dpr->ra = getRegister(instr->tokens[3]);
        return EXIT_SUCCESS;
    }

    // Arithmetic / Bit-logic
    dpr->m = 0;
    dpr->shift = (instr->numTokens > NUM_EXISTS_SH) ? getShift(instr->tokens[3]) : 0;
    dpr->operand = (instr->numTokens > NUM_EXISTS_SH) ? getImmediate(instr->tokens[4]) : 0;

    int arithmPos = getPositionInArray(instr->instrname, arithmetics, ARITHMETICS_SIZE);
    if (arithmPos != NOT_FOUND) { // Arithmetic
        dpr->opc = arithmPos;
        dpr->armOrLog = 1;
        dpr->n = 0;
    } else { // Logical
        int logPos = getPositionInArray(instr->instrname, logical, LOGICAL_SIZE);
        dpr->opc = logPos / OPC_DIV;
        dpr->armOrLog = 0;
        dpr->n = logPos % N_MOD;
    }
    return EXIT_SUCCESS;
}

// 2.3.2 Single Data Transfer Instructions
static int disassembleSDT(InstructionParse *instr, Instruction *instruction)
{
    instruction->instructionType = isSDT;
    struct SDT *sdt = &(instruction->sdt);

    sdt->sf = getMode(instr->tokens[0]);
    sdt->rt = getRegister(instr->tokens[0]);

    // Check address: register or literal
    if (strchr(instr->tokens[1], '[') != NULL) { // Single Data Transfer
        sdt->mode = 1;
        sdt->l = getPositionInArray(instr->instrname, loadAndStore, LOAD_AND_STORE_SIZE);
        sdt->xn = getRegister(instr->tokens[1] + 1); // remove [

        if (instr->numTokens == 2) { // Zero Unsigned Offset
            sdt->u = 1;
            sdt->imm12 = 0;
            return EXIT_SUCCESS;
        }

        // There is an immediate value or register provided
        char *cb2 = strchr(instr->tokens[2], ']');
        char *exM = strchr(instr->tokens[2], '!');
        if (cb2 != NULL && exM == NULL) {
            if (strchr(instr->tokens[2], '#') != NULL) { // Unsigned Immediate Offset
                sdt->u = 1;
                sdt->imm12 = getImmediate(instr->tokens[2]) / (sdt->mode ?  SDT_IMM12_64 : SDT_IMM12_32);
            } else { // Register Offset
                sdt->u = 0;
                sdt->offmode = 1;
                sdt->roff1 = SDT_ROFF1;
                sdt->roff2 = SDT_ROFF2;
                sdt->xm = getRegister(instr->tokens[2]);
            }
            return EXIT_SUCCESS;
        }
        // Pre-Indexed & Post-Indexed
        sdt->u = 0;
        sdt->offmode = 0;
        sdt->i = (exM != NULL);
        sdt->simm9 = getImmediate(instr->tokens[2]);
        sdt->bit = SDT_BIT;
    } else { // Load Literal
        sdt->mode = 0;
        int literal = getLiteral(instr->tokens[1], symtable);
        if (literal == INT32_MIN) {
            updateUndefTable(ll, instr->tokens[1]);
        }
        sdt->simm19 = (literal - PC * INSTR_BYTES) / INSTR_BYTES;
    }
    return EXIT_SUCCESS;
}

// 2.3.3 Branching Instructions
static int disassembleB(InstructionParse *instr, Instruction *instruction)
{
    instruction->instructionType = isB;
    struct B *b = &(instruction->b);

    if (!strcmp(instr->instrname, "b")) { // Unconditional
        b->type = BRANCH_UNCONDITIONAL;
        int literal = getLiteral(instr->tokens[0], symtable);
        if (literal == INT32_MIN) {
            updateUndefTable(bu, instr->tokens[0]);
        }
        b->simm26 = (literal - PC * INSTR_BYTES) / INSTR_BYTES;
    } else if (!strcmp(instr->instrname, "br")) { // Register
        b->type = BRANCH_REGISTER;
        b->bit = B_BIT;
        b->reg = B_REG;
        b->xn = getRegister(instr->tokens[0]);
    } else { // Conditional
        b->type = BRANCH_CONDITIONAL;

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
                EXIT_PROGRAM("Unrecognized conditional branch.");
        }
        int literal = getLiteral(instr->tokens[0], symtable);
        if (literal == INT32_MIN) {
            updateUndefTable(bc, instr->tokens[0]);
        }
        b->simm19 = (literal - PC * INSTR_BYTES) / INSTR_BYTES;
    }
    return EXIT_SUCCESS;
}

// Disassemble Aliases
// Rephrase the instruction and delegate behaviour to the corresponding disassembler
static int disassembleAlias(InstructionParse *instr, Instruction *instruction)
{
    int mode = getMode(instr->tokens[0]);
    int idx = getPositionInArray(instr->instrname, aliases, ALIASES_SIZE);

    instr->type = dp;
    strcpy(instr->instrname, aliasesName[idx]);
    if (idx == CMP || idx == CMN || idx == TST) {
        // Add rzr as 1st token - cmp, cmn, tst
        insertNewToken(instr->tokens, mode ? XZR : WZR, &(instr->numTokens), 0);
    } else if (idx == NEG || idx == NEGS || idx == MVN || idx == MOV) {
        // Add rzr as 2nd token - neg, negs, mvn, mov
        insertNewToken(instr->tokens, mode ? XZR : WZR, &(instr->numTokens), 1);
    } else {
        // Add rzr as 4th token - mul, mneg
        insertNewToken(instr->tokens, mode ? XZR : WZR, &(instr->numTokens), 3);
    }
    return EXIT_SUCCESS;
}

// Labels and Directives
// Update the symbol table when encountering new label
static void updateSymbolTable(InstructionParse *instr)
{
    char *p = strchr(instr->instrname, ':');
    if (p != NULL) { // Label case
        *p = '\0';
		struct symbolTable *newEntry = malloc(sizeof(struct symbolTable));
        assert(newEntry != NULL);
		strcpy(newEntry->label, instr->instrname);
		newEntry->address = PC * INSTR_BYTES;
		addToVector(symtable, newEntry);
	}
}

// Record binary instruction after being parsed
void addBinaryInstr(uint32_t instruction)
{
    binaryInstr[PC++] = instruction;
}

// Construct a specific data structure out of the tokens based on instruction type
int disassemble(InstructionParse *instr, Instruction *instruction)
{
    // Sends instruction to corresponding disassembler
    switch (instr->type) {
        case lb:
            updateSymbolTable(instr);
            return EXIT_SUCCESS;
        case dir:
            addBinaryInstr(getInt(instr->tokens[0]));
            return EXIT_SUCCESS;
        case als:
            return (disassembleAlias(instr, instruction)) ? EXIT_FAILURE : disassemble(instr, instruction);
        case b:
            return disassembleB(instr, instruction);
        case ls:
            return disassembleSDT(instr, instruction);
        case dp:
            return (getOpType(instr->tokens, instr->numTokens) == imm)
                ? disassembleDPI(instr, instruction)
                : disassembleDPR(instr, instruction);
        default:
            EXIT_PROGRAM("Unsupported instruction type.");
    }
}
