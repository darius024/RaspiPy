// Emulator state structure

#ifndef DATATYPES_EM_H
#define DATATYPES_EM_H

#include <stdint.h>

#define MODE32 32
#define MODE64 64
#define BYTE_SIZE 8
#define MODE32_BYTES (MODE32 / BYTE_SIZE)
#define MODE64_BYTES (MODE64 / BYTE_SIZE)

#define MASK8 0xFFLL
#define MASK16 0xFFFFLL
#define MASK32 0xFFFFFFFFLL

#define HALT_INSTR 0x8a000000LL

// Emulator State
struct EmulatorState {
    int64_t R[NUM_OF_REGISTERS]; // Registers R0-R30
    int64_t ZR; // Zero Register
    int64_t PC; // Program Counter
    int64_t SP; // Stack Pointer
    struct PSTATE { // Processor State
        bool N; // Negative flag
        bool Z; // Zero flag
        bool C; // Carry flag
        bool V; // oVerflow flag
    } pstate;
    uint8_t mem[MEMORY_SIZE]; // Memory
};

#endif