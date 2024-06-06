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
static const int bOnes[] = {26, 28};


// Function Prototype
//static void disassembleAliases(char *instrname, char **tokens, int numTokens, FILE *outputFile);

static void updateLabelMap(FILE *outputFile, unDefTypes type, uint32_t instr, char *labelName)
{
	struct labelMap *newEntry = (struct labelMap *)malloc(sizeof(struct labelMap));
	newEntry -> offset = ftell(outputFile);
	newEntry -> type = type;
	newEntry -> instr = instr;
	strcpy(newEntry -> label, labelName);
	addToVector(unDefLables, newEntry);
}

static void handleUnDefLabel(FILE *outputFile, int idx)
{
	struct labelMap *entry = (struct labelMap *)getFromVector(unDefLables, idx);
    //uint32_t PC = unDefLables[idx].offset;
	uint32_t PC = entry -> offset;

    //fseek(outputFile, unDefLables[idx].offset, SEEK_SET);
    //int literal = getLiteral(unDefLables[idx].label);

	fseek(outputFile, entry -> offset, SEEK_SET);
    int literal = getLiteral(entry -> label);

    //uint32_t instruction = unDefLables[idx].instr;
	uint32_t instruction = entry -> instr;

    int offset = (literal - PC) >> 2;
    printf("lit: %d pc: %d", literal, PC);

    //switch (unDefLables[idx].type)
    switch (entry -> type)
	{
    case ll:
    case bc:
        instruction |= (offset << 5) & maskBetweenBits(23, 5);
        break;
    case bu:
        instruction |= offset & maskBetweenBits(25, 0);
        break;
    default:
        break;
    }
    fwrite(&instruction, sizeof(int), 1, outputFile);
}

