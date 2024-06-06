#define OP0_START 25

#define OP0_LEN 4



#define DPI_SF_START 31
#define DPI_OPC_START 29
#define DPI_OPI_START 23
#define DPI_RD_START 0

#define DPI_SH_START 22
#define DPI_IMM12_START 10
#define DPI_RN_START 5

#define DPI_HW_START 21
#define DPI_IMM16_START 5

#define DPI_SF_LEN 1
#define DPI_OPC_LEN 2
#define DPI_OPI_LEN 3
#define DPI_RD_LEN 5

#define DPI_SH_LEN 1
#define DPI_IMM12_LEN 12
#define DPI_RN_LEN 5

#define DPI_HW_LEN 2
#define DPI_IMM16_LEN 16



#define DPR_SF_START 31
#define DPR_M_START 28
#define DPR_RM_START 16
#define DPR_OPERAND_START 10
#define DPR_RN_START 5
#define DPR_RD_START 0

#define DPR_OPC_START 29
#define DPR_ARMORLOG_START 24
#define DPR_SHIFT_START 22
#define DPR_N_START 21

#define DPR_OPR_START 31
#define DPR_X_START 15
#define DPR_RA_START 10

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



#define SDT_MODE_START 31
#define SDT_SF_START 30
#define SDT_RT_START 0

#define SDT_U_START 24
#define SDT_L_START 22
#define SDT_OFFMODE_START 21
#define SDT_XN_START 5

#define SDT_IMM12_START 10

#define SDT_IMM9_START 12
#define SDT_I_START 11

#define SDT_XM_START 16

#define SDT_SIMM19_START 5

#define SDT_MODE_LEN 1
#define SDT_SF_LEN 1
#define SDT_RT_LEN 5

#define SDT_U_LEN 1
#define SDT_L_LEN 1
#define SDT_OFFMODE_LEN 1
#define SDT_XN_LEN 5

#define SDT_IMM12_LEN 12

#define SDT_IMM9_LEN 9
#define SDT_I_LEN 1

#define SDT_XM_LEN 5

#define SDT_SIMM19_LEN 19



#define BRANCH_TYPE_START 30

#define BRANCH_SIMM26_START 0

#define BRANCH_SIMM19_START 5
#define BRANCH_COND_START 1
#define BRANCH_NEG_START 0

#define BRANCH_NOP_START 25
#define BRANCH_XN_START 5

#define BRANCH_TYPE_LEN 2

#define BRANCH_SIMM26_LEN 26

#define BRANCH_SIMM19_LEN 19
#define BRANCH_COND_LEN 3
#define BRANCH_NEG_LEN 1

#define BRANCH_NOP_LEN 1
#define BRANCH_XN_LEN 5
