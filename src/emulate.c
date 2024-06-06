#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "instruction.h"
#include "emulate.h"

#define MEMORY_SIZE (2 * 1024 * 1024) // 2MB
#define INSTR_BYTES 4
#define BYTE_SIZE 8
#define MODE32 32
#define MODE64 64
#define MODE32_BYTES (MODE32 / BYTE_SIZE)
#define MODE64_BYTES (MODE64 / BYTE_SIZE)

#define MASK8 0xFFLL
#define MASK16 0xFFFFLL
#define MASK32 0xFFFFFFFFLL

#define HALT 0x8a000000LL

#define ZR_SP 31
#define NUM_OF_REGISTERS 31

// Declarations
extern int64_t shift(int64_t, int8_t, uint8_t, bool);
extern uint32_t fetch(uint32_t);
extern void updateFlagsArithmetic(int64_t a, int64_t b, bool sf, bool isAdd);
extern void checkErrors(bool);

// Function Pointers
typedef void (*BitFunc)(uint32_t, void *, int, int);

// Memory
uint8_t mem[MEMORY_SIZE];

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
};
struct EmulatorState state = {0};

// Categories of instructions to support
typedef enum {
    isDPI, // Data processing immediate
    isDPR, // Data processing register
    isSDT, // Single data transfer
    isBranch, // Branch
} InstructionType;

// data processing immediate
struct DPI {
    bool sf; // bit-width: 0 for 32-bit, 1 for 64-bit
    uint8_t opc; // opcode
    uint8_t opi; // arithmetic or wide move
    union {
        struct { // arithmetic
            bool sh; // 12-bit left shift of imm12
            uint16_t imm12; // 1st operand: immediate
            uint8_t rn; // 2nd operand: register
        };
        struct {
            uint8_t hw; // (hw * 4) left shift of imm16
            uint16_t imm16; // operand
        }; // widemove
    };
    uint8_t rd; // Destination register
};

// data processing register
struct DPR {
    bool sf; // bit-width: 0 for 32-bit, 1 for 64-bit
    uint8_t opc; // opcode
    bool m; // multiply flag
    struct { // operation: arithmetic, bit-logic, multiply
        bool armOrLog; // 0 for logical, 1 or arithmetic
        uint8_t shift;
        bool n;
    };
    uint8_t rm;
    union {
        uint8_t operand; // arithmetic, bit-logic
        struct { // multiply
            bool x;
            uint8_t ra;
        };
    };
    uint8_t rn;
    uint8_t rd; // destination register
};

// single data transfer
struct SDT {
    bool mode; // 1 for single data transfer, 0 for load literal
    bool sf; // load size: 0 for 32-bit, 1 for 64-bit
    union {
        struct {
            bool u; // unsigned offset flag
            bool l; // type of data transfer
            bool offmode; // 1 for register offset, 0 for pre/post-index
            union {
                uint8_t xm;
                struct {
                    int16_t simm9;
                    bool i;
                };
                uint16_t imm12;
            };
            uint8_t xn;
        };
        int32_t simm19;
    };
    uint8_t rt;
};

// branch
struct Branch {
    uint8_t type; // 0 - unconditional, 1 - conditional, 3 - register
    bool nop; // 0 - nop, 1 - reg
    union {
        int32_t simm26;
        uint8_t xn;
        struct {
            int32_t simm19;
            uint8_t condTag;
            bool condInvert;
        };
    };
};

typedef struct {
    InstructionType instructionType;
    union {
        struct DPI dpimm;
        struct DPR dpr;
        struct SDT sdt;
        struct Branch br;
    };
} Instruction;

//
// Utility Functions
//

void initializeState(void) {
    state.pstate.Z = true;
}

