#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE (2 * 1024 * 1024)

// Declarations
extern int64_t shift(int64_t, unsigned short, unsigned short, bool);

// Memory
unsigned char mem[MEMORY_SIZE];

// Emulator State
struct estate
{
    int64_t R[31];
    int64_t ZR;
    int64_t PC;
    int64_t SP;
    struct PSTATE
    {
        bool N;
        bool Z;
        bool C;
        bool V;
        // int mode;
    } pstate;
};
struct estate state = {0};

// Categories of instructions to support
typedef enum
{
    DPImm,
    DPReg,
    LS,
    B,
    Err
} category;

//
// Utility Functions
//

void initializeState(void)
{
    state.pstate.Z = true;
}

void charToBinary(char c, char *memData)
{
    for (int i = 7; i >= 0; i--)
    {
        memData[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    memData[8] = '\0';
}

int instructionToInt(int addr)
{
    int result = 0;
    result |= mem[addr];
    result |= (mem[addr + 1] << 8);
    result |= (mem[addr + 2] << 16);
    result |= (mem[addr + 3] << 24);
    // This is also transofrmed from little endian
    return result;
}

void loadBinaryFile(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if (!extension || strcmp(extension + 1, "bin") != 0)
    {
        fprintf(stderr, "This is not a valid binary file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    FILE *file;
    file = fopen(filename, "rb");
    if (!file)
    {
        perror("Could not open input file.");
        exit(EXIT_FAILURE);
    }

    size_t numberOfBytes = fread(mem, 1, MEMORY_SIZE, file);

    if (numberOfBytes == 0)
    {
        perror("The file is empty.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void writeOutput(const char *filename)
{
    FILE *file;
    file = (!strcmp(filename, "stdout")) ? stdout : fopen(filename, "a");
    if (!file)
    {
        perror("Could not open output file.\n");
        exit(EXIT_FAILURE);
    }
    const char *extension = strrchr(filename, '.');
    if (file != stdout && (!extension || strcmp(extension + 1, "out") != 0))
    {
        fprintf(stderr, "This is not a valid .out file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Registers:\n");
    for (int i = 0; i < 31; i++)
    {
        fprintf(file, "X%d%d    = %016lx\n", i > 9 ? i / 10 : 0, i % 10, state.R[i]);
    }
    fprintf(file, "PC     = %016lx\n", state.PC);
    fprintf(file, "PSTATE : %c%c%c%c",
            state.pstate.N ? 'N' : '-',
            state.pstate.Z ? 'Z' : '-',
            state.pstate.C ? 'C' : '-',
            state.pstate.V ? 'V' : '-');
    fprintf(file, "\nNon-Zero Memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += 4)
    {
        int binInstr = instructionToInt(addr);
        if (binInstr != 0)
        {
            fprintf(file, "0x%08x : %08x\n", addr, binInstr);
        }
    }
    if (file != stdout)
        fclose(file);
}

//
// Binary Manipulation Helpers
//
int checkOp0(int instr)
{
    return instr >> 25 & 0xF;
}

void getBitsChar(uint32_t x, int nbits, char *buf)
{
    uint32_t mask = 1 << (nbits - 1);
    for (int i = 0; i < nbits; i++)
    {
        int bit = (x & mask) != 0;
        buf[i] = '0' + bit;
        mask >>= 1;
    }
    buf[nbits] = '\0';
}

void getBitsInt(uint32_t x, int nbits, int *res)
{
    uint32_t mask = 1 << (nbits - 1);
    *res = 0;
    for (int i = 0; i < nbits; i++)
    {
        int bit = (x & mask) != 0;
        *res = (0 + bit) * (1 << (nbits - i - 1)) + *res;
        mask >>= 1;
    }
}

void getBitsShortInt(uint32_t x, int nbits, unsigned short int *res)
{
    uint32_t mask = 1 << (nbits - 1);
    *res = 0;
    for (int i = 0; i < nbits; i++)
    {
        int bit = (x & mask) != 0;
        *res = (bit) * (1 << (nbits - i - 1)) + *res;
        mask >>= 1;
    }
}

//
// Pipeline Stages
//
int fetch(void)
{
    // Fetch instruction from memory
    int instruction = instructionToInt(state.PC);

    // Compute the address of the next instruction
    // We update PC after execution stage

    return instruction;
}

category decode(int instr)
{
    char op0[5];
    getBitsChar(checkOp0(instr), 4, op0);
    if (op0[1] == '0')
    {
        if (op0[0] == '1')
        {
            return (op0[2] == '0') ? DPImm : B;
        }
    }
    else
    {
        if (op0[3] == '1')
        {
            if (op0[2] == '0')
                return DPReg;
        }
        else
            return LS;
    }
    return Err;
}

// 1.4 Data Processing Instruction (Immediate)

struct dpimm
{
    bool sf;
    unsigned short opc;
    unsigned short opi;
    union
    {
        struct
        {
            bool sh;
            unsigned short imm12;
            unsigned short rn;
        } arithmetic;
        struct
        {
            unsigned short hw;
            unsigned short imm16;
        } widemove;
    };
    unsigned short rd;
} dpimm;

void decodeDPImm(int instr)
{
    getBitsShortInt(instr, 5, &dpimm.rd);
    instr >>= 5;
    int operand;
    getBitsInt(instr, 18, &operand);
    instr >>= 18;
    getBitsShortInt(instr, 3, &dpimm.opi);
    instr >>= 6;
    getBitsShortInt(instr, 2, &dpimm.opc);
    instr >>= 2;
    dpimm.sf = instr % 2;

    // Check for 32-bit or 64-bit mode for Rd
    int64_t mask = 0xFFFFFFFF;
    if (dpimm.sf == 0)
        state.R[dpimm.rd] &= mask;

    // Type of data processing operation
    // Only support: arithmetic - 010, widemove - 101
    switch (dpimm.opi)
    {
    case 2: // opi = 010
        getBitsShortInt(operand, 5, &dpimm.arithmetic.rn);
        operand >>= 5;
        getBitsShortInt(operand, 12, &dpimm.arithmetic.imm12);
        operand >>= 12;
        dpimm.arithmetic.sh = operand % 2;
        // Check for 32-bit or 64-bit mode for Rn
        if (dpimm.sf == 0)
            state.R[dpimm.arithmetic.rn] &= mask;
        break;
    case 5: // opi = 101
        getBitsShortInt(operand, 16, &dpimm.widemove.imm16);
        operand >>= 16;
        getBitsShortInt(operand, 2, &dpimm.widemove.hw);
        break;
    default:
        perror("Not supported type of data processing operation (opi).\n");
        exit(EXIT_FAILURE);
    }
}

void updateFlagsAdd(int64_t a, int64_t b)
{
    // We know b is unsigned, so positive.
    int64_t res = a + b;

    // Sign Flag (N)
    state.pstate.N = (!dpimm.sf)
                         ? ((int32_t)(res & 0xFFFFFFFF) < 0)
                         : (res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = (!dpimm.sf)
                         ? (res > 0xFFFFFFFF)
                         : (res < a);
    // Overflow Flag (V)
    state.pstate.V = (a > 0 && state.pstate.N);
}

void updateFlagsSub(int64_t a, int64_t b)
{
    // We know b is unsigned, so positive.
    int64_t res = a - b;

    // Sign Flag (N)
    state.pstate.N = (!dpimm.sf)
                         ? ((int32_t)(res & 0xFFFFFFFF) < 0)
                         : (res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = (a >= b);
    // Overflow Flag (V)
    state.pstate.V = (a < 0 && !state.pstate.N);
}

void executeDPImm(void)
{
    switch (dpimm.opi)
    {
    case 2: // opi = 010
    {
        uint32_t imm12 = dpimm.arithmetic.imm12 << (12 * dpimm.arithmetic.sh);

        int64_t Rn = (dpimm.arithmetic.rn == 31)
                         ? state.SP
                         : state.R[dpimm.arithmetic.rn];
        int64_t *Rd = (dpimm.rd == 31)
                          ? (dpimm.opc % 2 == 0) ? &state.SP : &state.ZR
                          : &state.R[dpimm.rd];

        switch (dpimm.opc)
        {
        case 0: // Add
            *Rd = Rn + imm12;
            break;
        case 1: // Add and set flags
            if (dpimm.rd != 31)
                *Rd = Rn + imm12;
            updateFlagsAdd(Rn, imm12);
            break;
        case 2: // Subtract
            *Rd = Rn - imm12;
            break;
        case 3: // Subtract and set flags
            if (dpimm.rd != 31)
                *Rd = Rn - imm12;
            updateFlagsSub(Rn, imm12);
            break;
        default:
            // Can't reach here
            break;
        }
        break;
    }
    case 5: // opi = 101
    {
        uint64_t imm16 = dpimm.widemove.imm16 << (dpimm.widemove.hw * 16);
        if (dpimm.rd == 31)
            break;
        int64_t *Rd = &state.R[dpimm.rd];

        switch (dpimm.opc)
        {
        case 0: // Move wide with NOT
        {
            *Rd = ~imm16;
            if (dpimm.sf == 0)
                *Rd &= 0xFFFFFFFF;
            break;
        }
        case 2: // Move wide with zero
        {
            *Rd = imm16;
            break;
        }
        case 3: // Move wide with keep
        {
            int64_t masku = (1 << (16 + (dpimm.widemove.hw * 16))) - 1;
            int64_t maskl = (1 << (dpimm.widemove.hw * 16)) - 1;
            int64_t mask = masku & maskl;
            *Rd = (*Rd & ~mask) | imm16;
            break;
        }
        default:
            // Can't reach here
            break;
        }
        break;
    }
    default:
    {
        perror("Not supported type of data processing opeartion (opi).\n");
        exit(EXIT_FAILURE);
    }
    }
}

void dataProcessingImmInstruction(int instr)
{
    decodeDPImm(instr);
    executeDPImm();
}

// 1.5 Data Processing Instruction (Register)

struct dpr
{
    bool sf;
    unsigned short opc;
    bool m;
    unsigned short opr;
    unsigned short rm;
    unsigned short operand;
    unsigned short rn;
    unsigned short rd;
} dpr;

void decodeDPR(int instr)
{
    getBitsShortInt(instr, 5, &dpr.rd);
    instr >>= 5;
    getBitsShortInt(instr, 5, &dpr.rn);
    instr >>= 5;
    getBitsShortInt(instr, 6, &dpr.operand);
    instr >>= 6;
    getBitsShortInt(instr, 5, &dpr.rm);
    instr >>= 5;
    getBitsShortInt(instr, 4, &dpr.opr);
    instr >>= 7;
    dpr.m = instr % 2;
    instr >>= 1;
    getBitsShortInt(instr, 2, &dpr.opc);
    instr >>= 2;
    dpr.sf = instr % 2;

    // Check for 32-bit or 64-bit mode for Rd
    int64_t mask = 0xFFFFFFFF;
    if (dpr.sf == 0)
    {
        state.R[dpr.rd] &= mask;
        state.R[dpr.rn] &= mask;
        state.R[dpr.rm] &= mask;
    }

    // Type of data processing operation
    // Support (M + opr): arithmetic - 01xx0, bit-logic - 00xxx, multiply - 11000
    if (dpr.m == 0)
    {
        unsigned short shiftMode = (dpr.opr >> 1) % 4;

        // Check arithmetic correct bits
        if ((dpr.opr >> 3) % 2 == 1)
        {
            if (shiftMode == 3 || dpr.opr % 2 == 1)
            {
                perror("Invalid shift for arithmetic instruction.");
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        if (dpr.opr != 8 && dpr.opc != 0)
        {
            perror("Invalid multiply instruction.");
            exit(EXIT_FAILURE);
        }
    }
}

void updateFlagsAnd(int64_t a, int64_t b)
{
    int64_t res = a & b;

    // Sign Flag (N)
    state.pstate.N = (!dpr.sf)
                         ? ((int32_t)(res & 0xFFFFFFFF) < 0)
                         : (res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = 0;
    // Overflow Flag (V)
    state.pstate.V = 0;
}

void executeDPR(void)
{
    if (dpr.m == 0)
    {
        unsigned short shiftMode = (dpr.opr >> 1) % 4;
        int64_t *Rd = (dpr.rd == 31)
                          ? (dpr.opc % 2 == 0) ? &state.SP : &state.ZR
                          : &state.R[dpr.rd];
        ;
        // Compute offset
        int64_t op2 = shift(state.R[dpr.rm], dpr.operand, shiftMode, dpr.sf);

        // Arithmetic
        if ((dpr.opr >> 3) % 2 == 1)
        {
            // Simulate execution from opc (same as for arithmetic immediate)
            switch (dpr.opc)
            {
            case 0: // Add
                *Rd = dpr.rn + op2;
                break;
            case 1: // Add and set flags
                if (dpr.rd != 31)
                    *Rd = dpr.rn + op2;
                updateFlagsAdd(dpr.rn, op2);
                break;
            case 2: // Subtract
                *Rd = dpr.rn - op2;
                break;
            case 3: // Subtract and set flags
                if (dpr.rd != 31)
                    *Rd = dpr.rn - op2;
                updateFlagsSub(dpr.rn, op2);
                break;
            default:
                // Can't reach here
                break;
            }
        }
        // Logical
        else
        {
            bool n = dpr.opr % 2;

            // Simulate execution from opc
            Rd = (dpr.rd == 31)
                     ? (dpr.opc % 2 == 0) ? &state.SP : &state.ZR
                     : &state.R[dpr.rd];

            switch (dpr.opc)
            {
            case 0: // Bitwise AND and Bit clear
                *Rd = (n) ? dpr.rn & op2 : dpr.rn & ~op2;
                break;
            case 1: // Bitwise inclusive OR and NOR
                *Rd = (n) ? dpr.rn | op2 : dpr.rn | ~op2;
                break;
            case 2: // Bitwise exclusive OR and NOR
                *Rd = (n) ? dpr.rn ^ op2 : dpr.rn ^ ~op2;
                break;
            case 3: // Bitwise AND and Bit clear, setting flags
                if (dpr.rd != 31)
                    *Rd = (n) ? dpr.rn & op2 : dpr.rn & ~op2;
                updateFlagsAnd(dpr.rn, op2);
                break;
            default:
                // Can't reach here
                break;
            }
        }
    }
    else
    {
        unsigned short ra;
        bool x = (dpr.operand >> 5) % 2;
        getBitsShortInt(dpr.operand, 5, &ra);

        if (dpr.rd == 31)
            return;

        state.R[dpr.rd] = (!x)
                              ? state.R[ra] + (state.R[dpr.rn] * state.R[dpr.rm])  // Multiply-Add
                              : state.R[ra] - (state.R[dpr.rn] * state.R[dpr.rm]); // Multiply-Sub
    }
}

void dataProcessingRInstruction(int instr)
{
    decodeDPR(instr);
    executeDPR();
}

// 1.6 Bitwise Shifts

int64_t shift(int64_t value, unsigned short amount, unsigned short mode, bool nobits)
{

    if (amount < 0 || amount >= (nobits ? 32 : 64))
    {
        perror("Shift amount out of range.");
        exit(EXIT_FAILURE);
    }

    int64_t mask32 = 0xFFFFFFFF;
    int32_t value32 = value & mask32;

    switch (mode)
    {
    case 0: // Logical Shift Left (lsl)
    {
        return (!nobits) ? (value32 << amount) & mask32
                         : (value << amount);
        break;
    }
    case 1: // Logical Shift Right (lsr)
    {
        return (!nobits) ? ((uint32_t)value32 >> amount) & mask32
                         : (uint64_t)value >> amount;
        break;
    }
    case 2: // Arithmetic Shift Right (asr)
    {
        return (!nobits) ? (value32 >> amount) & mask32
                         : value >> amount;
        break;
    }
    case 3: // Rotate Right (ror)
    {
        short int rotateAmount = (!nobits) ? 32 - amount
                                           : 64 - amount;
        int64_t bitsToRotate = value << rotateAmount;

        return (!nobits) ? (((uint32_t)value32 >> amount) | bitsToRotate) & mask32
                         : ((uint64_t)value >> amount) | bitsToRotate;
        break;
    }
    default:
        perror("Unsupported shift mode. Provide a mode between 0 and 3.");
        exit(EXIT_FAILURE);
        break;
    }
}

// 1.7 Single Data Transfer Instructions

struct sdt
{
    bool mode;
    bool sf;
    union
    {
        struct
        {
            bool u;
            bool l;
            bool offmode;
            union
            {
                unsigned short xm;
                struct
                {
                    unsigned short simm9;
                    bool i;
                };
                unsigned short imm12;
            } addrm;
            unsigned short xn;
        };
        int simm19;
    };
    unsigned short rt;
} sdt;

void decodeSDT(int instr)
{
    getBitsShortInt(instr, 5, &sdt.rt);
    instr >>= 5;
    int uncommon;
    getBitsInt(instr, 20, &uncommon);
    instr >>= 25;
    sdt.sf = instr % 2;
    sdt.mode = (instr >> 1) % 2;

    // Check for 32-bit or 64-bit mode for Rd
    int64_t mask = 0xFFFFFFFF;
    if (sdt.sf == 0)
        state.R[sdt.rt] &= mask;

    // Type of addressing mdode
    // Support: single data transfer - 1, load literal - 0
    // Single Data Transfer
    if (sdt.mode == 0)
    {
        getBitsShortInt(uncommon, 5, &sdt.xn);
        uncommon >>= 5;
        unsigned short offset;
        getBitsShortInt(uncommon, 12, &offset);
        uncommon >>= 12;
        sdt.l = uncommon % 2;
        uncommon >>= 2;
        sdt.u = uncommon % 2;

        sdt.offmode = (offset >> 11) % 2;

        // Unsigned Immediate Offset
        if (sdt.u == 1)
        {
            sdt.addrm.imm12 = offset;
        }
        // Pre/Post - Index
        if (!sdt.offmode)
        {
            sdt.addrm.i = (offset >> 1) % 2;
            sdt.addrm.simm9 = offset / 4;
        }
        // Register Offset
        else
        {
            getBitsShortInt(offset >> 6, 5, &sdt.addrm.xm);
        }
    }
    // Load Literal
    else
        getBitsInt(uncommon, 19, &sdt.simm19);
}

void loadFromMemory(int addr, int64_t *reg, bool mode)
{
    int result = 0;
    int bytes = mode * 4 + 4;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    for (int i = 0; i < bytes; i++)
    {
        result |= mem[addr + i] << (8 * i);
    }
    // This is also transofrmed from little endian
    *reg = result;
}

void storeToMemory(int addr, int64_t *reg, bool mode)
{
    int bytes = mode * 4 + 4;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    char *ptr = (char *)reg;
    for (int i = bytes - 1; i >= 0; i--)
    {
        mem[addr + i] = *ptr++;
    }
    // This is also transofrmed from little endian
}

void executeSDT(void)
{
    // Although xn is 64 bits, addresses can only be 21 bits, so trim to int.
    int targetAddress;

    // Single Data Transfer
    if (sdt.mode == 0)
    {
        targetAddress = (sdt.xn == 31)
                            ? state.SP
                            : state.R[sdt.xn];

        // Unsigned Immediate Offset
        if (sdt.u == 1)
        {
            short int uoffset = (!sdt.sf)
                                    ? sdt.addrm.imm12 * 8
                                    : sdt.addrm.imm12 * 4;
            targetAddress += uoffset;
        }
        // Pre/Post - Index
        if (!sdt.offmode)
        {
            targetAddress += (sdt.addrm.i) ? sdt.addrm.simm9 : 0;
            if (sdt.xn == 31)
            {
                state.SP += sdt.addrm.simm9;
            }
            else
            {
                state.R[sdt.xn] += sdt.addrm.simm9;
            }
        }
        // Register Offset
        else
        {
            targetAddress += state.R[sdt.addrm.xm];
        }
    }
    // Load Literal
    else
    {
        targetAddress = state.PC + sdt.simm19 * 4;
    }

    // Simulate the Data Transfer
    if (sdt.l == 1)
    {
        // Load
        loadFromMemory(targetAddress, &state.R[sdt.rt], sdt.sf);
    }
    else
    {
        // Store
        storeToMemory(targetAddress, &state.R[sdt.rt], sdt.sf);
    }
}

void singleDataTransferInstruction(int instr)
{
    decodeSDT(instr);
    executeSDT();
}

// 1.8 Branch Instructions

struct br
{
    unsigned short type;
    union
    {
        int simm26;
        unsigned short xn;
        struct
        {
            int simm19;
            unsigned short cond;
        };
    };
} br;

void decodeB(int instr)
{
    int branch;
    getBitsInt(instr, 26, &branch);
    instr >>= 30;
    getBitsShortInt(instr, 2, &br.type);

    // Type of branch
    // Supported: unconditional - 00, register - 11, conditional - 01
    switch (br.type)
    {
    case 0: // opi = 010
        getBitsInt(branch, 26, &br.simm26);
        break;
    case 1: // opi = 101
        branch >>= 5;
        getBitsShortInt(branch, 5, &br.xn);
        break;
    case 3: // opi = 101
        getBitsShortInt(branch, 4, &br.cond);
        branch >>= 5;
        getBitsInt(branch, 19, &br.simm19);
        break;
    default:
        perror("Not supported type of branching.\n");
        exit(EXIT_FAILURE);
    }
}

void executeB(void)
{
    switch (br.type)
    {
    case 0: // Unconditional
        state.PC += br.simm26 * 4;
        break;
    case 1: // Conditional
    {
        bool toBranch = false;
        switch (br.cond)
        {
        case 0: // EQ - 0000
            toBranch = state.pstate.Z;
            break;
        case 1: // NE - 0001
            toBranch = !state.pstate.Z;
            break;
        case 10: // GE - 1010
            toBranch = (state.pstate.N == state.pstate.V);
            break;
        case 11: // LT - 1011
            toBranch = (state.pstate.N != state.pstate.V);
            break;
        case 12: // GT - 1100
            toBranch = (!state.pstate.Z && state.pstate.N == state.pstate.V);
            break;
        case 13: // LE - 1101
            toBranch = (state.pstate.Z || state.pstate.N != state.pstate.V);
            break;
        case 14: // AL - 1110
            toBranch = 1;
            break;
        }
        if (toBranch)
        {
            state.PC += br.simm19 * 4;
        }
        break;
    }
    case 3: // Register
        state.PC = (br.xn == 31)
                       ? state.ZR
                       : state.R[br.xn];
        break;
    default:
    {
        perror("Not supported type of data processing opeartion (opi).\n");
        exit(EXIT_FAILURE);
    }
    }
}

void branchInstruction(int instr)
{
    decodeB(instr);
    executeB();
}

//
// Main Program
//

int main(int argc, char **argv)
{
    char *inputFile = argv[1];
    char *outputFile = (argc > 2) ? argv[2] : "stdout";

    // Set up initial state
    initializeState();
    // Read from binary file
    // Store instructions into memory
    loadBinaryFile(inputFile);

    unsigned int instr;

    // 0x8a000000 in hex = 2315255808 in dec
    while ((instr = fetch()) != 2315255808)
    {
        category cat = decode(instr);

        switch (cat)
        {
        case DPImm:
            dataProcessingImmInstruction(instr);
            break;
        case DPReg:
            dataProcessingRInstruction(instr);
            break;
        case LS:
            singleDataTransferInstruction(instr);
            break;
        case B:
            branchInstruction(instr);
            break;
        default:
            // Incorrect opcode
            fprintf(stderr, "Incorrect opcode0, instruction not supported: %x", instr);
            exit(EXIT_FAILURE);
        }
        // Update address of next instruction
        if (cat != B)
            state.PC += 4;
    }

    // Write the final state after executing all instructions
    writeOutput(outputFile);
    return EXIT_SUCCESS;
}
