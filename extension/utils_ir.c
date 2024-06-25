#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ir.h"
#include "utils_ir.h"

extern int64_t registers[NUM_REGISTERS];

BranchConditional get_comparison(BinaryOp *binary_op)
{
    if (!strcmp(binary_op->op, "==")) {
        return B_EQ;
    }
    if (!strcmp(binary_op->op, "!=")) {
        return B_NE;
    }
    if (!strcmp(binary_op->op, ">=")) {
        return B_GE;
    }
    if (!strcmp(binary_op->op, ">")) {
        return B_GT;
    }
    if (!strcmp(binary_op->op, "<=")) {
        return B_LE;
    }
    if (!strcmp(binary_op->op, "<")) {
        return B_LT;
    }
    exit(EXIT_FAILURE);
}

BranchConditional get_negated_comparison(BinaryOp *binary_op)
{
    if (!strcmp(binary_op->op, "==")) {
        return B_NE;
    }
    if (!strcmp(binary_op->op, "!=")) {
        return B_EQ;
    }
    if (!strcmp(binary_op->op, ">=")) {
        return B_LT;
    }
    if (!strcmp(binary_op->op, ">")) {
        return B_LE;
    }
    if (!strcmp(binary_op->op, "<=")) {
        return B_GT;
    }
    if (!strcmp(binary_op->op, "<")) {
        return B_GE;
    }
    exit(EXIT_FAILURE);
}

uint8_t get_register(State *state, char *name)
{
    for (int i = 0; i < state->map_size; i++) {
        if (!strcmp(state->map[i]->name, name)) {
            return state->map[i]->reg;
        }
    }
    // Not found
    // Create new register for that variable
    strcpy(state->map[state->map_size]->name, name);
    state->map[state->map_size]->value = 0;
    state->map[state->map_size]->reg = get_free_intermediary_register(state);
    return state->map[state->map_size++]->reg;
}

uint8_t get_free_register(void) {
    for (uint8_t i = 0; i < NUM_REGISTERS; i++) {
        if (registers[i] == NOT_USED) {
            registers[i] = 0;
            return i;
        }
    }
    exit(EXIT_FAILURE);
}

uint8_t get_free_intermediary_register(State *state)
{
    for(uint8_t i = 0; i < NUM_REGISTERS; i++) {
        bool found = false;
        for (int j = 0; j < state->map_size && !found; j++) {
            if (state->map[j]->reg == i) {
                found = true;
            }
        }
        if (!found) {
            registers[i] = 0;
            return i; 
        }
    }
    exit(EXIT_FAILURE);
}

void free_register(State *state, uint8_t reg) {
    for (uint8_t i = 0; i <= NUM_REGISTERS; i++) {
        int found = 0;
        for (int j = 0; j < state->map_size && !found; j++) {
            if (state->map[j]->reg == reg) {
                state->map_size--;
                for (int j = i; j < state->map_size; j++) {
                    strcpy(state->map[j]->name, state->map[j + 1]->name);
                    state->map[j]->reg = state->map[j + 1]->reg;
                    state->map[j]->value = state->map[j + 1]->value;
                }
                registers[reg] = NOT_USED;
                return;
            }
        }
    }
    registers[reg] = NOT_USED;
}

void free_intermediary_register(State *state, uint8_t reg)
{
    bool found = false;
    for (int i = 0; i < state->map_size && !found; i++) {
        if (state->map[i]->reg == reg) {
            found = true;
        }
    }
    if (!found) {
        registers[reg] = NOT_USED;
    }
}

bool used_register(State *state, uint8_t reg)
{
    for (int i = 0; i < state->map_size; i++) {
        if (state->map[i]->reg == reg) {
            return true;
        }
    }
    // Check for SP
    return (state->stack_size != 0);
}

TokenType get_register_type(uint8_t reg)
{
    return (registers[reg] < (int64_t) INT32_MAX) ? REGW : REGX;
}

TokenType get_operation_type(uint8_t *reg1, uint8_t *reg2, uint8_t *reg3)
{
    TokenType type = REGW;
    if(reg1 != NULL && get_register_type(*reg1) == REGX) type = REGX;
    if(reg2 != NULL && get_register_type(*reg2) == REGX) type = REGX;
    if(reg3 != NULL && get_register_type(*reg3) == REGX) type = REGX;
    return type;
}

void push_to_stack(IRProgram *program, State *state, uint8_t reg, int *line)
{
    IRInstruction *stack = create_ir_instruction(IR_SUB, SP, SP, 64, NOT_USED, line);
    stack->dest->type = REGX;
    stack->src1->type = REGX;
    stack->src2->type = IMM;
    insert_instruction(program, stack, 1);
    IRInstruction *push = create_ir_instruction(IR_STR, reg, SP, NOT_USED, NOT_USED, line);
    push->dest->type = REGX;
    push->src1->type = REGX;
    insert_instruction(program, push, 1);
    state->stack[state->stack_size] = 0;
    state->stack_size++;
    state->stack_pointer -= 64;
}

void pop_from_stack(IRProgram *program, State *state, uint8_t reg, int *line)
{
    IRInstruction *pop = create_ir_instruction(IR_LDR, reg, SP, NOT_USED, NOT_USED, line);
    pop->dest->type = REGX;
    pop->src1->type = REGX;
    insert_instruction(program, pop, 1);
    state->stack[state->stack_size] = registers[reg];
    state->stack_size--;
    state->stack_pointer += 64;
    IRInstruction *stack = create_ir_instruction(IR_ADD, SP, SP, 64, NOT_USED, line);
    stack->dest->type = REGX;
    stack->src1->type = REGX;
    stack->src2->type = IMM;
    insert_instruction(program, stack, 1);
}

