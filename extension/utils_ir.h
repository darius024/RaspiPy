#ifndef UTILS_IR_H
#define UTILS_IR_H

#include <stdbool.h>
#include <stdint.h>
#include "ast.h"
#include "ir.h"

#define MAX_ARGS 8

State *create_state();

BranchConditional get_comparison(BinaryOp *binary_op);
BranchConditional get_negated_comparison(BinaryOp *binary_op);

uint8_t get_register(State *state, char *name);
uint8_t get_free_register(void);
uint8_t get_free_intermediary_register(State *state);
void free_register(State *state, uint8_t i);
void free_intermediary_register(State *state, uint8_t reg);
void insert_instruction(IRProgram *program, IRInstruction *instruction, int count_update);
void insert_program(IRProgram *program, IRProgram *block);
int64_t get_var_value(State *state, char *name);
bool used_register(State *state, uint8_t reg);

TokenType get_register_type(uint8_t reg);
TokenType get_operation_type(uint8_t *reg1, uint8_t *reg2, uint8_t *reg3);

void push_to_stack(IRProgram *program, State *state, uint8_t reg, int *line);
void pop_from_stack(IRProgram *program, State *state, uint8_t reg, int *line);
void save_register(IRProgram *program, State *state, uint8_t reg, int *line);
void restore_register(IRProgram *program, State *state, uint8_t reg, int *line);

void update_state(State *state, uint8_t reg, int64_t value);
void add_name(State *state, uint8_t reg, char *name);

void push_directive(State *state, int64_t value, const char *name);
int get_directive_address(State *state);

void add_label(State *state, int64_t address, char *name);
void update_state_fun(State *state, char *name, State *callee_state);
int get_label_address(State *state);

void add_fun(State *state, int64_t address, char *name);
int get_fun_address(State *state, char *name);

void append_state(State *state, State *callee_state);

#endif 
