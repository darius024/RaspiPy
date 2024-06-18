#ifndef UTILS_IR_H
#define UTILS_IR_H

#include <stdint.h>
#include "ast.h"
#include "ir.h"

#define MAX_ARGS 8

State *create_state();

BranchConditional getComparison(BinaryOp *binary_op);
BranchConditional getNegatedComparison(BinaryOp *binary_op);
uint8_t getRegister(Name *name, State *state);
uint8_t getNextFreeRegister();
void freeRegister(State *state, uint8_t i);
void insertInstruction(IRProgram *program, IRInstruction *instruction);
void insertProgram(IRProgram *program, IRProgram *block);
int64_t *search_vars(char *name, State *state);
void updateState(State *state, uint8_t reg, int64_t value);
IRProgram *push_to_stack(IRProgram *program, State *state, uint8_t reg, int *line);
IRProgram *pop_from_stack(IRProgram *program, State *state, uint8_t reg, int *line);

#endif 
