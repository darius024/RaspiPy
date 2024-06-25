// Utility functions for the emulator

#ifndef UTILS_EM_H
#define UTILS_EM_H

#include <stdbool.h>
#include <stdint.h>


// Prototypes
extern void getBits(uint32_t *instr, void *value, int start, int nbits);
extern void signExtendTo32Bits(void *value, int nbits);
extern void maskTo32Bits(bool sf, int64_t *reg);

#endif
