#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "hotspot.h"
#include "ir.h"
#include "state.h"
#include "../src/constants.h"
#include "../src/instructions.h"
#include "../src/utils_as.h"

int registers[NUM_REGISTERS] = {NOT_USED};

uint32_t generateInstruction(IRInstruction *instruction) 
{
    // Check for hotspot - C to assembly to binary
    if (instruction->count >= THRESHOLD) {
        return returnHotSpot(instruction->line);
    }
    // No hotspot - C to binary
    uint32_t out = 0;
    switch (instruction->type) {
        case IR_ADD:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){ADD}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_SUB:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){SUB}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_CMP:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){SUB_SETFLAGS}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_NEG:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){SUB}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_MUL:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){1}, DPR_M_OFFSET, DPR_M_LEN);
            putBits(&out, &(int){DPR_MUL}, DPR_OPR_OFFSET, DPR_OPR_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_AND:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){BITWISE_AND}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        // case IR_BIC:
        //     putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
        //     putBits(&out, &(int){BITWISE_AND}, DPR_OPC_OFFSET, DPR_OPC_LEN);
        //     putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
        //     putBits(&out, &(int){1}, DPR_N_OFFSET, DPR_N_LEN);
        //     putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
        //     putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
        //     putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
        //     break;
        case IR_ORR:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){BITWISE_OR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_EOR:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){BITWISE_XOR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_TST:
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){BITWISE_AND_SETFLAGS}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        // case IR_WIDEMOVE:
        //     putBits(&out, &(int){OP0_DPI}, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, &(int){1}, DPI_SF_OFFSET, DPI_SF_LEN);
        //     putBits(&out, &(int){MOVE_WITH_ZERO}, DPI_OPC_OFFSET, DPI_OPC_LEN);
        //     putBits(&out, &(int){WIDEMOVE}, DPI_OPI_OFFSET, DPI_OPI_LEN);
        //     putBits(&out, &(int){instruction->src1->reg}, DPI_IMM16_OFFSET, DPI_IMM16_LEN);
        //     putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPI_RD_LEN);
        //     break;
        case IR_MOV: // same as IR_OR
            putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
            putBits(&out, &(int){BITWISE_OR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
            putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
            putBits(&out, &(int){instruction->src1->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
            putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
            break;
        case IR_B:
            putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){BRANCH_UNCONDITIONAL}, B_TYPE_OFFSET, B_TYPE_LEN);
            putBits(&out, &(int){instruction->dest->reg}, B_SIMM26_OFFSET, B_SIMM26_LEN);
            break;
        // case IR_BCOND:
        //     putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
        //     putBits(&out, &(int){BRANCH_CONDITIONAL}, B_TYPE_OFFSET, B_TYPE_LEN);
        //     putBits(&out, &(int){instruction->dest->reg}, B_SIMM19_OFFSET, B_SIMM19_LEN);
        //     putBits(&out, &(int){instruction->src1->reg}, B_TAG_OFFSET, B_TAG_LEN);
        //     break;
        case IR_BR:
            putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){BRANCH_REGISTER}, B_TYPE_OFFSET, B_TYPE_LEN);
            putBits(&out, &(int){B_BIT}, B_BIT_OFFSET, B_BIT_LEN);
            putBits(&out, &(int){B_REG}, B_REG_OFFSET, B_REG_LEN);
            putBits(&out, &(int){instruction->dest->reg}, B_XN_OFFSET, B_XN_LEN);
            break;
        case IR_LDR:
            putBits(&out, &(int){OP0_SDT}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, SDT_MODE_OFFSET, SDT_MODE_LEN);
            putBits(&out, &(int){1}, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
            putBits(&out, &(int){1}, SDT_L_OFFSET, SDT_L_LEN);
            putBits(&out, &(int){instruction->src1->reg}, SDT_XN_OFFSET, SDT_XN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, SDT_RT_OFFSET, SDT_RT_LEN);
            break;
        case IR_STR:
            putBits(&out, &(int){OP0_SDT}, OP0_OFFSET, OP0_LEN);
            putBits(&out, &(int){1}, SDT_MODE_OFFSET, SDT_MODE_LEN);
            putBits(&out, &(int){1}, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
            putBits(&out, &(int){0}, SDT_L_OFFSET, SDT_L_LEN);
            putBits(&out, &(int){instruction->src1->reg}, SDT_XN_OFFSET, SDT_XN_LEN);
            putBits(&out, &(int){instruction->dest->reg}, SDT_RT_OFFSET, SDT_RT_LEN);
            break;
        default:
            break;
    }
    return out;
}

int main()
{
    // TODO: add main function, check imm or reg in the above
    return EXIT_SUCCESS;
}
