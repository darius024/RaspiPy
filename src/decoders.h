#include <stdint.h>

#include "structs.h"

// Function Pointers
typedef void (*BitFunc)(uint32_t *, void *, int, int);

// Prototypes
extern int decodeDPI(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeDPR(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeSDT(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decodeB(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
extern int decode(uint32_t *instr, Instruction *instruction, BitFunc bitFunc);
