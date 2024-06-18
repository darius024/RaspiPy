#include <stdint.h>
#include <stdlib.h>

#include "hotspot.h"
#include "ir.h"
#include "src/datatypes_as.h"
#include "src/decoders.h"
#include "src/disassembler.h"
#include "src/utils_as.h"

#define GENERATE_STRING(enum_type) #enum_type

extern HotMap hotspots[MAX_HOTSPOTS];
extern int hotspots_count;

// Functions to wokr with hotspots

char *getReg(int src) {
    char *reg = malloc(4 * sizeof(char));
    sprintf(reg, "w%d", src);
    return reg;
}

char *getImm(int src) {
    char *imm = malloc(10 * sizeof(char));
    sprintf(imm, "#%d", src);
    return imm;
}

uint32_t getHotSpot(IRInstruction *ir_instr)
{
    char assembly_line[MAX_ASSEMBLY_LINE] = getMnemonic(ir_instr->type);

    if (ir_instr->dest != NOT_USED) {
        strcat(assembly_line, " ");
        strcat(assembly_line, getReg(ir_instr->dest));
    }
    if (ir_instr->src1 != NOT_USED) {
        strcat(assembly_line, " ");
        strcat(assembly_line, getReg(ir_instr->src1));
    }
    if (ir_instr->src2 != NOT_USED) { // register or immediate value, depends on type
        strcat(assembly_line, " ");
        strcat(assembly_line, getReg(ir_instr->src2));
    }
    if (ir_instr->src3 != NOT_USED) { // register or immediate value, depends on type
        strcat(assembly_line, " ");
        strcat(assembly_line, getReg(ir_instr->src3));
    }

    InstructionParse *instr = initializeInstructionParse();
    Instruction *instruction = initializeInstruction();

    char *instrSavePntr = NULL;
    // Set the mnemonic and type of the instruction
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr);
    strcpy(instr->instrname, token);
    instr->type = identifyType(instr->instrname);

    // Take the tokens one by one
    instr->numTokens = 0;
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    while (token != NULL) {
        strcpy(instr->tokens[instr->numTokens++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }

    int disassembleError = disassemble(instr, instruction);
    checkError(disassembleError);
    
    if (instr->type == lb || instr->type == dir) return; // no decode
    uint32_t instrBin = 0;
    int decodeError = decode(&instrBin, instruction, putBits);
    checkError(decodeError);

    return instrBin;
}

void addHotSpot(uint32_t instrBin, int line)
{
    hotspots[hotspots_count].instruction = instrBin;
    hotspots[hotspots_count++].line = line;
}




// Executes the program and counts the number of times each line is executed
void executeProgram(IRProgram program)
{

}