void save_register(IRProgram *program, State *state, uint8_t reg, int *line)
{
    // Save in caller/calle saved
    if (used_register(state, reg)) {
        push_to_stack(program, state, reg + 8, line);
        IRInstruction *call_saved = create_ir_instruction(IR_MOV, reg + 8, reg, NOT_USED, NOT_USED, line);
        call_saved->dest->type = REGX;
        call_saved->src1->type = REGX;
        insert_instruction(program, call_saved, 0);
    }
}

void restore_register(IRProgram *program, State *state, uint8_t reg, int *line)
{
    // As X0 stores the address, restore X0 in another free register and link it with the variable
    if (reg == X0) {
        reg = get_free_intermediary_register(state);
        for (int i = 0; i < state->map_size; i++) {
            if (state->map[i]->reg == X0) {
                state->map[i]->reg = reg;
            }
        }
        IRInstruction *call_saved = create_ir_instruction(IR_MOV, reg, 8, NOT_USED, NOT_USED, line);
        call_saved->dest->type = REGX;
        call_saved->src1->type = REGX;
        insert_instruction(program, call_saved, 0);
        pop_from_stack(program, state, 8, line);
        return;
    }
    // Restore from caller/calle saved
    if (used_register(state, reg)) {
        IRInstruction *call_saved = create_ir_instruction(IR_MOV, reg, reg + 8, NOT_USED, NOT_USED, line);
        call_saved->dest->type = REGX;
        call_saved->src1->type = REGX;
        insert_instruction(program, call_saved, 0);
        pop_from_stack(program, state, reg + 8, line);
    }
}

void insert_instruction(IRProgram *program, IRInstruction *instruction, int count_update)
{
    if (program->head == NULL) {
        program->head = instruction;
        program->tail = instruction;
    } else {
        program->tail->next = instruction;
        program->tail = instruction;
    }
    instruction->count += count_update;
}

void insert_program(IRProgram *program, IRProgram *block)
{
    if (block == NULL) {
        return;
    }
    if (program->head == NULL) {
        program->head = block->head;
        program->tail = block->tail;
    } else {
        program->tail->next = block->head;
        program->tail = block->tail;
    }
}

int64_t get_var_value(State *state, char *name)
{
    for (int i = 0; i < state->map_size; i++) {
        if (!strcmp(state->map[i]->name, name)) {
            return state->map[i]->value;
        }    
    }
    exit(EXIT_FAILURE);
}

void update_state(State *state, uint8_t reg, int64_t value)
{
    for (int i = 0; i < state->map_size; i++) {
        if (state->map[i]->reg == reg) {
            state->map[i]->value = value;
        }
    }
    registers[reg] = value;
}

void add_name(State *state, uint8_t reg, char *name)
{
    registers[reg] = 0;
    strcpy(state->map[state->map_size]->name, name);
    state->map[state->map_size]->value = registers[reg];
    state->map[state->map_size++]->reg = reg;
}


void push_directive(State *state, int64_t value, const char *name)
{
    if (name == NULL) {
        char str[MAX_NAMES];
        sprintf(str, "dir_%d", state->directives_size);
        strcpy(state->directives[state->directives_size]->name, str);
        state->directives[state->directives_size++]->value = value;
    } else {
        bool found = false;
        for (int i = 0; i < state->directives_size && !found; i++) {
            if (!strcmp(state->directives[i]->name, name)) {
                found = true;
            }
        }
        if (!found) {
            strcpy(state->directives[state->directives_size]->name, name);
            state->directives[state->directives_size++]->value = value;
        }
    }
}

int get_directive_address(State *state)
{
    return state->directives_size - 1;
}

void add_label(State *state, int64_t address, char *name)
{
    if (name == NULL) {
        char str[MAX_NAMES];
        sprintf(str, "label_%d", state->symbol_table_size);
        strcpy(state->symbol_table[state->symbol_table_size]->name, str);
    } else {
        strcpy(state->symbol_table[state->symbol_table_size]->name, name);
    }
    state->symbol_table[state->symbol_table_size++]->address = address;
}

int get_label_address(State *state)
{
    return state->symbol_table_size - 1;
}

void add_fun(State *state, int64_t address, char *name)
{
    add_label(state, address, name);
    strcpy(state->funcs[state->funcs_size]->name, name);
    state->funcs[state->funcs_size++]->address = address;
}

void update_state_fun(State *state, char *name, State *callee_state)
{
    for (int i = 0; i < state->funcs_size; i++) {
        if (!strcmp(state->funcs[i]->name, name)) {
            state->funcs[i]->state = callee_state;
        }
    }
}

int get_fun_address(State *state, char *name)
{
    for (int i = 0; i < state->symbol_table_size; i++) {
        if (!strcmp(state->symbol_table[i]->name, name)) {            
            return i;
        }
    }
    exit(EXIT_FAILURE);
}

void append_state(State *state, State *callee_state)
{
    // Append functions and labels to callee state, so it can access them
    for (int i = 0; i < state->funcs_size; i++) {
        add_fun(callee_state, state->funcs[i]->address, state->funcs[i]->name);
        update_state_fun(callee_state, state->funcs[i]->name, state->funcs[i]->state);
    }
    for (int i = 0; i < state->symbol_table_size; i++) {
        bool found = false;
        for (int j = 0; j < state->funcs_size && !found; j++) {
            if (!strcmp(state->funcs[i]->name, state->symbol_table[i]->name)) {
                found = true;
            }
        }
        if (!found) {
            add_label(callee_state, state->symbol_table[i]->address, state->symbol_table[i]->name);
        }
    }
}
