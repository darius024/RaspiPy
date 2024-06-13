#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "constants.h"
#include "decoders.h"
#include "instructions.h"
#include "utils_em.h"


int decodeDPI(uint32_t *instr, Instruction *instruction, BitFunc bitFunc)
{
    instruction->instructionType = isDPI;
    struct DPI *dpi = &(instruction->dpi);

    bitFunc(instr, &(dpi->sf), DPI_SF_OFFSET, DPI_SF_LEN);
    bitFunc(instr, &(dpi->opc), DPI_OPC_OFFSET, DPI_OPC_LEN);
    bitFunc(instr, &(dpi->opi), DPI_OPI_OFFSET, DPI_OPI_LEN);
    bitFunc(instr, &(dpi->rd), DPI_RD_OFFSET, DPI_RD_LEN);

    // Type of data processing operation
    switch (dpi->opi) {
        case ARITHMETIC: // Arithmetic
            bitFunc(instr, &(dpi->sh), DPI_SH_OFFSET, DPI_SH_LEN);
            bitFunc(instr, &(dpi->imm12), DPI_IMM12_OFFSET, DPI_IMM12_LEN);
            bitFunc(instr, &(dpi->rn), DPI_RN_OFFSET, DPI_RN_LEN);
            break;
        case WIDEMOVE: // Wide Move
            bitFunc(instr, &(dpi->hw), DPI_HW_OFFSET, DPI_HW_LEN);
            bitFunc(instr, &(dpi->imm16), DPI_IMM16_OFFSET, DPI_IMM16_LEN);
            break;
        default:
            EXIT_PROGRAM("Unsupported opi (bits 23-25), use either 010 or 101.");
    }
    return EXIT_SUCCESS;
}

