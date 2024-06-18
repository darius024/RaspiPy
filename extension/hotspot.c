#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "ast.h"
#include "hotspot.h"
#include "ir.h"
#include "src/datatypes_as.h"
#include "src/decoders.h"
#include "src/disassembler.h"
#include "src/utils_as.h"

#define GENERATE_STRING(enum_type) #enum_type

extern HotMap hotspots[MAX_HOTSPOTS];
extern int hotspots_count;

// Functions to work with hotspots

static void putMnemonic(char *assembly_line, IRType type)
{
    char mnemonic[MAX_TOKEN_LENGTH];
    strcpy(mnemonic, GENERATE_STRING(type) + 3);
    for (int i = 0; i < strlen(mnemonic); i++) {
        mnemonic[i] = tolower(mnemonic[i]);
    }
    strcat(assembly_line, mnemonic);
}

static void putReg(char *assembly_line, int src) {
    char reg[MAX_TOKEN_LENGTH];
    sprintf(reg, "w%d", src);
    strcat(assembly_line, " ");
    strcat(assembly_line, reg);
}

static void putImm(char *assembly_line, int src) {
    char imm[MAX_TOKEN_LENGTH];
    sprintf(imm, "#%d", src);
    strcat(assembly_line, " ");
    strcat(assembly_line, imm);
}

static void putRegOrImm(char *assembly_line, Token *token)
{
     if (token->reg != NOT_USED) {
        strcat(assembly_line, " ");
        if (token->type == REG) {
            putReg(assembly_line, token->reg);
        } else if (token->type == IMM) {
            putImm(assembly_line, token->reg);
        } else {
            // I have no idea. We shouldn't assemble lines with labels
        }
    }
}

static uint32_t getHotSpot(IRInstruction *ir_instr)
{
    char assembly_line[MAX_ASSEMBLY_LINE] = "";
    putMnemonic(assembly_line, ir_instr->type);
    putRegOrImm(assembly_line, ir_instr->dest);
    putRegOrImm(assembly_line, ir_instr->src1);
    putRegOrImm(assembly_line, ir_instr->src2);
    putRegOrImm(assembly_line, ir_instr->src3);

    InstructionParse *instr = initializeInstructionParse();
    Instruction *instruction = initializeInstruction();

    // Decompose
    char *instrSavePntr = NULL;
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr);
    strcpy(instr->instrname, token);
    instr->type = identifyType(instr->instrname);
    instr->numTokens = 0;
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    while (token != NULL) {
        strcpy(instr->tokens[instr->numTokens++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }

    // Disassemble
    int disassembleError = disassemble(instr, instruction);
    checkError(disassembleError);
    
    // Decode
    uint32_t instrBin = 0;
    int decodeError = decode(&instrBin, instruction, putBits);
    checkError(decodeError);

    // Return
    return instrBin;
}

static void addHotSpot(uint32_t instrBin, int line)
{
    hotspots[hotspots_count].instruction = instrBin;
    hotspots[hotspots_count++].line = line;
}

uint32_t returnHotSpot(int line)
{
    for (int i = 0; i < hotspots_count; i++) {
        if (hotspots[i].line == line) {
            return hotspots[i].instruction;
        }
    }
    exit(EXIT_FAILURE);
}


// Executes the program and counts the number of times each line is executed
void executeProgram(IRProgram *program)
{
    IRInstruction *instr = program->head;
    while (instr != NULL) {
        // Need to support IR_BR as well, not easy to do
        instr->count++;
        instr = instr->next;
    }
}
