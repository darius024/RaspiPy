#include <stdint.h>
#include <stdbool.h>

// Specific ADTs

// Data Processing Immediate
struct DPI {
    bool sf;     // bit-width: 0 - 32-bit, 1 - 64-bit
    uint8_t opc; // opcode
    uint8_t opi; // arithmetic or wide move
    union {
        struct { // arithmetic
            bool sh;        // 12-bit left shift of imm12
            uint16_t imm12; // 1st operand: immediate
            uint8_t rn;     // 2nd operand: register
        };
        struct { // widemove
            uint8_t hw;     // (hw * 4) left shift of imm16
            uint16_t imm16; // operand
        };
    };
    uint8_t rd; // destination register
};

// Data Processing Register
struct DPR {
    bool sf;     // bit-width: 0 - 32-bit, 1 - 64-bit
    uint8_t opc; // opcode
    bool m;      // multiply flag
    union { // opr
        struct { // arithmetic, bit-logic
            bool armOrLog; // 0 - logical, 1 - arithmetic
            uint8_t shift;
            bool n;
        };
        uint8_t opr; // multiply
    };
    uint8_t rm;
    union {
        uint8_t operand; // arithmetic, bit-logic
        struct { // multiply
            bool x;
            uint8_t ra;
        };
    };
    uint8_t rn;
    uint8_t rd; // destination register
};

// Single Data Transfer
struct SDT {
    bool mode; // 1 - single data transfer, 0 - load literal
    bool sf;   // load size: 0 - 32-bit, 1 - 64-bit
    union {
        struct { // single data transfer
            bool u;       // unsigned offset flag
            bool l;       // type of data transfer
            bool offmode; // 1 - register offset, 0 - pre/post-index
            union {
                uint8_t xm;
                struct {
                    int16_t simm9;
                    bool i;
                };
                uint16_t imm12;
            };
            uint8_t xn;
        };
        int32_t simm19; // load literal
    };
    uint8_t rt; // target register
};

// branch
struct B {
    uint8_t type; // 0 - unconditional, 1 - conditional, 3 - register
    union {
        int32_t simm26;
        uint8_t xn;
        struct {
            int32_t simm19;
            struct {
                uint8_t tag; // encoding of mnemonic
                bool neg;    // negate condition
            } cond;
        };
    };
};

// Generic ADTs

typedef enum {
    isDPI, // Data processing immediate
    isDPR, // Data processing register
    isSDT, // Single data transfer
    isB,   // Branch
} InstructionType;

typedef struct {
    InstructionType instructionType;
    union {
        struct DPI dpi;
        struct DPR dpr;
        struct SDT sdt;
        struct B b;
    };
} Instruction;