int decodeDPR(uint32_t *instr, Instruction *instruction, BitFunc bitFunc)
{
    instruction->instructionType = isDPR;
    struct DPR *dpr = &(instruction->dpr);

    bitFunc(instr, &(dpr->sf), DPR_SF_OFFSET, DPR_SF_LEN);
    bitFunc(instr, &(dpr->opc), DPR_OPC_OFFSET, DPR_OPC_LEN);
    bitFunc(instr, &(dpr->m), DPR_M_OFFSET, DPR_M_LEN);
    bitFunc(instr, &(dpr->rm), DPR_RM_OFFSET, DPR_RM_LEN);
    bitFunc(instr, &(dpr->rn), DPR_RN_OFFSET, DPR_RN_LEN);
    bitFunc(instr, &(dpr->rd), DPR_RD_OFFSET, DPR_RD_LEN);

    // Type of data processing operation
    if (dpr->m == 0) { // Arithmetic, Bit-logic
        bitFunc(instr, &(dpr->armOrLog), DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
        bitFunc(instr, &(dpr->shift), DPR_SHIFT_OFFSET, DPR_SHIFT_LEN);
        bitFunc(instr, &(dpr->n), DPR_N_OFFSET, DPR_N_LEN);
        bitFunc(instr, &(dpr->operand), DPR_OPERAND_OFFSET, DPR_OPERAND_LEN);
    }
    else { // Multiply
        bitFunc(instr, &(dpr->opr), DPR_OPR_OFFSET, DPR_OPR_LEN);
        bitFunc(instr, &(dpr->x), DPR_X_OFFSET, DPR_X_LEN);
        bitFunc(instr, &(dpr->ra), DPR_RA_OFFSET, DPR_RA_LEN);
    }
    return EXIT_SUCCESS;
}

int decodeSDT(uint32_t *instr, Instruction *instruction, BitFunc bitFunc)
{
    instruction->instructionType = isSDT;
    struct SDT *sdt = &(instruction->sdt);

    bitFunc(instr, &(sdt->mode), SDT_MODE_OFFSET, SDT_MODE_LEN);
    bitFunc(instr, &(sdt->sf), SDT_SF_OFFSET, SDT_SF_LEN);
    bitFunc(instr, &(sdt->mode), SDT_MODE_OFFSET2, SDT_MODE_LEN2);
    bitFunc(instr, &(sdt->rt), SDT_RT_OFFSET, SDT_RT_LEN);

    // Type of addressing mode
    if (sdt->mode == 1) { // Single Data Transfer
        bitFunc(instr, &(sdt->u), SDT_U_OFFSET, SDT_U_LEN);
        bitFunc(instr, &(sdt->l), SDT_L_OFFSET, SDT_L_LEN);
        bitFunc(instr, &(sdt->offmode), SDT_OFFMODE_OFFSET, SDT_OFFMODE_LEN);
        bitFunc(instr, &(sdt->xn), SDT_XN_OFFSET, SDT_XN_LEN);

        if (sdt->u == 1) { // Unsigned Immediate Offset
            bitFunc(instr, &(sdt->imm12), SDT_IMM12_OFFSET, SDT_IMM12_LEN);
        } else if (sdt->offmode == 0) { // Pre/Post - Index
            bitFunc(instr, &(sdt->simm9), SDT_IMM9_OFFSET, SDT_IMM9_LEN);
            signExtendTo32Bits(&(sdt->simm9), SDT_IMM9_LEN);
            bitFunc(instr, &(sdt->i), SDT_I_OFFSET, SDT_I_LEN);
            bitFunc(instr, &(sdt->bit), SDT_BIT_OFFSET, SDT_BIT_LEN);
        } else { // Register Offset
            bitFunc(instr, &(sdt->xm), SDT_XM_OFFSET, SDT_XM_LEN);
            bitFunc(instr, &(sdt->roff1), SDT_ROFF1_OFFSET, SDT_ROFF1_LEN);
            bitFunc(instr, &(sdt->roff2), SDT_ROFF2_OFFSET, SDT_ROFF2_LEN);
        }
    }
    else { // Load Literal
        bitFunc(instr, &(sdt->simm19), SDT_SIMM19_OFFSET, SDT_SIMM19_LEN);
        signExtendTo32Bits(&(sdt->simm19), SDT_SIMM19_LEN);
    }
    return EXIT_SUCCESS;
}

int decodeB(uint32_t *instr, Instruction *instruction, BitFunc bitFunc)
{
    instruction->instructionType = isB;
    struct B *b = &(instruction->b);

    bitFunc(instr, &(b->type), B_TYPE_OFFSET, B_TYPE_LEN);

    // Type of branch
    switch (b->type) {
        case BRANCH_UNCONDITIONAL: // Unconditional
            bitFunc(instr, &(b->simm26), B_SIMM26_OFFSET, B_SIMM26_LEN);
            signExtendTo32Bits(&(b->simm26), B_SIMM26_LEN);
            break;
        case BRANCH_CONDITIONAL: // Conditional
            bitFunc(instr, &(b->simm19), B_SIMM19_OFFSET, B_SIMM19_LEN);
            signExtendTo32Bits(&(b->simm19), B_SIMM19_LEN);
            bitFunc(instr, &(b->cond.tag), B_TAG_OFFSET, B_TAG_LEN);
            bitFunc(instr, &(b->cond.neg), B_NEG_OFFSET, B_NEG_LEN);
            break;
        case BRANCH_REGISTER: // Register
            bitFunc(instr, &(b->bit), B_BIT_OFFSET, B_BIT_LEN);
            bitFunc(instr, &(b->reg), B_REG_OFFSET, B_REG_LEN);
            bitFunc(instr, &(b->xn), B_XN_OFFSET, B_XN_LEN);
            break;
        default:
            EXIT_PROGRAM("Unsupported branch type (bits 30-31), use either 00, 01 or 11.");
    }
    return EXIT_SUCCESS;
}

static int setOp0(Instruction *instruction) {
    switch(instruction->instructionType) {
        case isDPI:
            return OP0_DPI;
        case isDPR:
            return OP0_DPR;
        case isSDT:
            return OP0_SDT;
        case isB:
            return OP0_B;
        default:
            EXIT_PROGRAM("Can't set the right opcode (op0).");
    }
}

// Generic decode function
int decode(uint32_t *instr, Instruction *instruction, BitFunc bitFunc)
{
    uint8_t op0 = setOp0(instruction);
    bitFunc(instr, &op0, OP0_OFFSET, OP0_LEN);

    if (OP0_IS_DPI(op0)) { // 100x - Data Processing Immediate
        return decodeDPI(instr, instruction, bitFunc);
    } else if (OP0_IS_DPR(op0)) { // x101 - Data Processing Register
        return decodeDPR(instr, instruction, bitFunc);
    } else if (OP0_IS_SDT(op0)) { // x1x0 - Loads and Stores
        return decodeSDT(instr, instruction, bitFunc);
    } else if (OP0_IS_B(op0)) { // 101x - Branch
        return decodeB(instr, instruction, bitFunc);
    } else {
        EXIT_PROGRAM("Unsupported op0 (bits 25-28), use either 100x, x101, x1x0 or 101x.");
    }
}