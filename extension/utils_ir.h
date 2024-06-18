#ifndef UTILS_IR_H
#define UTILS_IR_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"

#define MAX_ARGS 8
#define NOT_USED -1

extern int registers[31];

typedef struct {
    Entry *map[MAX_VAR];
    int map_size;
} State;


BranchConditional getComparison(BinaryOp *binary_op);

BranchConditional getNegatedComparison(BinaryOp *binary_op);

int getRegister(Name *name, State *state);

IRProgram* create_ir_program();


void free_ir_program(IRProgram *program);

IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line);


void free_ir_instruction(IRInstruction *instruction);


int getNextFreeRegister();

int freeRegister(int i);

Expression *getArguments(FunctionCall *function_call); 

void insertInstruction(IRProgram *program, IRInstruction *instruction);

void insertProgram(IRProgram *program, IRProgram *block);

int64_t *search_vars(char *name, state *State);


#endif 