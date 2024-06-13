// Decoder functions used to transform between the binary and struct form of the instruction

#ifndef DECODERS_H
#define DECODERS_H

#include <stdint.h>
#include "structs.h"


// Function Pointer
typedef void (*BitFunc)(uint32_t *, void *, int, int);

// Prototypes
extern int decodeDPI(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeDPR(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeSDT(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeB(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decode(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);

#endif