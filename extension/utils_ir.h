#ifndef UTILS_IR_H
#define UTILS_IR_H

#include <stdint.h>
#include "ast.h"
#include "ir.h"
#include "state.h"

#define MAX_ARGS 8

State *create_state();

BranchConditional getComparison(BinaryOp *binary_op);
BranchConditional getNegatedComparison(BinaryOp *binary_op);
uint8_t getRegister(Name *name, State *state);
uint8_t getNextFreeRegister();
void freeRegister(State *state, uint8_t i);
void freeNonVarRegister(State *state, uint8_t reg);
void insertInstruction(IRProgram *program, IRInstruction *instruction, int count_update);
void insertProgram(IRProgram *program, IRProgram *block);
int64_t searchName(char *name, State *state);
void updateState(State *state, uint8_t reg, int64_t value);
void addNameState(State *state, uint8_t reg, char *name);
void push_to_stack(IRProgram *program, State *state, uint8_t reg, int *line);
void pop_from_stack(IRProgram *program, State *state, uint8_t reg, int *line);
void saveRegister(IRProgram *program, State *state, uint8_t reg, int *line);
void restoreRegister(IRProgram *program, State *state, uint8_t reg, int *line);


#endif 
