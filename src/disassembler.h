#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "structs.h"

// Function Prototypes
extern void disassembleDPI(InstructionParse *instr, Instruction *instruction);

extern void disassembleDPR(InstructionParse *instr, Instruction *instruction);

extern void disassembleSDT(InstructionParse *instr, Instruction *instruction);

extern void disassembleB(InstructionParse *instr, Instruction *instruction);

extern void disassembleAlias(InstructionParse *instr, Instruction *instruction);

#endif