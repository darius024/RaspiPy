#include <stdint.h>
#include <stdbool.h>

#define MASK32 0xFFFFFFFFLL

// Utilities for the emulator

void getBits(uint32_t *instr, void *value, int start, int nbits) {
    uint32_t mask = ((1 << nbits) - 1) << start;
    if (nbits == 1){
        *((bool *)value) = (*instr & mask) >> start;
    } else if (nbits <= 8) {
        *((uint8_t *)value) = (*instr & mask) >> start;
    } else if (nbits <= 16) {
        *((uint16_t *)value) = (*instr & mask) >> start;
    } else {
        *((int32_t *)value) = (*instr & mask) >> start;
    }
}

void signExtendTo32Bits(void *value, int nbits) {
    int p = (nbits <= 16) ? *((int16_t *)value) : *((int32_t *)value);
    if (p & (1 << (nbits - 1))) {
        int mask = MASK32 ^ ((1 << nbits) - 1);
        p |= mask;
    }
    if (nbits <= 16) {
        *((int16_t *)value) = p;
    } else {
        *((int32_t *)value) = p;
    }
}

void maskTo32Bits(bool sf, int64_t *reg) {
    if (sf == 0) {
        *reg &= MASK32;
    }
}