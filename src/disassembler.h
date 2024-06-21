// Disassembler functions used to construct the structs out of the tokens

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "datatypes_as.h"
#include "structs.h"


// Prototypes
extern int disassemble(InstructionParse *instr, Instruction *instruction);
extern void addBinaryInstr(uint32_t instruction);

#endif
