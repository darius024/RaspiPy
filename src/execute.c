#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "datatypes_em.h"
#include "execute.h"
#include "utils_em.h"
#include "structs.h"


extern struct EmulatorState state;
static int shift(int64_t value, int64_t *op, int8_t amount, uint8_t mode, bool nbits);

static void updatePC(void)
{
    state.PC += INSTR_BYTES;
}

static void updateFlagsArithmetic(int64_t a, int64_t b, bool sf, bool isAdd)
{
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

static void updateFlagsAnd(int64_t a, int64_t b, bool sf)
{
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

// Arithmetic instructions in DPI and DPR
static int addOrSub(uint8_t opc, uint8_t rd, bool sf, int64_t *Rd, int64_t a, int64_t b)
{
    switch (opc) {
        case ADD: // Add
            *Rd = a + b;
            break;
        case ADD_SETFLAGS: // Add and set flags
            if (rd != ZR_SP) {
                *Rd = a + b;
            }
            updateFlagsArithmetic(a, b, sf, true);
            break;
        case SUB: // Subtract
            *Rd = a - b;
            break;
        case SUB_SETFLAGS: // Subtract and set flags
            if (rd != ZR_SP) {
                *Rd = a - b;
            }
            updateFlagsArithmetic(a, b, sf, false);
            break;
        default:
            EXIT_PROGRAM("Unsupported opc of data processing operation.");
    }
    return EXIT_SUCCESS;
}

static void loadFromMemory(int addr, int64_t *reg, bool sf)
{
    int64_t result = 0;
    int bytes = (sf) ? MODE64_BYTES : MODE32_BYTES;
    for (int i = 0; i < bytes; i++) {
        result |= ((int64_t)state.mem[addr + i]) << (BYTE_SIZE * i);
    }
    *reg = result;
    // The value is read from little endian memory
}

static void storeToMemory(int addr, int64_t reg, bool sf)
{
    int bytes = (sf) ? MODE64_BYTES : MODE32_BYTES;
    for (int i = 0; i < bytes; i++) {
        state.mem[addr + i] = (reg >> (BYTE_SIZE * i)) & MASK8;
    }
    // The value is read from little endian memory
}

// 1.4 Data Processing Instruction (Immediate)
static int executeDPI(Instruction instruction)
{
    struct DPI dpi = instruction.dpi;
    int64_t *Rd = (dpi.rd == ZR_SP) ? &state.SP : &state.R[dpi.rd];
    
    switch (dpi.opi) {
        case ARITHMETIC: { // Arithmetic
            int64_t imm12 = ((int64_t)dpi.imm12) << (dpi.sh * ARITHMETIC_SHIFT);
            int64_t Rn = (dpi.rn == ZR_SP) ? state.SP : state.R[dpi.rn];
            maskTo32Bits(dpi.sf, &Rn);
            addOrSub(dpi.opc, dpi.rd, dpi.sf, Rd, Rn, imm12);
            break;
        }
        case WIDEMOVE: { // Wide Move
            if (dpi.rd == ZR_SP) {
                break;
            }
            uint64_t imm16 = ((uint64_t)dpi.imm16) << (dpi.hw * WIDEMOVE_SHIFT);
            switch (dpi.opc) {
                case MOVE_WITH_NOT: // Move wide with not
                    *Rd = ~imm16;
                    break;
                case MOVE_WITH_ZERO: // Move wide with zero
                    *Rd = imm16;
                    break;
                case MOVE_WITH_KEEP: { // Move wide with keep
                    int64_t mask = MASK16 << (dpi.hw * WIDEMOVE_SHIFT);
                    *Rd = (*Rd & ~mask) | imm16;
                    break;
                }
                default:
                    EXIT_PROGRAM("Unsupported wide move type (bits 29-30), use either 00, 10 or 11.");
            }
            break;
        }
        default:
            EXIT_PROGRAM("Unsupported opi (bits 23-25), use either 010 or 101.");
    }
    maskTo32Bits(dpi.sf, Rd);
    updatePC();
    return EXIT_SUCCESS;
}

// 1.5 Data Processing Instruction (Register)
static int executeDPR(Instruction instruction) {
    struct DPR dpr = instruction.dpr;
    int64_t *Rd = &state.R[dpr.rd];
    int64_t Rm = (dpr.rm != ZR_SP) ? state.R[dpr.rm] : state.ZR;
    int64_t Rn = (dpr.rm != ZR_SP) ? state.R[dpr.rn] : state.ZR;

    maskTo32Bits(dpr.sf, &Rm);
    maskTo32Bits(dpr.sf, &Rn);

    if (dpr.m == 0) {
        // Compute offset
        int64_t op2;
        shift(Rm, &op2, dpr.operand, dpr.shift, dpr.sf);

        if (dpr.armOrLog == 1) { // Arithmetic
            addOrSub(dpr.opc, dpr.rd, dpr.sf, Rd, Rn, op2);
        } else { // Logical
            if (dpr.n == 1) {
                op2 = ~op2;
            }
            switch (dpr.opc) {
                case BITWISE_AND: // Bitwise AND and Bit clear
                    *Rd = Rn & op2;
                    break;
                case BITWISE_OR: // Bitwise inclusive OR and NOR
                    *Rd = Rn | op2;
                    break;
                case BITWISE_XOR: // Bitwise exclusive OR and NOR
                    *Rd = Rn ^ op2;
                    break;
                case BITWISE_AND_SETFLAGS: // Bitwise AND and Bit clear, setting flags
                    if (dpr.rd != ZR_SP) {
                        *Rd = Rn & op2;
                    }
                    updateFlagsAnd(Rn, op2, dpr.sf);
                    break;
            }
        }
    } else { // Multiply
        if (dpr.rd != ZR_SP) {
            int64_t Ra = (dpr.ra != ZR_SP) ? state.R[dpr.ra] : state.ZR;
            *Rd = (dpr.x == 0) ? Ra + (Rn * Rm)  // Multiply-Add
                               : Ra - (Rn * Rm); // Multiply-Sub
        }
    }
    maskTo32Bits(dpr.sf, Rd);
    updatePC();
    return EXIT_SUCCESS;
}

// 1.6 Bitwise Shifts
static int shift(int64_t value, int64_t *op, int8_t amount, uint8_t mode, bool nbits) {
    amount %= (nbits) ? MODE64 : MODE32;

    switch (mode) {
        case LOGICAL_SHIFT_LEFT: // Logical Shift Left (lsl)
            *op = (nbits) ? (value << amount)
                          : ((int32_t)value << amount) & MASK32;
            break;
        case LOGICAL_SHIFT_RIGHT: // Logical Shift Right (lsr)
            *op = (nbits) ? (uint64_t)value >> amount
                          : ((uint32_t)value >> amount) & MASK32;
            break;
        case ARITHMETIC_SHIFT_RIGHT: // Arithmetic Shift Right (asr)
            *op = (nbits) ? value >> amount
                          : ((int32_t)value >> amount) & MASK32;
            break;
        case ROTATE_RIGHT: { // Rotate Right (ror)
            *op = (nbits) ? ((uint64_t)value >> amount) | value << (MODE64 - amount)
                          : (((uint32_t)value >> amount) | value << (MODE32 - amount)) & MASK32;
            break;
        }
        default:
            EXIT_PROGRAM("Unsupported shift mode, provide a mode between 0 and 3.");
    }
    return EXIT_SUCCESS;
}

// 1.7 Single Data Transfer Instruction
static int executeSDT(Instruction instruction) {
    struct SDT sdt = instruction.sdt;
    uint32_t targetAddress;

    maskTo32Bits(sdt.sf, &state.R[sdt.rt]);

    if (sdt.mode == 1) { // Single Data Transfer
        int64_t *Xn = (sdt.xn == ZR_SP) ? &state.SP : &state.R[sdt.xn];
        targetAddress = *Xn;

        if (sdt.u == 1) { // Unsigned Immediate Offset
            uint16_t uoffset = sdt.imm12 * ((sdt.sf) ? MODE64_BYTES : MODE32_BYTES);
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
    updatePC();
    return EXIT_SUCCESS;
}

// 1.8 Branch Instruction
static int executeB(Instruction instruction) {
    struct B b = instruction.b;

    switch (b.type) {
        case BRANCH_UNCONDITIONAL: // Unconditional
            state.PC += ((int64_t)b.simm26) * INSTR_BYTES;
            break;
        case BRANCH_CONDITIONAL: { // Conditional
            bool toBranch;
            switch (b.cond.tag) {
                case EQ_NE_TAG: // EQ (equal) - 0000, NE (not equal) - 0001
                    toBranch = state.pstate.Z;
                    break;
                case GE_LT_TAG: // GE (greater or equal) - 1010, LT (less) - 1011
                    toBranch = (state.pstate.N == state.pstate.V);
                    break;
                case GT_LE_TAG: // GT (greater) - 1100, LE (less or equal) - 1101
                    toBranch = (!state.pstate.Z && (state.pstate.N == state.pstate.V));
                    break;
                case ALWAYS_TAG: // AL (always) - 1110
                    toBranch = true;
                    break;
                default:
                    EXIT_PROGRAM("Unsupported branch condition (bits 1-3), use either 000, 101, 110 or 111.");
            }
            if (toBranch ^ b.cond.neg) {
                state.PC += ((int64_t)b.simm19) * INSTR_BYTES;
            } else {
                updatePC();
            }
            break;
        }
        case BRANCH_REGISTER: // Register
            state.PC = (b.xn == ZR_SP) ? state.ZR : state.R[b.xn];
            break;
        default:
            EXIT_PROGRAM("Unsupported branch type (bits 30-31), use either 00, 01 or 11.");
    }
    return EXIT_SUCCESS;
}

// Execute the instruction and apply changes to the state
int execute(Instruction instruction)
{
    switch (instruction.instructionType) {
        case isDPI:
            return executeDPI(instruction);
        case isDPR:
            return executeDPR(instruction);
        case isSDT:
            return executeSDT(instruction);
        case isB:
            return executeB(instruction);
        default:
            EXIT_PROGRAM("Unsupported instruction type.");
    }
}