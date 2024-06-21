#ifndef IR_H
#define IR_H

#include <stdint.h>

#define NUM_REGISTERS 31
#define RP 30 // return pointer
#define ZR 31
#define SP 32 // stack pointer
#define X0 0
#define MAX_NAMES 16
#define MAX_VARS 64
#define MAX_FUNCS 16
#define NOT_USED -1
#define MAX_MOVE_VALUE (1 << 16)
#define MEMORY_SIZE_2 (1024 * 1024 * 2)
#define STACK_OFFSET 2048
#define MAX_STACK 1024
#define MAX_HOTSPOTS 128
#define MAX_ASSEMBLY_LINE 128
#define THRESHOLD 16


typedef struct HotMap {
    int line;
    uint32_t instruction;
} HotMap;

typedef struct {
    char name[MAX_NAMES];
    int64_t value;
    uint8_t reg;
} Entry;

typedef struct {
    char name[MAX_NAMES];
    int line;
} Func;

typedef struct {
    int map_size;
    Entry map[MAX_VARS];
    int funcs_size;
    Func funcs[MAX_FUNCS];
    int stack_size;
    int64_t stack[MAX_STACK];
    int hotspots_size;
    HotMap hotspots[MAX_HOTSPOTS];
} State;


/*
 * Register state - shared by the whole program
*/
extern int registers[NUM_REGISTERS];

/*
 * Program Intermediate Representation
 * - represented as a linked list of instructions
 * - each instruction has a type and fields
*/
typedef enum {
    IR_ADD, IR_SUB,
    // adds/subs can be derived
    IR_CMP,
    // cmn can be derived
    IR_NEG,
    // negs can be derived
    IR_AND, IR_EOR, IR_ORR,
    // ands, bic(s), eon, orn can be derived
    IR_TST,
    IR_MOV,
    IR_MOVZ,
    IR_MVN,
    // movn, movk can be derived
    IR_MADD,
    // msub, mneg can be derived
    IR_MUL,

    IR_B,
    IR_BR,
    IR_BCOND,

    IR_LDR,
    IR_STR,

    IR_DIR
} IRType;

typedef enum {
    B_EQ, B_NE, B_GE, B_GT, B_LE, B_LT
} BranchConditional;

typedef enum {
    IMM, REG, LABEL
} TokenType;

typedef struct {
    uint8_t reg; // register
    TokenType type;
} Token;

typedef struct IRInstruction {
    IRType type; // mnemonic
    Token *dest; // 1st token
    Token *src1; // 2nd token
    Token *src2; // 3rd token
    Token *src3; // 4th token
    int line;    // instruction position
    int count;   // number of types executed
    struct IRInstruction *next;
} IRInstruction;

typedef struct {
    IRInstruction *head;
    IRInstruction *tail;
} IRProgram;

/*
 * Program State
 * - includes:
 *      : map of all variables with their associate value and register
 *      : map of all function definitions and their address
 *      : map of all directives that should be stored in binary under program
 *      : map of all hotspots
 * - each instruction has a type and fields
*/

IRProgram* create_ir_program(void);
void free_ir_program(IRProgram *program);

Token *create_token(uint8_t value);
IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line);
void free_ir_instruction(struct IRInstruction *instruction);


// Prototypes
uint32_t getHotSpot(IRInstruction *ir_instr);
uint32_t returnHotSpot(State *state, int line);
void addHotSpot(State *state, uint32_t instrBin, int line);

extern State *create_state(void);
extern void free_state(State *state);

void putMnemonic(char *assembly_line, IRType type);
void putRegOrImm(char *assembly_line, Token *token);

#endif
