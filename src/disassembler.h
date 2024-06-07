#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdio.h>
#include <stdbool.h>

#define SIZE_ALIASES (sizeof(aliases) / sizeof(char *))
#define SIZE_WIDEMOVES (sizeof(wideMoves) / sizeof(char *))
#define SIZE_ARITHMETICS (sizeof(arithmetics) / sizeof(char *))
#define SIZE_LOGICAL (sizeof(logical) / sizeof(char *))
#define SIZE_MULTIPLY (sizeof(multiply) / sizeof(char *))

#define SIZE_DPI (sizeof(dpiOnes) / sizeof(int))
#define SIZE_DPR (sizeof(dprOnes) / sizeof(int))
#define SIZE_LS (sizeof(lsOnes) / sizeof(int))
#define SIZE_SDT (sizeof(sdtOnes) / sizeof(int))
#define SIZE_SDTREG (sizeof(sdtRegOnes) / sizeof(int))
#define SIZE_B (sizeof(bOnes) / sizeof(int))



// Categories of instructions to support
typedef enum
{
    isDPI, // Data processing immediate
    isDPR, // Data processing register
    isSDT, // Single data transfer
    isB,   // Branch
} InstructionType;

// Data Processing Immediate
typedef struct
{
    bool sf;     // bit-width: 0 for 32-bit, 1 for 64-bit
    uint8_t opc; // opcode
    uint8_t opi; // arithmetic or wide move
    union
    {
        struct // arithmetic
        {
            bool sh;        // 12-bit left shift of imm12
            uint16_t imm12; // 1st operand: immediate
            uint8_t rn;     // 2nd operand: register
        };
        struct // widemove
        {
            uint8_t hw;     // (hw * 4) left shift of imm16
            uint16_t imm16; // operand
        };
    };
    uint8_t rd; // destination register
} DPI;

// Data Processing Register
typedef struct
{
    bool sf; // bit-width: 0 for 32-bit, 1 for 64-bit
    union    // opc
    {
        uint8_t opc;
    };
    bool m; // multiply flag
    union   // opr
    {
        struct // arithmetic / bit-logic
        {
            bool armOrLog; // 1 for arithmetic, 0 for logic
            uint8_t shift;
            bool n;
        };
        uint8_t opr;
    };
    uint8_t rm;
    union
    {
        uint8_t operand; // arithmetic / bit-logic
        struct           // multiply
        {
            bool x;
            uint8_t ra;
        };
    };
    uint8_t rn;
    uint8_t rd; // destination register
} DPR;

// Single Data Transfer
typedef struct
{
    bool mode; // 1 for single data transfer, 0 for load literal
    bool sf;   // load size: 0 for 32-bit, 1 for 64-bit
    union
    {
        struct // single data transfer
        {
            bool u;       // unsigned offset flag
            bool l;       // type of data transfer
            bool offmode; // 1 for register offset, 0 for pre/post-index
            union         // addressing mode
            {
                uint8_t xm;
                struct
                {
                    int16_t simm9;
                    bool i; // 1 for pre-, 0 for post-
                };
                uint16_t imm12;
            };
            uint8_t xn;
        };
        int32_t simm19; // load literal
    };
    uint8_t rt;
} SDT;

// Branch
typedef struct
{
    uint8_t type; // 0 - unconditional, 1 - conditional, 3 - register
    bool nop;     // 0 - nop, 1 - reg
    union
    {
        int32_t simm26;
        uint8_t xn;
        struct
        {
            int32_t simm19;
            struct {
                uint8_t tag;  // encoding of mnemonic //maybe split more like in emulate
                bool neg;     // 4th bit of mnemonic
            } cond;
        };
    };
} B;

typedef struct
{
    InstructionType instructionType;
    union
    {
        DPI dpi;
        DPR dpr;
        SDT sdt;
        B b;
    };
} Instruction;


// Function Prototypes
extern void disassembleDPI(char *instrname, char **tokens, int numTokens, Instruction *instr);

extern void disassembleDPR(char *instrname, char **tokens, int numTokens, Instruction *instr);

extern void disassembleSDT(char *instrname, char **tokens, int numTokens, Instruction *instr, FILE *outputFile, int PC);

extern void disassembleB(char *instrname, char *token, Instruction *instr, FILE *outputFile, int PC);

extern void disassembleDir(char *dir, FILE *outputFile);

extern void handleUndefTable(FILE *outputFile);

extern int getPositionInArray(char *word,const char **words, int num);

extern bool checkWordInArray(char *word,const char **words, int num);

extern bool checkForAliases(char *instrname);

extern void setOnes(uint32_t *instruction, const int *bits, int num);

extern void putBits(uint32_t *instr, void *value, int start, int nbits);

#endif