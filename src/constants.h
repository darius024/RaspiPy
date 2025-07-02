// Constants for the entire program

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>

#define EXIT_PROGRAM(msg) do {perror(msg "\n"); exit(EXIT_FAILURE);} while(0)
#define NOT_FOUND -1

#define INSTR_BYTES 4
#define ZR_SP 31
#define NUM_OF_REGISTERS 31
#define MEMORY_SIZE (2 * 1024 * 1024) // 2MB

#define XZR "xzr"
#define WZR "wzr"
#define RZR "zr"
#define RSP "sp"

#define OP0_IS_DPI(op) (op / 2 == 4) // 100x
#define OP0_IS_DPR(op) (op % 8 == 5) // x101
#define OP0_IS_SDT(op) (op % 2 == 0 && op / 4 % 2 == 1) // x1x0
#define OP0_IS_B(op) (op / 2 == 5) // 101x

#define OP0_DPI 8 // 1000
#define OP0_DPR 5 // 0101
#define OP0_SDT 12 // 1100
#define OP0_B 10 // 1010

#define ARITHMETIC 2 // 010
#define ARITHMETIC_SHIFT 12
#define ADD 0 // 00
#define ADD_SETFLAGS 1 // 01
#define SUB 2 // 10
#define SUB_SETFLAGS 3 // 11

#define WIDEMOVE 5 // 101
#define WIDEMOVE_SHIFT 16
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

#define DPR_OPC 0 // 00
#define DPR_MUL 8 // 1000
#define SDT_IMM12_32 4
#define SDT_IMM12_64 8
#define SDT_ROFF1 3 // 11
#define SDT_ROFF2 2 // 10
#define SDT_BIT 1 // 1
#define B_BIT 1 // 1
#define B_REG 31 // 11111


#define NUM_EXISTS_SH 3
#define NUM_EXISTS_HW 2

#define OPC_DIV 2
#define N_MOD 2

#define EQ_NE_TAG 0
#define EQ_NEG 0
#define NE_NEG 1

#define GE_LT_TAG 5
#define GE_NEG 0
#define LT_NEG 1

#define GT_LE_TAG 6
#define GT_NEG 0
#define LE_NEG 1

#define ALWAYS_TAG 7
#define ALWAYS_NEG 0

#define CMP 0
#define CMN 1
#define NEG 2
#define NEGS 3
#define TST 4
#define MVN 5
#define MOV 6
#define MUL 7
#define MNEG 8

#endif
