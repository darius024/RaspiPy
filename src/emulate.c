#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE (2 * 1024 * 1024) // 2MB
#define MASK32 0xFFFFFFFFLL

// Declarations
extern int64_t shift(int64_t, unsigned short, unsigned short, bool);

// Memory
unsigned char mem[MEMORY_SIZE];

// Emulator State
struct EState
{
    int64_t R[31]; // Registers R0-R30
    int64_t ZR; // Zero Register
    int64_t PC; // Program Counter
    int64_t SP; // Stack Pointer
    struct PSTATE // Processor State
    {
        bool N; // Negative flag
        bool Z; // Zero flag
        bool C; // Carry flag
        bool V; // oVerflow flag
    } pstate;
};
struct EState state = {0};

// Categories of instructions to support
typedef enum
{
    isDPI, // Data processing immediate
    isDPR, // Data processing register
    isSDT, // Single data transfer
    isBranch, // Branch
} InstructionType;

// data processing immediate
struct DPI
{
    bool sf; // bit-width: 0 for 32-bit, 1 for 64-bit
    uint8_t opc; // opcode
    uint8_t opi; // arithmetic or wide move
    union
    {
        struct
        {
            bool sh; // 12-bit left shift of imm12
            uint16_t imm12; // 1st operand: immediate
            uint8_t rn; // 2nd operand: register
        } arithmetic;
        struct
        {
            uint8_t hw; // (hw * 4) left shift of imm16
            uint16_t imm16; // operand
        } widemove;
    };
    uint8_t rd; // Destination register
};

// data processing register
struct DPR
{
    bool sf; // bit-width: 0 for 32-bit, 1 for 64-bit
    uint8_t opc; // opcode
    bool m; // multiply flag
    uint8_t opr; // operation: arithmetic, bit-logic, multiply
    uint8_t rm;
    uint8_t operand;
    uint8_t rn;
    uint8_t rd; // destination register
};

// single data transfer
struct SDT
{
    bool mode; // 1 for single data transfer, 0 for load literal
    bool sf; // load size: 0 for 32-bit, 1 for 64-bit
    union
    {
        struct
        {
            bool u; // unsigned offset flag
            bool l; // type of data transfer
            bool offmode; // 1 for register offset, 0 for pre/post-index
            union
            {
                uint8_t xm;
                struct
                {
                    int32_t simm9;
                    bool i;
                };
                uint16_t imm12;
            } addrm;
            uint8_t xn;
        };
        int32_t simm19;
    };
    uint8_t rt;
};

// branch
struct Branch
{
    uint8_t type;
    bool nop;
    union
    {
        int32_t simm26;
        uint8_t xn;
        struct
        {
            int32_t simm19;
            uint8_t cond;
        };
    };
};

typedef union
{
    struct DPI dpimm;
    struct DPR dpr;
    struct SDT sdt;
    struct Branch br;
} Instruction;

typedef struct 
{
    InstructionType instructionType;
    Instruction instruction;
} Decoded;

//
// Utility Functions
//

