#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ir.h"
#include "utils_ir.h"

int registers[NUM_REGISTERS] = {NOT_USED};

BranchConditional getComparison(BinaryOp *binary_op)
{
    if (strcmp(binary_op->op, "==")) {
        return B_EQ;
    }
    if (strcmp(binary_op->op, "!=")) {
        return B_NE;
    }
    if (strcmp(binary_op->op, ">=")) {
        return B_GE;
    }
    if (strcmp(binary_op->op, ">")) {
        return B_GT;
    }
    if (strcmp(binary_op->op, "<=")) {
        return B_LE;
    }
    if (strcmp(binary_op->op, "<")) {
        return B_LT;
    }
    exit(EXIT_FAILURE);
}

BranchConditional getNegatedComparison(BinaryOp *binary_op)
{
    if (strcmp(binary_op->op, "==")) {
        return B_NE;
    }
    if (strcmp(binary_op->op, "!=")) {
        return B_EQ;
    }
    if (strcmp(binary_op->op, ">=")) {
        return B_LT;
    }
    if (strcmp(binary_op->op, ">")) {
        return B_LE;
    }
    if (strcmp(binary_op->op, "<=")) {
        return B_GT;
    }
    if (strcmp(binary_op->op, "<")) {
        return B_GE;
    }
    exit(EXIT_FAILURE);
}

uint8_t getRegisterState(Name *name, State *state)
{
    for (int i = 0; i < state->map_size; i++) {
        if (strcmp(name->name, state->map[i].name) == 0) {
            return state->map[i].reg;
        }
    }
    // Not found
    // Create new register for that variable
    strcpy(state->map[state->map_size].name, name -> name);
    state->map[state->map_size].value = 0;
    state->map[state->map_size].reg = getNextFreeRegister(state);
    return state->map[state->map_size++].reg;
}

uint8_t getNextFreeRegister() {
    for (uint8_t i = 0; i < 30; i++) {
        if (registers[i] == NOT_USED) {
            registers[i] = 100;
            return i;
        }
    }
    exit(EXIT_FAILURE);
}

void freeRegister(State *state, uint8_t reg) {
    for (uint8_t i = 1; i <= 30; i++) {
        int found = 0;
        int pos = 0;
        for (int j = 0; j < state->map_size && !found; j++) {
            if (state->map[j].reg == reg) {
                found = 1;
                pos = j;
            }
        }
        if (found) { // delete entry in map
            state->map_size--;
            for (int j = pos; j < state->map_size; j++) {
                state->map[j] = state->map[j + 1];
            }
            registers[reg] = NOT_USED;
        }
    }
}

void freeNonVarRegister(State *state, uint8_t reg)
{
    int found = 0;
    for (int i = 0; i < state->map_size && !found; i++) {
        if (state->map[i].reg == reg) {
            found = 1;
        }
    }
    if (!found) {
        registers[reg] = NOT_USED;
    }
}

void push_to_stack(IRProgram *program, State *state, uint8_t reg, int *line)
{
    IRInstruction *push = create_ir_instruction(IR_STR, reg, state->stack_size, NOT_USED, NOT_USED, line);
    push->dest->type = REG;
    push->src1->type = LABEL;
    insertInstruction(program, push, 0);
    state->stack[state->stack_size] = registers[reg];
    state->stack_size++;
    IRInstruction *stack = create_ir_instruction(IR_ADD, SP, SP, 32, NOT_USED, line);
    stack->dest->type = REG;
    stack->src1->type = REG;
    stack->src2->type = IMM;
    insertInstruction(program, stack, 0);
}

void pop_from_stack(IRProgram *program, State *state, uint8_t reg, int *line)
{
    IRInstruction *pop = create_ir_instruction(IR_LDR, reg, state->stack_size, NOT_USED, NOT_USED, line);
    pop->dest->type = REG;
    pop->src1->type = LABEL;
    state->stack[state->stack_size - 2] = 0;
    state->stack_size--;
    IRInstruction *stack = create_ir_instruction(IR_SUB, SP, SP, 32, NOT_USED, line);
    stack->dest->type = REG;
    stack->src1->type = REG;
    stack->src2->type = IMM;
    insertInstruction(program, stack, 0);
}


void saveRegister(IRProgram *program, State *state, uint8_t reg, int *line)
{
    // Save in caller/calle saved
    push_to_stack(program, state, reg + 8, line);
    IRInstruction *call_saved = create_ir_instruction(IR_MOV, reg + 8, reg, NOT_USED, NOT_USED, line);
    insertInstruction(program, call_saved, 0);
}

void restoreRegister(IRProgram *program, State *state, uint8_t reg, int *line)
{
    // Restore from caller/calle saved
    IRInstruction *call_saved = create_ir_instruction(IR_MOV, reg, reg + 8, NOT_USED, NOT_USED, line);
    insertInstruction(program, call_saved, 0);
    pop_from_stack(program, state, reg + 8, line);
}

void insertInstruction(IRProgram *program, IRInstruction *instruction, int count_update)
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

void insertProgram(IRProgram *program, IRProgram *block)
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

int64_t searchName(char *name, State *state)
{
    for (int i = 0; i < state->map_size; i++) {
        if (strcmp(state->map[i].name, name) == 0) {
            return state->map[i].value;
        }    
    }
    exit(EXIT_FAILURE);
}

void updateState(State *state, uint8_t reg, int64_t value)
{
    for (int i = 0; i < state->map_size; i++) {
        if (state->map[i].reg == reg) {
            registers[reg] = value;
            state->map[i].value = value;
        }
    }
}

void addNameState(State *state, uint8_t reg, char *name)
{
    strcpy(state->map[state->map_size].name, name);
    state->map[state->map_size].value = registers[reg];
    state->map[state->map_size].reg = reg;
}
