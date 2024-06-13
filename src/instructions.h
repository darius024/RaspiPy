// Constants for the instruction subset

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define OP0_OFFSET 25
#define OP0_LEN 4


#define DPI_SF_OFFSET 31
#define DPI_OPC_OFFSET 29
#define DPI_OPI_OFFSET 23
#define DPI_RD_OFFSET 0

#define DPI_SH_OFFSET 22
#define DPI_IMM12_OFFSET 10
#define DPI_RN_OFFSET 5

#define DPI_HW_OFFSET 21
#define DPI_IMM16_OFFSET 5

#define DPI_SF_LEN 1
#define DPI_OPC_LEN 2
#define DPI_OPI_LEN 3
#define DPI_RD_LEN 5

#define DPI_SH_LEN 1
#define DPI_IMM12_LEN 12
#define DPI_RN_LEN 5

#define DPI_HW_LEN 2
#define DPI_IMM16_LEN 16


#define DPR_SF_OFFSET 31
#define DPR_M_OFFSET 28
#define DPR_RM_OFFSET 16
#define DPR_OPERAND_OFFSET 10
#define DPR_RN_OFFSET 5
#define DPR_RD_OFFSET 0

#define DPR_OPC_OFFSET 29
#define DPR_ARMORLOG_OFFSET 24
#define DPR_SHIFT_OFFSET 22
#define DPR_N_OFFSET 21

#define DPR_OPR_OFFSET 21
#define DPR_X_OFFSET 15
#define DPR_RA_OFFSET 10

#define DPR_SF_LEN 1
#define DPR_M_LEN 1
#define DPR_RM_LEN 5
#define DPR_OPERAND_LEN 6
#define DPR_RN_LEN 5
#define DPR_RD_LEN 5

#define DPR_OPC_LEN 2
#define DPR_ARMORLOG_LEN 1
#define DPR_SHIFT_LEN 2
#define DPR_N_LEN 1

#define DPR_OPR_LEN 4
#define DPR_X_LEN 1
#define DPR_RA_LEN 5


#define SDT_MODE_OFFSET 31
#define SDT_SF_OFFSET 30
#define SDT_MODE_OFFSET2 29
#define SDT_RT_OFFSET 0

#define SDT_U_OFFSET 24
#define SDT_L_OFFSET 22
#define SDT_OFFMODE_OFFSET 21
#define SDT_XN_OFFSET 5

#define SDT_IMM12_OFFSET 10

#define SDT_IMM9_OFFSET 12
#define SDT_I_OFFSET 11
#define SDT_BIT_OFFSET 10

#define SDT_XM_OFFSET 16
#define SDT_ROFF1_OFFSET 13
#define SDT_ROFF2_OFFSET 10

#define SDT_SIMM19_OFFSET 5

#define SDT_MODE_LEN 1
#define SDT_SF_LEN 1
#define SDT_MODE_LEN2 1
#define SDT_RT_LEN 5

#define SDT_U_LEN 1
#define SDT_L_LEN 1
#define SDT_OFFMODE_LEN 1
#define SDT_XN_LEN 5

#define SDT_IMM12_LEN 12

#define SDT_IMM9_LEN 9
#define SDT_I_LEN 1
#define SDT_BIT_LEN 1

#define SDT_XM_LEN 5
#define SDT_ROFF1_LEN 2
#define SDT_ROFF2_LEN 2

#define SDT_SIMM19_LEN 19


#define B_TYPE_OFFSET 30

#define B_SIMM26_OFFSET 0

#define B_SIMM19_OFFSET 5
#define B_TAG_OFFSET 1
#define B_NEG_OFFSET 0

#define B_BIT_OFFSET 25
#define B_REG_OFFSET 16
#define B_XN_OFFSET 5

#define B_TYPE_LEN 2

#define B_SIMM26_LEN 26

#define B_SIMM19_LEN 19
#define B_TAG_LEN 3
#define B_NEG_LEN 1

#define B_BIT_LEN 1
#define B_REG_LEN 5
#define B_XN_LEN 5

#endif