void loadBinaryFile(const char *filename) {
    const char *extension = strrchr(filename, '.');
    if (!extension || strcmp(extension + 1, "bin") != 0) {
        fprintf(stderr, "This is not a valid binary file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    FILE *file;
    file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open input file.");
        exit(EXIT_FAILURE);
    }

    size_t numberOfBytes = fread(mem, 1, MEMORY_SIZE, file);

    if (numberOfBytes == 0) {
        perror("The file is empty.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void writeOutput(const char *filename) {
    FILE *file;
    file = (!strcmp(filename, "stdout")) ? stdout : fopen(filename, "w");
    if (!file) {
        perror("Could not open output file.\n");
        exit(EXIT_FAILURE);
    }

    const char *extension = strrchr(filename, '.');
    if (file != stdout && (!extension || strcmp(extension + 1, "out") != 0)) {
        fprintf(stderr, "This is not a valid .out file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Registers:\n");
    for (int i = 0; i < 31; i++) {
        fprintf(file, "X%d%d    = %016lx\n", i > 9 ? i / 10 : 0, i % 10, state.R[i]);
    }

    fprintf(file, "PC     = %016lx\n", state.PC);
    fprintf(file, "PSTATE : %c%c%c%c",
            state.pstate.N ? 'N' : '-',
            state.pstate.Z ? 'Z' : '-',
            state.pstate.C ? 'C' : '-',
            state.pstate.V ? 'V' : '-');

    fprintf(file, "\nNon-Zero Memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += 4) {
        uint32_t binInstr = fetch(addr);
        if (binInstr != 0) {
            fprintf(file, "0x%08x : %08x\n", addr, binInstr);
        }
    }

    if (file != stdout) {
        fclose(file);
    }
}

//
// Binary Manipulation Helpers
//

void getBits(uint32_t x, void *destination, int start, int nbits) {
    uint32_t mask = ((1 << nbits) - 1) << start;
    if (nbits == 1){
        *((bool *)destination) = (x & mask) >> start;
    } else if (nbits <= 8) {
        *((uint8_t *)destination) = (x & mask) >> start;
    } else if (nbits <= 16) {
        *((uint16_t *)destination) = (x & mask) >> start;
    } else {
        *((int32_t *)destination) = (x & mask) >> start;
    }
}

void signExtendTo32Bits(void *val, int nbits) {
    int p = (nbits < 16) ? *((int16_t *)val) : *((int32_t *)val);

    if (p & (1 << (nbits - 1))) {
        // The number is negative
        int mask = MASK32 ^ ((1 << nbits) - 1);
        p |= mask;
    }

    // Check for type of *val
    if (nbits < 16) {
        *((int16_t *)val) = p;
    } else {
        *((int32_t *)val) = p;
    }
}

void maskTo32Bits(bool sf, int64_t *reg) {
    if (sf == 0) {
        *reg &= MASK32;
    }
}

// Used in DPI and DPR

void addOrSub(uint8_t opc, uint8_t rd, bool sf, int64_t *Rd, int64_t a, int64_t b) {
    switch (opc) {
        case 0: // Add
            *Rd = a + b;
            break;
        case 1: // Add and set flags
            if (rd != ZR_SP) {
                *Rd = a + b;
            }
            updateFlagsArithmetic(a, b, sf, true);
            break;
        case 2: // Subtract
            *Rd = a - b;
            break;
        case 3: // Subtract and set flags
            if (rd != ZR_SP) {
                *Rd = a - b;
            }
            updateFlagsArithmetic(a, b, sf, false);
            break;
        default:
            perror("Unsupported opc of data processing operation.");
            exit(EXIT_FAILURE);
    }
}

// 1.4 Data Processing Instruction (Immediate)

int decodeDPImm(uint32_t instr, Instruction *instruction, BitFunc bitFunc) {
    instruction->instructionType = isDPI;
    struct DPI *dpimm = &(instruction->dpimm);
    bitFunc(instr, &(dpimm->sf), 31, 1);
    bitFunc(instr, &(dpimm->opc), 29, 2);
    bitFunc(instr, &(dpimm->opi), 23, 3);
    bitFunc(instr, &(dpimm->rd), 0, 5);
    
    // Type of data processing operation: arithmetic - 010, widemove - 101
    switch (dpimm->opi) {
        case 2: // opi = 010
            bitFunc(instr, &(dpimm->sh), 22, 1);
            bitFunc(instr, &(dpimm->imm12), 10, 12);
            bitFunc(instr, &(dpimm->rn), 5, 5);
            break;
        case 5: // opi = 101
            bitFunc(instr, &(dpimm->hw), 21, 2);
            bitFunc(instr, &(dpimm->imm16), 5, 16);
            break;
        default: // unsupported opi
            perror("Unsupported opi (bits 23-25), use either 010 or 101.\n");
            return 1;
    }
    return 0;
}

void updateFlagsArithmetic(int64_t a, int64_t b, bool sf, bool isAdd) {
    int64_t res = isAdd ? a + b : a - b;

    // Sign Flag (N)
    state.pstate.N = sf ? (res < 0) : ((int32_t)res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = isAdd ? (sf ? ((uint64_t)res < (uint64_t)a)
                                 : ((uint32_t)res < (uint32_t)a))
                           : (sf ? ((uint64_t)a >= (uint64_t)b)
                                 : ((uint32_t)a >= (uint32_t)b));
    // Overflow Flag (V)
    state.pstate.V = isAdd ? (sf ? (((a > 0) == (b > 0)) && ((res > 0) != (a > 0)))
                                 : ((((int32_t)a > 0) == ((int32_t)b > 0)) && (((int32_t)res > 0) != ((int32_t)a > 0))))
                           : (sf ? (((a > 0) != (b > 0)) && ((res > 0) == (b > 0)))
                                 : ((((int32_t)a > 0) != ((int32_t)b > 0)) && (((int32_t)res > 0) == ((int32_t)b > 0))));
}

int executeDPImm(Instruction instruction) {
    struct DPI dpimm = instruction.dpimm;
    int64_t *Rd = (dpimm.rd == ZR_SP) ? &state.SP : &state.R[dpimm.rd];

    switch (dpimm.opi) {
        case 2: { // Arithmetic
            int64_t imm12 = ((int64_t)dpimm.imm12) << (12 * dpimm.sh);
            int64_t Rn = (dpimm.rn == ZR_SP) ? state.SP : state.R[dpimm.rn];
            maskTo32Bits(dpimm.sf, &Rn);
            addOrSub(dpimm.opc, dpimm.rd, dpimm.sf, Rd, Rn, imm12);
            break;
        }
        case 5: { // Wide Move
            if (dpimm.rd == ZR_SP) {
                break;
            }
            uint64_t imm16 = ((uint64_t)dpimm.imm16) << (dpimm.hw * 16);
            switch (dpimm.opc) {
                case 0: // Move wide with NOT
                    *Rd = ~imm16;
                    break;
                case 2: // Move wide with zero
                    *Rd = imm16;
                    break;
                case 3: { // Move wide with keep
                    int64_t mask = MASK16 << (dpimm.hw * 16);
                    *Rd = (*Rd & ~mask) | imm16;
                    break;
                }
                default:
                    perror("Unsupported wide move type (bits 29-30), use either 00, 10 or 11.\n");
                    return 1;
            }
            break;
        }
        default: {
            perror("Unsupported opi (bits 23-25), use either 010 or 101.\n");
            return 1;
        }
    }

    maskTo32Bits(dpimm.sf, Rd);
    return 0;
}

// 1.5 Data Processing Instruction (Register)

int decodeDPR(uint32_t instr, Instruction *instruction, BitFunc bitFunc) {
    instruction->instructionType = isDPR;
    struct DPR *dpr = &(instruction->dpr);
    bitFunc(instr, &(dpr->sf), 31, 1);
    bitFunc(instr, &(dpr->m), 28, 1);
    bitFunc(instr, &(dpr->rm), 16, 5);
    bitFunc(instr, &(dpr->operand), 10, 6);
    bitFunc(instr, &(dpr->rn), 5, 5);
    bitFunc(instr, &(dpr->rd), 0, 5);

    // Type of data processing operation
    if (dpr->m == 0) { // Arithmetic, Bit-logic
        bitFunc(instr, &(dpr->opc), 29, 2);
        bitFunc(instr, &(dpr->armOrLog), 24, 1);
        bitFunc(instr, &(dpr->shift), 22, 2);
        bitFunc(instr, &(dpr->n), 21, 1);
    } else { // Multiply
        bitFunc(instr, &(dpr->x), 15, 1);
        bitFunc(instr, &(dpr->ra), 10, 5);
    }
    return 0;
}

void updateFlagsAnd(int64_t a, int64_t b, bool sf) {
    int64_t res = a & b;

    // Sign Flag (N)
    state.pstate.N = (sf == 0) ? ((int32_t)(res & MASK32) < 0)
                               : (res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = 0;
    // Overflow Flag (V)
    state.pstate.V = 0;
}

int executeDPR(Instruction instruction) {
    struct DPR dpr = instruction.dpr;
    int64_t *Rd = &state.R[dpr.rd];
    int64_t Rm = state.R[dpr.rm];
    int64_t Rn = state.R[dpr.rn];

    maskTo32Bits(dpr.sf, &Rm);
    maskTo32Bits(dpr.sf, &Rn);

    if (dpr.m == 0) {
        // Compute offset
        int64_t op2 = shift(Rm, dpr.operand, dpr.shift, dpr.sf);

        if (dpr.armOrLog == 1) { // Arithmetic
            addOrSub(dpr.opc, dpr.rd, dpr.sf, Rd, Rn, op2);
        } else { // Logical
            // Simulate execution considering the opc
            if (dpr.n == 1) {
                op2 = ~op2;
            }
            switch (dpr.opc) {
                case 0: // Bitwise AND and Bit clear
                    *Rd = Rn & op2;
                    break;
                case 1: // Bitwise inclusive OR and NOR
                    *Rd = Rn | op2;
                    break;
                case 2: // Bitwise exclusive OR and NOR
                    *Rd = Rn ^ op2;
                    break;
                case 3: // Bitwise AND and Bit clear, setting flags
                    if (dpr.rd != ZR_SP) {
                        *Rd = Rn & op2;
                    }
                    updateFlagsAnd(Rn, op2, dpr.sf);
                    break;
            }
        }
    } else { // Multiply
        if (dpr.rd != ZR_SP) {
            int64_t Ra = (dpr.ra != ZR_SP) ? state.R[dpr.ra] : 0;
            *Rd = (dpr.x == 0)
                      ? Ra + (Rn * Rm)  // Multiply-Add
                      : Ra - (Rn * Rm); // Multiply-Sub
        }
    }

    maskTo32Bits(dpr.sf, Rd);
    return 0;
}

// 1.6 Bitwise Shifts

int64_t shift(int64_t value, int8_t amount, uint8_t mode, bool nobits) {
    amount %= (!nobits) ? MODE32 : MODE64;

    int32_t value32 = value & MASK32;

    switch (mode) {
        case 0: // Logical Shift Left (lsl)
            return (!nobits) ? (value32 << amount) & MASK32
                            : (value << amount);
            break;
        case 1: // Logical Shift Right (lsr)
            return (!nobits) ? ((uint32_t)value32 >> amount) & MASK32
                            : (uint64_t)value >> amount;
            break;
        case 2: // Arithmetic Shift Right (asr)
            return (!nobits) ? (value32 >> amount) & MASK32
                            : value >> amount;
            break;
        case 3: { // Rotate Right (ror)
            uint8_t rotateAmount = (!nobits) ? MODE32 - amount
                                            : MODE64 - amount;
            int64_t bitsToRotate = value << rotateAmount;

            return (!nobits) ? (((uint32_t)value32 >> amount) | bitsToRotate) & MASK32
                            : ((uint64_t)value >> amount) | bitsToRotate;
            break;
        }
        default:
            perror("Unsupported shift mode. Provide a mode between 0 and 3.\n");
            exit(EXIT_FAILURE);
    }
}

// 1.7 Single Data Transfer Instructions

int decodeSDT(uint32_t instr, Instruction *instruction, BitFunc bitFunc) {
    instruction->instructionType = isSDT;
    struct SDT *sdt = &(instruction->sdt);
    bitFunc(instr, &(sdt->mode), 31, 1);
    bitFunc(instr, &(sdt->sf), 30, 1);
    bitFunc(instr, &(sdt->rt), 0, 5);

    // Type of addressing mode: single data transfer - 1, load literal - 0
    if (sdt->mode == 1) { // Single Data Transfer
        bitFunc(instr, &(sdt->u), 24, 1);
        bitFunc(instr, &(sdt->l), 22, 1);
        bitFunc(instr, &(sdt->offmode), 21, 1);
        bitFunc(instr, &(sdt->xn), 5, 5);

        if (sdt->u == 1) { // Unsigned Immediate Offset
            bitFunc(instr, &(sdt->imm12), 10, 12);
        } else if (sdt->offmode == 0) { // Pre/Post - Index
            bitFunc(instr, &(sdt->simm9), 12, 9);
            signExtendTo32Bits(&(sdt->simm9), 9);
            bitFunc(instr, &(sdt->i), 11, 1);
        } else { // Register Offset
            bitFunc(instr, &(sdt->xm), 16, 5);
        }

    } else { // Load Literal
        bitFunc(instr, &(sdt->simm19), 5, 19);
        signExtendTo32Bits(&(sdt->simm19), 19);
    }

    return 0;
}

void loadFromMemory(int addr, int64_t *reg, bool mode) {
    int64_t result = 0;
    int bytes = (mode) ? MODE64_BYTES : MODE32_BYTES;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    for (int i = 0; i < bytes; i++) {
        result |= ((int64_t)mem[addr + i]) << (BYTE_SIZE * i);
    }
    // This is also transofrmed from little endian
    *reg = result;
}

void storeToMemory(int addr, int64_t reg, bool mode) {
    int bytes = (mode) ? MODE64_BYTES : MODE32_BYTES;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    for (int i = 0; i < bytes; i++) {
        mem[addr + i] = (reg >> (BYTE_SIZE * i)) & MASK8;
    }
    // This is also transofrmed from little endian
}

int executeSDT(Instruction instruction) {
    struct SDT sdt = instruction.sdt;

    // Although xn is 64 bits, addresses can only be 21 bits, so trim to int.
    uint32_t targetAddress;

    maskTo32Bits(sdt.sf, &state.R[sdt.rt]);

    // Single Data Transfer
    if (sdt.mode == 1) {
        int64_t *Xn = (sdt.xn == ZR_SP) ? &state.SP : &state.R[sdt.xn];
        targetAddress = *Xn;

        // Unsigned Immediate Offset
        if (sdt.u == 1) {
            uint16_t uoffset = sdt.imm12 * ((sdt.sf == 0) ? MODE32_BYTES : MODE64_BYTES);
            targetAddress += uoffset;
        } else if (sdt.offmode == 0) { // Pre/Post - Index
            targetAddress += (sdt.i) ? sdt.simm9 : 0;
            *Xn += (int64_t)sdt.simm9;
        } else { // Register Offset
            int64_t *Xm = (sdt.xn == ZR_SP) ? &state.SP : &state.R[sdt.xm];
            targetAddress += *Xm;
        }

        // Simulate the Data Transfer
        if (sdt.l == 1) { // Load
            loadFromMemory(targetAddress, &state.R[sdt.rt], sdt.sf);
        } else { // Store
            storeToMemory(targetAddress, state.R[sdt.rt], sdt.sf);
        }
        
    } else { // Load Literal
        targetAddress = state.PC + ((int64_t)sdt.simm19) * INSTR_BYTES;

        // Simulate the Data Transfer
        loadFromMemory(targetAddress, &state.R[sdt.rt], sdt.sf);
    }

    return 0;
}

// 1.8 Branch Instructions

int decodeB(uint32_t instr, Instruction *instruction, BitFunc bitFunc) {
    instruction->instructionType = isBranch;
    struct Branch *br = &(instruction->br);
    bitFunc(instr, &(br->type), 30, 2);

    // Type of branch: unconditional - 00, register - 11, conditional - 01
    switch (br->type) {
        case 0: // Unconditional
            bitFunc(instr, &(br->simm26), 0, 26);
            signExtendTo32Bits(&(br->simm26), 26);
            break;
        case 1: // Conditional
            bitFunc(instr, &(br->simm19), 5, 19);
            signExtendTo32Bits(&(br->simm19), 19);
            bitFunc(instr, &(br->condTag), 1, 3);
            bitFunc(instr, &(br->condInvert), 0, 1);
            break;
        case 3: // Register
            bitFunc(instr, &(br->nop), 25, 1);
            bitFunc(instr, &(br->xn), 5, 5);
            break;
        default:
            perror("Unsupported branch type (bits 30-31), use either 00, 01 or 11.\n");
            return 1;
    }

    return 0;
}

int executeB(Instruction instruction) {
    struct Branch br = instruction.br;

    switch (br.type) {
        case 0: // Unconditional
            state.PC += ((int64_t)br.simm26) * INSTR_BYTES;
            break;
        case 1: { // Conditional
            bool toBranch;
            switch (br.condTag) {
                case 0: // EQ (equal) - 0000, NE (not equal) - 0001
                    toBranch = state.pstate.Z ^ br.condInvert;
                    break;
                case 5: // GE (greater or equal) - 1010, LT (less) - 1011
                    toBranch = (state.pstate.N == state.pstate.V) ^ br.condInvert;
                    break;
                case 6: // GT (greater) - 1100, LE (less or equal) - 1101
                    toBranch = (!state.pstate.Z && (state.pstate.N == state.pstate.V)) ^ br.condInvert;
                    break;
                case 7: // AL (always) - 1110
                    toBranch = 1;
                    break;
                default:
                    perror("Unsupported branch condition (bits 1-3), use either 000, 101, 110 or 111.\n");
                    return 1;
            }

            state.PC += ((toBranch) ? (int64_t)br.simm19 : 1) * INSTR_BYTES;
            break;
        }
        case 3: // Register
            if (br.nop) {
                state.PC = (br.xn == ZR_SP) ? state.ZR : state.R[br.xn];
            } else { // Nop
                state.PC += INSTR_BYTES;
            }
            break;
        default:
            perror("Unsupported branch type (bits 30-31), use either 00, 01 or 11.\n");
            return 1;
    }
    return 0;
}

//
// Pipeline Stages
//
uint32_t fetch(uint32_t addr) {
    // Fetch instruction from memory
    uint32_t result = 0;
    for (int i = 0; i < INSTR_BYTES; i++) {
        result |= ((uint32_t)mem[addr + i]) << (BYTE_SIZE * i);
    }
    // This is also transofrmed from little endian
    return result;
}

int decode(uint32_t instr, Instruction *instruction, BitFunc bitFunc) {
    uint8_t op0;
    getBits(instr, &op0, 25, 4);

    if (op0 / 2 == 4) { // 100x - Data Processing Immediate
        return decodeDPImm(instr, instruction, bitFunc);
    } else if (op0 % 8 == 5) { // x101 - Data Processing Register
        return decodeDPR(instr, instruction, bitFunc);
    } else if (op0 % 2 == 0 && op0 / 4 % 2 == 1) { // x1x0 - Loads and Stores
        return decodeSDT(instr, instruction, bitFunc);
    } else if (op0 / 2 == 5) { // 101x - Branch
        return decodeB(instr, instruction, bitFunc);
    } else { // invalid op0
        perror("Unsupported op0 (bits 25-28), use either 100x, x101, x1x0 or 101x.\n");
        return 1;
    }
}

int execute(Instruction instruction) {
    switch (instruction.instructionType) {
        case isDPI:
            return executeDPImm(instruction);
        case isDPR:
            return executeDPR(instruction);
        case isSDT:
            return executeSDT(instruction);
        case isBranch:
            return executeB(instruction);
        default:
            perror("Unsupported instruction type.\n");
            return 1;
    }
}

void checkErrors(bool error) {
    if (error) {
        exit(EXIT_FAILURE);
    }
}

//
// Main Program
//

int main(int argc, char **argv) {
    char *inputFile = argv[1];
    char *outputFile = (argc > 2) ? argv[2] : "stdout";

    // Set up initial state
    initializeState();
    // Read from binary file
    // Store instructions into memory
    loadBinaryFile(inputFile);

    uint32_t instr;
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));

    while ((instr = fetch(state.PC)) != HALT) {
        int decodeError = decode(instr, instruction, getBits);
        checkErrors(decodeError);
        int executeError = execute(*instruction);
        checkErrors(executeError);

        // Update address of next instruction
        if (instruction->instructionType != isBranch) {
            state.PC += INSTR_BYTES;
        }
    }

    free(instruction);

    // Write the final state after executing all instructions
    writeOutput(outputFile);
    return EXIT_SUCCESS;
}
