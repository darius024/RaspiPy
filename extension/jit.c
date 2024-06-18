#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ir.h"
#include "../src/constants.h"
#include "../src/instructions.h"
#include "../src/utils_as.h"

int registers[NUM_REGISTERS] = {NOT_USED};

uint32_t generateInstruction(IRInstruction *instruction) 
{
    uint32_t out = 0;
    switch (instruction->type) {
        case IR_ADD:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, ADD, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 1, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_SUB:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, SUB, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 1, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_CMP:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, SUB_SETFLAGS, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 1, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_NEG:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, SUB, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 1, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->dest, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_MUL:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, 1, DPR_M_OFFSET, DPR_M_LEN);
            putBits(&out, DPR_MUL, DPR_OPR_OFFSET, DPR_OPR_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_AND:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, BITWISE_AND, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        // case IR_BIC:
        //     putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
        //     putBits(&out, BITWISE_AND, DPR_OPC_OFFSET, DPR_OPC_LEN);
        //     putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
        //     putBits(&out, 1, DPR_N_OFFSET, DPR_N_LEN);
        //     putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
        //     putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
        //     putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
        //     break;
        case IR_ORR:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, BITWISE_OR, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_EOR:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, BITWISE_XOR, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_TST:
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, BITWISE_AND_SETFLAGS, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src2, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->src1, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        // case IR_WIDEMOVE:
        //     putBits(&out, OP0_DPI, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, 1, DPI_SF_OFFSET, DPI_SF_LEN);
        //     putBits(&out, MOVE_WITH_ZERO, DPI_OPC_OFFSET, DPI_OPC_LEN);
        //     putBits(&out, WIDEMOVE, DPI_OPI_OFFSET, DPI_OPI_LEN);
        //     putBits(&out, instruction->src1, DPI_IMM16_OFFSET, DPI_IMM16_LEN);
        //     putBits(&out, instruction->dest, DPI_RD_OFFSET, DPI_RD_LEN);
        //     break;
        case IR_MOV: // same as IR_OR
            putBits(&out, OP0_DPR, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, BITWISE_OR, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, 0, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, instruction->src1, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, instruction->dest, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_B:
            putBits(&out, OP0_B, OP0_OFFSET, OP0_LEN);
            putBits(&out, BRANCH_UNCONDITIONAL, B_TYPE_OFFSET, B_TYPE_LEN);
            putBits(&out, instruction->dest, B_SIMM26_OFFSET, B_SIMM26_LEN);
            break;
        // case IR_BCOND:
        //     putBits(&out, OP0_B, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, BRANCH_CONDITIONAL, B_TYPE_OFFSET, B_TYPE_LEN);
        //     putBits(&out, instruction->dest, B_SIMM19_OFFSET, B_SIMM19_LEN);
        //     putBits(&out, instruction->src1, B_TAG_OFFSET, B_TAG_LEN);
        //     break;
        case IR_BR:
            putBits(&out, OP0_B, OP0_OFFSET, OP0_LEN);
            putBits(&out, BRANCH_REGISTER, B_TYPE_OFFSET, B_TYPE_LEN);
            putBits(&out, B_BIT, B_BIT_OFFSET, B_BIT_LEN);
            putBits(&out, B_REG, B_REG_OFFSET, B_REG_LEN);
            putBits(&out, instruction->dest, B_XN_OFFSET, B_XN_LEN);
            break;
        case IR_LDR:
            putBits(&out, OP0_SDT, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, SDT_MODE_OFFSET, SDT_MODE_LEN);
            putBits(&out, 1, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
            putBits(&out, 1, SDT_L_OFFSET, SDT_L_LEN);
            putBits(&out, instruction->src1, SDT_XN_OFFSET, SDT_XN_OFFSET);
            putBits(&out, instruction->dest, SDT_RT_OFFSET, SDT_RT_LEN);
            break;
        case IR_STR:
            putBits(&out, OP0_SDT, OP0_OFFSET, OP0_LEN);
            putBits(&out, 1, SDT_MODE_OFFSET, SDT_MODE_LEN);
            putBits(&out, 1, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
            putBits(&out, 0, SDT_L_OFFSET, SDT_L_LEN);
            putBits(&out, instruction->src1, SDT_XN_OFFSET, SDT_XN_OFFSET);
            putBits(&out, instruction->dest, SDT_RT_OFFSET, SDT_RT_LEN);
            break;
        case IR_DIR:
            // TODO
            break;
        default:
            break;
    }
    return out;
}


int main()
{
    return EXIT_SUCCESS;
}