//loops through vector to handle all the undefined lables
void handleUnDefLabels(FILE *outputFile)
{   
    for (int i = 0; i < unDefLables -> currentSize; i++)
    {
        handleUnDefLabel(outputFile, i);
    }
}
//go through array till you find index of word in array or return if not -1
int getPositionInArray(char *word,const char **words, int num)
{
    for (int i = 0; i < num; i++)
    {
        if (strcasecmp(word, words[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

bool checkWordInArray(char *word,const char **words, int num)
{
    return (getPositionInArray(word, words, num) != -1);
}

//check if the instruction name is an 
bool checkForAliases(char *instrname)
{
    return checkWordInArray(instrname, aliases, SIZE_ALIASES);
}

void setOnes(uint32_t *instruction,const int *bits, int num)
{
    for (int i = 0; i < num; i++)
    {
        *instruction |= 1 << bits[i];
    }
}

// puts 1s from start to start + n bits
void putBits(uint32_t *x, void *val, int start, int nbits)
{
    uint32_t mask = ((1 << nbits) - 1);

    // Check for type of *val
    if (nbits == 1)
    {
        *x |= (*(bool *)val & mask) << start;
    }
    else if (nbits <= 8)
    {
        *x |= (*(uint8_t *)val & mask) << start;
    }
    else if (nbits <= 16)
    {
        *x |= (*(uint16_t *)val & mask) << start;
    }
    else
    {
        *x |= (*(uint32_t *)val & mask) << start;
    }
}




// 2.3.1 Data Processing Instructions
// void disassembleDP(char *instrname, char **tokens, int numTokens, FILE *outputFile)
// {
//     // Start with xxxx...
//     int instruction = 0;

//     // Set sf
//     instruction |= getMode(tokens[0]) << 31;

//     // Broke down depending on the opcode

//     // Multiply - <mul_opcode> rd, rn, rm, ra
//     // Check that 4th token is a register
//     if (numTokens == 4 && strchr(tokens[3], '#') == NULL)
//     {
//         // tokens: 0 - rd, 1 - rn, 2 - rm, 3 - ra

//         // Set 1s
//         instruction |= (1 << 28) + (1 << 27) + (1 << 25) + (1 << 24);
//         // Set rm
//         instruction |= getRegister(tokens[2]) << 16;
//         // Set x: 0 - madd, 1 - msub
//         instruction |= !strcmp(instrname, "msub") << 15;
//         // Set ra
//         instruction |= getRegister(tokens[3]) << 10;
//         // Set rn
//         instruction |= getRegister(tokens[1]) << 5;
//         // Set rd
//         instruction |= getRegister(tokens[0]);

//         // instr[PC++] = instruction;
//         fwrite(&instruction, sizeof(int), 1, outputFile);
//         return;
//     }

//     // Two operands, no destination - <opcode> rn, <operand> - cmp, cmn, tst
//     bool isTwoOpNoDest = 0;
//     for (int i = 0; i < 3; i++)
//     {
//         if (!strcmp(instrname, twoOperandNoDest[i]))
//             isTwoOpNoDest = 1;
//     }
//     if (isTwoOpNoDest)
//     {
//         // These are all aliases
//         disassembleAliases(instrname, tokens, numTokens, outputFile);
//         return;
//     }

//     // Single operand - <opcode> rd, <operand> - mov, mul, mneg, neg(s), mvn, wide-moves
//     short isSingleOperand = -1;
//     for (int i = 0; i < 9; i++)
//     {
//         if (!strcmp(instrname, singleOperand[i]))
//             isSingleOperand = i;
//     }
//     if (isSingleOperand != -1)
//     {
//         if (isSingleOperand < 6)
//         {
//             // These are all aliases
//             disassembleAliases(instrname, tokens, numTokens, outputFile);
//             return;
//         }
//         else
//         {
//             // These are wide moves

//             // Set 1s
//             instruction |= 1 << 28;
//             // Set rd
//             instruction |= getRegister(tokens[0]);
//             // Set opi = 101
//             instruction |= (1 << 25) + (1 << 23);
//             // Set imm16
//             instruction |= getImmediate(tokens[1]) << 5;
//             // Set hw
//             if (numTokens > 2) // lsl #<imm> case
//                 instruction |= (getImmediate(tokens[3]) / 16) << 21;
//             // else hw = 0

//             // Set opc
//             // Cases: movn, movk, movz
//             if (strcmp(instrname, "movn") != 0) // movz, movk cases
//             {
//                 // Set opc to 10
//                 instruction |= 1 << 30;
//                 if (strcmp(instrname, "movk") == 0)
//                     instruction |= 1 << 29;
//             }
//             // instr[PC++] = instruction;
//             fwrite(&instruction, sizeof(int), 1, outputFile);
//             return;
//         }
//     }

//     // Two operand - <opcode> rd, rn, <operand> - arithmetic, bit-wise
//     if (!strcmp(instrname, "mul") || !strcmp(instrname, "mneg"))
//     {
//         // These are aliases
//         disassembleAliases(instrname, tokens, numTokens, outputFile);
//         return;
//     }

//     // Next:  add, adds, sub, subs (imm + reg)
//     // Next:  and, ands, bic, bics, eor, orr, eon, orn (reg)

//     // Common features for both immediate and register
//     // Set rd
//     instruction |= getRegister(tokens[0]);
//     // Set rn
//     instruction |= getRegister(tokens[1]) << 5;

//     // Get imm or rd
//     bool rOrImm;
//     int rOrImmValue = getOperand(tokens[2], &rOrImm);
//     // Check for <shift> #<imm>
//     int shiftType = 0;
//     int shiftValue = 0;
//     if (numTokens > 3) // shift exists case
//     {
//         shiftType = getShift(tokens[3]);
//         shiftValue = getImmediate(tokens[4]);
//     }

//     // Specific features for immediate and register
//     if (rOrImm)
//     {
//         // Immediate Case - Arithmetic only

//         // Set 1s
//         instruction |= 1 << 28;
//         // Set opi = 010
//         instruction |= 1 << 24;
//         // Set imm12
//         instruction |= rOrImmValue << 10;
//         // Set sh - is 1 when we shift by 12
//         instruction |= (shiftValue == 12) << 22;
//         // Set opc
//         for (int opc = 0; opc < 4; opc++)
//         {
//             if (!strcmp(instrname, arithmetics[opc]))
//                 instruction |= opc << 29;
//         }
//     }
//     else
//     {
//         // Register Case - Arithmetic + Logic

//         // Set 1s
//         instruction |= (1 << 27) + (1 << 25);
//         // Set rm
//         instruction |= getRegister(tokens[2]) << 16;

//         // arithmetic - 1, logic - 0
//         bool arithmOrLogic = false;
//         for (int opc = 0; opc < 4; opc++)
//         {
//             if (!strcmp(instrname, arithmetics[opc]))
//             {
//                 instruction |= opc << 29;
//                 arithmOrLogic = true;
//             }
//         }

//         // General for both arithmetic and logical
//         // Set opr
//         instruction |= arithmOrLogic << 24;
//         instruction |= shiftType << 22;
//         // Set operand
//         instruction |= shiftValue << 10;

//         // Arithmetic done
//         // Specific for logical
//         if (!arithmOrLogic)
//         {
//             // Set opc, N
//             for (int opc = 0; opc < 8; opc++)
//             {
//                 if (!strcmp(instrname, logical[opc]))
//                 {
//                     // opc = 00, 01, 10, 11
//                     instruction |= (opc / 2) << 29;
//                     // N = 0 or 1
//                     instruction |= (opc % 2) << 21;
//                 }
//             }
//         }
//     }

//     // instr[PC++] = instruction;
//     fwrite(&instruction, sizeof(int), 1, outputFile);
// }

void disassembleDPI(char *instrname, char **tokens, int numTokens, Decoded *instr)
{
    DPI *dpi = &(instr->dpimm);

    // Set 1s
    setOnes(&(instr->instruction), dpiOnes, SIZE_DPI);
    // Set sf
    dpi->sf = getMode(tokens[0]);
    // Set rd
    dpi->rd = getRegister(tokens[0]);


    // Arithmetics
    //position in arithmetic array if not there -1
    int arithPos = getPositionInArray(instrname, arithmetics, SIZE_ARITHMETICS);

    if (arithPos + 1)
    {
        // Set opc
        dpi->opc = arithPos;
        // Set opi
        dpi->opi = 2;
        // Set rn
        dpi->rn = getRegister(tokens[1]);
        // Set imm12
        dpi->imm12 = getImmediate(tokens[2]);
        // Set sh
        dpi->sh = (numTokens > 3) ? (getImmediate(tokens[4]) / 12) : 0;
    }
    // Wide Moves
    else
    {
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

void disassembleDPR(char *instrname, char **tokens, int numTokens, Decoded *instr)
{
    DPR *dpr = &(instr->dpr);

    // Set 1s
    setOnes(&(instr->instruction), dprOnes, SIZE_DPR);
    // Set sf
    dpr->sf = getMode(tokens[0]);
    // Set rm
    dpr->rm = getRegister(tokens[2]);
    // Set rn
    dpr->rn = getRegister(tokens[1]);
    // Set rd
    dpr->rd = getRegister(tokens[0]);

    // Multiply
    int mulPos = getPositionInArray(instrname, multiply, SIZE_MULTIPLY);

    if (mulPos + 1)
    {
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

    // Arithmetic
    int arithPos =  getPositionInArray(instrname, arithmetics, SIZE_ARITHMETICS);

    if (arithPos + 1)
    {
        // Set opc
        dpr->opc = arithPos;
        // Set armOrLog
        dpr->armOrLog = 1;
        // Set n
        dpr->n = 0;
    }
    // Logical
    else
    {
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
// static void disassembleAliases(char *instrname, char **tokens, int numTokens, Decoded *decoded)
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
void disassembleSDT(char *instrname, char **tokens, int numTokens, Decoded *instr, FILE *outputFile, int PC)
{
    // Start with xxx1100xxxx...
    uint32_t instruction = instr->instruction;
    
    //int instruction = (1 << 28) + (1 << 27);
    setOnes(&instruction, lsOnes, SIZE_LS);

    //define SDT
    SDT *sdt = &(instr -> sdt);//

    // Set rt
    instruction |= getRegister(tokens[0]);
    sdt->rt = getRegister(tokens[0]);//

    // Set sf
    int mode = getMode(tokens[0]);
    sdt->mode = getMode(tokens[0]);//

    instruction |= mode << 30;

    // Check address: register or literal
    if (*tokens[1] == '[')
    {
        // Single Data Transfer

        // Common features for all
        // Set 1s
        //instruction |= (1 << 31) + (1 << 29);
        setOnes(&instruction, sdtOnes, SIZE_SDT);

        // Set L: 1 - load, 0 - store
        instruction |= (!strcmp(instrname, "ldr")) << 22;
        sdt->l = !strcmp(instrname, "ldr");//

        char *cb1 = strchr(tokens[1], ']');
        if (cb1 != NULL)
            *cb1 = '\0'; // remove ] if necessary
        // Set xn
        instruction |= getRegister(tokens[1] + 1) << 5; // remove [
        sdt->xn = getRegister(tokens[1] + 1);    

        // Zero Unsigned Offset
        if (numTokens == 2)
        {
            // No imm provided
            // Set U
            instruction |= 1 << 24;
            sdt->u = 1;//

            // instr[PC++] = instruction;
            fwrite(&instruction, sizeof(int), 1, outputFile);
            return;
        }
        // numTokens = 3, so there is an immediate value or register provided
        char *cb2 = strchr(tokens[2], ']');
        char *exM = strchr(tokens[2], '!');
        if (cb2 != NULL && exM == NULL)
        {
                if (strchr(tokens[2], '#') != NULL)
                {
                    // Unsigned Immediate Offset
                    // Set U
                    instruction |= 1 << 24;
                    sdt->u = 1;//

                    // Set offset
                    int imm12 = getImmediate(tokens[2]) / (mode ? 8 : 4);
                    instruction |= imm12 << 10;
                    sdt->imm12 = imm12;//
                }
                else
                {
                    // Register Offset
                    sdt->offmode = 1;//

                    // Set 1s
                    instruction |= (1 << 21) + (1 << 14) + (1 << 13) + (1 << 11);
                    // Set xm
                    instruction |= getRegister(tokens[2]) << 16;
                    sdt->xm = getRegister(tokens[2]); //

                }
                // instr[PC++] = instruction;
                fwrite(&instruction, sizeof(int), 1, outputFile);
                return;
        }
        // Deal with Pre-Indexed after if, so pass through
        // Pre-Indexed & Post-Indexed
        sdt->offmode = 0;//

        // Set 1s
        instruction |= 1 << 10;
        // Set I: 1 - pre, 0 - posr
        instruction |= (cb2 != NULL) << 11;
        sdt->i = (cb2 != NULL); // 1 for pre-, 0 for post-

        // Set simm9
        int simm9 = getImmediate(tokens[2]);
        sdt->simm9 = simm9;

        // Apply mask because the value is signed
        instruction |= simm9 << 12 & maskBetweenBits(20, 12);
    }
    else
    {

        // Load Literal

        // Check for immediate value
        if (*tokens[1] == '#') 
        {
            sdt->simm19 = getImmediate(tokens[1]);
            return;
        }


        int literal = getLiteral(tokens[1]);    
        if (literal == -1)
        {
            updateLabelMap(outputFile, ll, instruction, tokens[1]);
            fwrite(&instruction, sizeof(int), 1, outputFile); // will be overwritten
            return;
        } 
    

        int offset = (literal - PC * 4) >> 2;
        sdt->simm19 = offset;

        instruction |= (offset << 5) & maskBetweenBits(23, 5);

    }
    // instr[PC++] = instruction;
    fwrite(&instruction, sizeof(int), 1, outputFile);
}

// 2.3.3 Branching Instructions
void disassembleB(char *instrname, char *token, Decoded *instr, FILE *outputFile, int PC)
{
    uint32_t instruction = instr->instruction;
    
    // Start with xx0101xxxx...
    //int instruction = (1 << 28) + (1 << 26);
    setOnes(&instruction, bOnes, SIZE_B);

    B *branch = &instr->br; 
    // Unconditional
    if (!strcmp(instrname, "b"))
    {
        //type uncondition = 0
        branch->type = 0;
        // Set simm26
        // Check for immediate value
        if (*token == '#') 
        {
            branch->simm26 = getImmediate(token);
            return;
        }

        int literal = getLiteral(token);
        if (literal == -1)
        {
            updateLabelMap(outputFile, bu, instruction, token);
            fwrite(&instruction, sizeof(int), 1, outputFile); // will be overwritten
            return;
        }
        int offset = (literal - PC * 4) >> 2;
        branch->simm26 = offset;

        instruction |= offset & maskBetweenBits(25, 0);
    }
    // Register
    else if (!strcmp(instrname, "br"))
    {
        //register type = 3
        branch->type = 3;
        // Set 1s in the right places
        instruction |= (1 << 31) + (1 << 30) + (1 << 25) + maskBetweenBits(20, 16);


        // Set xn
        branch->xn = getRegister(token);
        instruction |= getRegister(token) << 5;
    }
    // Conditional
    else
    {
        //conditional type = 1
        branch->type = 1; 

        // Set 1s in the right places
        instruction |= 1 << 30;

        // Take by cases regarding the mnemonic
        char *p = instrname + 2;
        switch (*p)
        {
        case 'e': // eq - 0000
            branch->cond = 0;
            break;
        case 'n': // ne - 0001
            instruction |= 1;
            branch->cond = 1;
            break;
        case 'g': // ge - 1010 and gt - 1100
            instruction |= (*(p + 1) == 'e')
                               ? 10
                               : 12;
            branch->cond = (*(p + 1) == 'e')
                               ? 10
                               : 12;
            break;
        case 'l': // lt - 1011 and le - 1101
            instruction |= (*(p + 1) == 't')
                               ? 11
                               : 13;
            branch->cond = (*(p + 1) == 'e')
                               ? 10
                               : 12;
            break;
        case 'a': // al - 1110
            instruction |= 14;
            branch->cond = 14;
            break;
        default:
            // Can't reach here
            break;
        }
        // Set simm19

        if (*token == '#') 
        {
            branch->simm19 = getImmediate(token);
            return;
        }

        int literal = getLiteral(token);
        if (literal == -1)
        {
            updateLabelMap(outputFile, bc, instruction, token);
            fwrite(&instruction, sizeof(int), 1, outputFile); // will be overwritten
            return;
        }

        int offset = (literal - PC * 4) >> 2;
        branch->simm19 = offset;
        instruction |= (offset << 5) & maskBetweenBits(23, 5);
    }
    // instr[PC++] = instruction;
    fwrite(&instruction, sizeof(int), 1, outputFile);
}

// 2.3.4 Special Instructions/Directives
void disassembleDir(char *dir, FILE *outputFile)
{
    int instruction = getInt(dir);
    fwrite(&instruction, sizeof(int), 1, outputFile);
}
