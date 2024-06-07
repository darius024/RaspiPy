// Header file with constants for emulate

#define OP0_IS_DPI(op) (op / 2 == 4)                    // 100x
#define OP0_IS_DPR(op) (op % 8 == 5)                    // x101
#define OP0_IS_SDT(op) (op % 2 == 0 && op / 4 % 2 == 1) // x1x0
#define OP0_IS_B(op) (op / 2 == 5)                      // 101x

#define ARITHMETIC 2 // 010
#define ADD 0 // 00
#define ADD_SETFLAGS 1 // 01
#define SUB 2 // 10
#define SUB_SETFLAGS 3 // 11

#define WIDEMOVE 5 // 101
#define MOVE_WITH_NOT 0 // 00
#define MOVE_WITH_ZERO 2 // 10
#define MOVE_WITH_KEEP 3 // 11

#define BITWISE_AND 0 // 00
#define BITWISE_OR 1 // 01
#define BITWISE_XOR 2 // 10
#define BITWISE_AND_SETFLAGS 3 // 11

#define LOGICAL_SHIFT_LEFT 0 // lsl
#define LOGICAL_SHIFT_RIGHT 1 // lsr
#define ARITHMETIC_SHIFT_RIGHT 2 // asr
#define ROTATE_RIGHT 3 // ror

#define BRANCH_UNCONDITIONAL 0 // 00
#define BRANCH_CONDITIONAL 1 // 01
#define BRANCH_REGISTER 3 // 11

#define EQ_NE 0
#define GE_LT 1
#define GT_LE 2
#define ALWAYS 3