void initializeState(void)
{
    state.pstate.Z = true;
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
    file = (!strcmp(filename, "stdout")) ? stdout : fopen(filename, "w");
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

int getBits(uint32_t x, int start, int nbits) {
    uint32_t mask = ((1 << nbits) - 1) << start;
    return (x & mask) >> start;
}

void signExtendTo32Bits(int *value, int nbits)
{
    if (*value & (1 << (nbits - 1)))
    {
        // The number is negative
        int mask = MASK32 ^ ((1 << nbits) - 1);
        *value |= mask;
    }
}

// 1.4 Data Processing Instruction (Immediate)

Decoded decodeDPImm(int instr)
{
    struct DPI dpimm;
    dpimm.rd = getBits(instr, 0, 5);
    dpimm.opi = getBits(instr, 23, 3);
    dpimm.opc = getBits(instr, 29, 2);
    dpimm.sf = getBits(instr, 31, 1);

    // Check for 32-bit or 64-bit mode for Rd
    int64_t mask = MASK32;
    if (dpimm.sf == 0)
        state.R[dpimm.rd] &= mask;

    // Type of data processing operation
    // Only support: arithmetic - 010, widemove - 101
    switch (dpimm.opi)
    {
    case 2: // opi = 010
        dpimm.arithmetic.rn = getBits(instr, 5, 5);
        dpimm.arithmetic.imm12 = getBits(instr, 10, 12);
        dpimm.arithmetic.sh = getBits(instr, 22, 1);
        break;
    case 5: // opi = 101
        dpimm.widemove.imm16 = getBits(instr, 5, 16);
        dpimm.widemove.hw = getBits(instr, 21, 2);
        break;
    default:
        perror("Unsupported type of data processing operation (opi).\n");
        exit(EXIT_FAILURE);
    }

    Instruction instruction = {.dpimm = dpimm};
    Decoded decoded = {isDPI, instruction};
    return decoded;
}

void updateFlagsAdd(int64_t a, int64_t b, bool sf)
{
    int64_t res = a + b;

    // Sign Flag (N)
    state.pstate.N = sf ? (res < 0) : ((int32_t)res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = sf ? ((uint64_t)res < (uint64_t)a)
                        : ((uint32_t)res < (uint32_t)a);
    // Overflow Flag (V)
    state.pstate.V = sf ? (((a > 0) == (b > 0)) && ((res > 0) != (a > 0)))
                        : ((((int32_t)a > 0) == ((int32_t)b > 0)) && (((int32_t)res > 0) != ((int32_t)a > 0)));
}

void updateFlagsSub(int64_t a, int64_t b, bool sf)
{
    int64_t res = a - b;

    // Sign Flag (N)
    state.pstate.N = sf ? (res < 0) : ((int32_t)res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = sf ? ((uint64_t)a >= (uint64_t)b)
                        : ((uint32_t)a >= (uint32_t)b);
    // Overflow Flag (V)
    state.pstate.V = sf ? (((a > 0) != (b > 0)) && ((res > 0) == (b > 0)))
                        : ((((int32_t)a > 0) != ((int32_t)b > 0)) && (((int32_t)res > 0) == ((int32_t)b > 0)));
}

void executeDPImm(Decoded decoded)
{
    struct DPI dpimm = decoded.instruction.dpimm;
    switch (dpimm.opi)
    {
        case 2: // Arithmetic
        {
            int64_t imm12 = ((int64_t)dpimm.arithmetic.imm12) << (12 * dpimm.arithmetic.sh);

            int64_t Rn = (dpimm.arithmetic.rn == 31)
                            ? state.SP
                            : state.R[dpimm.arithmetic.rn];
            // We don't update state.ZR, so we shouldn't point to it.
            int64_t *Rd = (dpimm.rd == 31)
                            ? &state.SP
                            : &state.R[dpimm.rd];
            if (!dpimm.sf)
                Rn &= MASK32;

            switch (dpimm.opc)
            {
                case 0: // Add
                    *Rd = Rn + imm12;
                    break;
                case 1: // Add and set flags
                    if (dpimm.rd != 31)
                        *Rd = Rn + imm12;
                    updateFlagsAdd(Rn, imm12, dpimm.sf);
                    break;
                case 2: // Subtract
                    *Rd = Rn - imm12;
                    break;
                case 3: // Subtract and set flags
                    if (dpimm.rd != 31)
                        *Rd = Rn - imm12;
                    updateFlagsSub(Rn, imm12, dpimm.sf);
                    break;
            }
            
            if (!dpimm.sf)
                *Rd &= MASK32;
            break;
        }
        case 5: // Wide Move
        {
            uint64_t imm16 = ((uint64_t)dpimm.widemove.imm16) << (dpimm.widemove.hw * 16);
            if (dpimm.rd == 31)
                break;
            int64_t *Rd = &state.R[dpimm.rd];

            switch (dpimm.opc)
            {
                case 0: // Move wide with NOT
                    *Rd = ~imm16;
                    if (!dpimm.sf)
                        *Rd &= MASK32;
                    break;
                case 2: // Move wide with zero
                    *Rd = imm16;
                    break;
                case 3: // Move wide with keep
                {
                    int64_t mask = 0XFFFFLL << (dpimm.widemove.hw * 16);
                    *Rd = (*Rd & ~mask) | imm16;
                    if (!dpimm.sf)
                        *Rd &= MASK32;
                    break;
                }
                default:
                    perror("Unsupported Wide Move type.\n");
                    exit(EXIT_FAILURE);
                break;
            }
        }
        default:
        {
            perror("Not supported type of data processing operation (opi).\n");
            exit(EXIT_FAILURE);
        }
    }
}

// 1.5 Data Processing Instruction (Register)

Decoded decodeDPR(int instr)
{
    struct DPR dpr;
    dpr.rd = getBits(instr, 0, 5);
    dpr.rn = getBits(instr, 5, 5);
    dpr.operand = getBits(instr, 10, 6);
    dpr.rm = getBits(instr, 16, 5);
    dpr.opr = getBits(instr, 21, 4);
    dpr.m = getBits(instr, 28, 1);
    dpr.opc = getBits(instr, 29, 2);
    dpr.sf = getBits(instr, 31, 1);

    // Check for 32-bit or 64-bit mode for Rd
    int64_t mask = MASK32;
    if (dpr.sf == 0)
        state.R[dpr.rd] &= mask;

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

    Instruction instruction = {.dpr = dpr};
    Decoded decoded = {isDPR, instruction};
    return decoded;
}

void updateFlagsAnd(int64_t a, int64_t b, bool notB, bool sf)
{
    int64_t res = (notB) ? a & ~b : a & b;

    // Sign Flag (N)
    state.pstate.N = (!sf)
                         ? ((int32_t)(res & MASK32) < 0)
                         : (res < 0);
    // Zero Flag (Z)
    state.pstate.Z = (res == 0);
    // Carry Flag (C)
    state.pstate.C = 0;
    // Overflow Flag (V)
    state.pstate.V = 0;
}

void executeDPR(Decoded decoded)
{
    struct DPR dpr = decoded.instruction.dpr;

    int64_t *Rd = &state.R[dpr.rd];
    int64_t Rm = state.R[dpr.rm];
    int64_t Rn = state.R[dpr.rn];

    if (dpr.m == 0)
    {
        unsigned short shiftMode = (dpr.opr >> 1) % 4;

        if (!dpr.sf)
        {
            Rm &= MASK32;
            Rn &= MASK32;
        }
        // Compute offset
        int64_t op2 = shift(Rm, dpr.operand, shiftMode, dpr.sf);

        // Arithmetic
        if ((dpr.opr >> 3) % 2 == 1)
        {
            // Simulate execution from opc (same as for arithmetic immediate)
            switch (dpr.opc)
            {
                case 0: // Add
                    *Rd = Rn + op2;
                    break;
                case 1: // Add and set flags
                    if (dpr.rd != 31)
                        *Rd = Rn + op2;
                    updateFlagsAdd(Rn, op2, dpr.sf);
                    break;
                case 2: // Subtract
                    *Rd = Rn - op2;
                    break;
                case 3: // Subtract and set flags
                    if (dpr.rd != 31)
                        *Rd = Rn - op2;
                    updateFlagsSub(Rn, op2, dpr.sf);
                    break;
            }
            
            if (!dpr.sf)
                *Rd &= MASK32;
        }
        // Logical
        else
        {
            bool n = dpr.opr % 2;

            // Simulate execution from opc

            switch (dpr.opc)
            {
                case 0: // Bitwise AND and Bit clear
                    *Rd = (!n) ? Rn & op2 : Rn & ~op2;
                    break;
                case 1: // Bitwise inclusive OR and NOR
                    *Rd = (!n) ? Rn | op2 : Rn | ~op2;
                    break;
                case 2: // Bitwise exclusive OR and NOR
                    *Rd = (!n) ? Rn ^ op2 : Rn ^ ~op2;
                    break;
                case 3: // Bitwise AND and Bit clear, setting flags
                    if (dpr.rd != 31)
                        *Rd = (!n) ? Rn & op2 : Rn & ~op2;
                    updateFlagsAnd(Rn, op2, n, dpr.sf);
                    break;
            }
            if (!dpr.sf)
                *Rd &= MASK32;
        }
    }
    else
    {
        unsigned char ra = getBits(dpr.operand, 0, 5);
        bool x = getBits(dpr.operand, 5, 1);

        if (dpr.rd == 31)
            return;

        *Rd = (!x)
                  ? state.R[ra] + (Rn * Rm)  // Multiply-Add
                  : state.R[ra] - (Rn * Rm); // Multiply-Sub
        if (!dpr.sf)
            *Rd &= MASK32;
    }
}

// 1.6 Bitwise Shifts

int64_t shift(int64_t value, unsigned short amount, unsigned short mode, bool nobits)
{
    amount %= (!nobits) ? 32 : 64;

    int32_t value32 = value & MASK32;

    switch (mode)
    {
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
        case 3: // Rotate Right (ror)
        {
            short int rotateAmount = (!nobits) ? 32 - amount
                                            : 64 - amount;
            int64_t bitsToRotate = value << rotateAmount;

            return (!nobits) ? (((uint32_t)value32 >> amount) | bitsToRotate) & MASK32
                            : ((uint64_t)value >> amount) | bitsToRotate;
            break;
        }
        default:
            perror("Unsupported shift mode. Provide a mode between 0 and 3.");
            exit(EXIT_FAILURE);
    }
}

// 1.7 Single Data Transfer Instructions

Decoded decodeSDT(int instr)
{
    struct SDT sdt;
    sdt.rt = getBits(instr, 0, 5);
    sdt.mode = getBits(instr, 29, 1);
    sdt.sf = getBits(instr, 30, 1);

    // Type of addressing mode
    // Support: single data transfer - 1, load literal - 0

    // Single Data Transfer
    if (sdt.mode == 1)
    {
        sdt.xn = getBits(instr, 5, 5);
        unsigned short offset = getBits(instr, 10, 12);
        sdt.l = getBits(instr, 22, 1);
        sdt.u = getBits(instr, 24, 1);

        sdt.offmode = getBits(instr, 21, 1);

        // Unsigned Immediate Offset
        if (sdt.u == 1)
            sdt.addrm.imm12 = offset;
        else
        {
            // Pre/Post - Index
            if (sdt.offmode == 0)
            {
                sdt.addrm.i = getBits(instr, 11, 1);
                sdt.addrm.simm9 = getBits(instr, 12, 9);
                signExtendTo32Bits(&sdt.addrm.simm9, 9);
            }
            // Register Offset
            else
                sdt.addrm.xm = getBits(instr, 16, 5);
        }
    }
    // Load Literal
    else
    {
        sdt.simm19 = getBits(instr, 5, 19);
        signExtendTo32Bits(&sdt.simm19, 19);
    }

    Instruction instruction = {.sdt = sdt};
    Decoded decoded = {isSDT, instruction};
    return decoded;
}

void loadFromMemory(int addr, int64_t *reg, bool mode)
{
    int64_t result = 0;
    int bytes = mode * 4 + 4;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    for (int i = 0; i < bytes; i++)
        result |= ((int64_t)mem[addr + i]) << (8 * i);
    // This is also transofrmed from little endian
    *reg = result;
}

void storeToMemory(int addr, int64_t reg, bool mode)
{
    int bytes = mode * 4 + 4;
    // Mode: 0 -> 32-bits,  1 -> 64-bits
    for (int i = 0; i < bytes; i++)
        mem[addr + i] = (reg >> (i * 8)) & 0xFF;
    // This is also transofrmed from little endian
}

void executeSDT(Decoded decoded)
{
    struct SDT sdt = decoded.instruction.sdt;

    // Although xn is 64 bits, addresses can only be 21 bits, so trim to int.
    int targetAddress;

    // Single Data Transfer
    if (sdt.mode == 1)
    {
        targetAddress = (sdt.xn == 31)
                            ? state.SP
                            : state.R[sdt.xn];

        // Unsigned Immediate Offset
        if (sdt.u == 1)
        {
            unsigned short uoffset = (!sdt.sf)
                                         ? sdt.addrm.imm12 * 4
                                         : sdt.addrm.imm12 * 8;
            targetAddress += uoffset;
        }
        else
        {
            // Pre/Post - Index
            if (sdt.offmode == 0)
            {
                targetAddress += (sdt.addrm.i) ? sdt.addrm.simm9 : 0;
                if (sdt.xn == 31)
                    state.SP += (int64_t)sdt.addrm.simm9;
                else
                    state.R[sdt.xn] += (int64_t)sdt.addrm.simm9;
            }
            // Register Offset
            else
                targetAddress += state.R[sdt.addrm.xm];
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
            storeToMemory(targetAddress, state.R[sdt.rt], sdt.sf);
        }
    }
    // Load Literal
    else
    {
        targetAddress = state.PC + ((int64_t)sdt.simm19) * 4;

        // Simulate the Data Transfer
        loadFromMemory(targetAddress, &state.R[sdt.rt], sdt.sf);
    }
}

// 1.8 Branch Instructions

Decoded decodeB(int instr)
{
    struct Branch br;
    br.type = getBits(instr, 30, 2);

    // Type of branch
    // Supported: unconditional - 00, register - 11, conditional - 01
    switch (br.type)
    {
        case 0: // Unconditional
            br.simm26 = getBits(instr, 0, 26);
            signExtendTo32Bits(&br.simm26, 26);
            break;
        case 1: // Conditional
            br.cond = getBits(instr, 0, 4);
            br.simm19 = getBits(instr, 5, 19);
            signExtendTo32Bits(&br.simm19, 19);
            break;
        case 3: // Register
            br.xn = getBits(instr, 5, 5);
            br.nop = getBits(instr, 20, 1);
            break;
        default:
            perror("Not supported type of branching.\n");
            exit(EXIT_FAILURE);
    }
    
    Instruction instruction = {.br = br};
    Decoded decoded = {isBranch, instruction};
    return decoded;
}

void executeB(Decoded decoded)
{
    struct Branch br = decoded.instruction.br;
    switch (br.type)
    {
        case 0: // Unconditional
            state.PC += ((int64_t)br.simm26) * 4;
            break;
        case 1: // Conditional
        {
            bool toBranch;
            switch (br.cond)
            {
                case 0: // EQ (equal) - 0000
                    toBranch = state.pstate.Z;
                    break;
                case 1: // NE (not equal) - 0001
                    toBranch = !state.pstate.Z;
                    break;
                case 10: // GE (greater or equal) - 1010
                    toBranch = (state.pstate.N == state.pstate.V);
                    break;
                case 11: // LT (less) - 1011
                    toBranch = (state.pstate.N != state.pstate.V);
                    break;
                case 12: // GT (greater) - 1100
                    toBranch = (!state.pstate.Z && (state.pstate.N == state.pstate.V));
                    break;
                case 13: // LE (less or equal) - 1101
                    toBranch = (state.pstate.Z || (state.pstate.N != state.pstate.V));
                    break;
                case 14: // AL (always) - 1110
                    toBranch = true;
                    break;
                default:
                    perror("Unsupported condition branch mnemonic.");
                    exit(EXIT_FAILURE);
                    break;
            }
            if (toBranch)
                state.PC += ((int64_t)br.simm19) * 4;
            else
                state.PC += 4;
            break;
        }
        case 3: // Register
            if (br.nop)
            {
                state.PC = (br.xn == 31)
                            ? state.ZR
                            : state.R[br.xn];
            }
            else // Nop
                state.PC += 4;
            break;
        default:
            perror("Not supported type of data processing opeartion (opi).\n");
            exit(EXIT_FAILURE);
            break;
    }
}

//
// Pipeline Stages
//
int fetch(void)
{
    // Fetch instruction from memory
    // Compute the address of the next instruction
    // We update PC after execution stage

    return instructionToInt(state.PC);
}

Decoded decode(int instr)
{
    uint8_t op0 = getBits(instr, 25, 4);
    switch (op0)
    {
    // Data Processing Immediate - 100x
    case 8:
    case 9:
        return decodeDPImm(instr);
    // Data Processing Register - x101
    case 5:
    case 13:
        return decodeDPR(instr);
    // Load and Store - x1x0
    case 4:
    case 6:
    case 12:
    case 14:
        return decodeSDT(instr);
    // Branch - 101x
    case 10:
    case 11:
        return decodeB(instr);
    default:
        perror("Unsupported opcode.\n");
        exit(EXIT_FAILURE);
    }
}

void execute(Decoded decoded) 
{
    switch (decoded.instructionType) 
    {
        case isDPI:
            executeDPImm(decoded);
            break;
        case isDPR:
            executeDPR(decoded);
            break;
        case isSDT:
            executeSDT(decoded);
            break;
        case isBranch:
            executeB(decoded);
            break;
        default:
            perror("Unsupported instruction type.\n");
            exit(EXIT_FAILURE);
    }
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

    // halt is 0x8a000000
    while ((instr = fetch()) != 0x8a000000LL)
    {
        Decoded decoded = decode(instr);
        execute(decoded);

        // Update address of next instruction
        if (decoded.instructionType != isBranch)
            state.PC += 4;
    }

    // Write the final state after executing all instructions
    writeOutput(outputFile);
    return EXIT_SUCCESS;
}
