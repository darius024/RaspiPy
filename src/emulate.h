#define OP0_IS_DPI(op) (op / 2 == 4) // 100x
#define OP0_IS_DPR(op) (op % 8 == 5) // x101
#define OP0_IS_SDT(op) (op % 2 == 0 && op / 4 % 2 == 1) // x1x0
#define OP0_IS_BRANCH(op) (op / 2 == 5) // 101x

#define ARITHMETIC 2 // 010
#define WIDEMOVE 5 // 101

#define BITWISE_AND 0 // 00
#define BITWISE_OR 1 // 01
#define BITWISE_XOR 2 // 10
#define BITWISE_AND_FLAGS 3 // 11

#define BRANCH_UNCONDITIONAL 0 // 00
#define BRANCH_CONDITIONAL 1 // 01
#define BRANCH_REGISTER 3 // 11

#define BRANCH_EQ_NE 0
#define BRANCH_GE_LT 1
#define BRANCH_GT_LE 2
#define BRANCH_ALWAYS 3

#define LOGICAL_SHIFT_LEFT 0
#define LOGICAL_SHIFT_RIGHT 1
#define ARITHMETIC_SHIFT_RIGHT 2
#define ROTATE_RIGHT 3
