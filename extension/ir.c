#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "ast.h"
#include "ir.h"
#include "../src/io.h"

IRProgram* create_ir_program(void)
{
    IRProgram *program = malloc(sizeof(IRProgram));
    assert(program != NULL);
    program->head = NULL;
    program->tail = NULL;
    return program;
}

void free_ir_program(IRProgram *program)
{
    if (program) {
        free_ir_instruction(program->head);
        free(program);
    }
}

Token *create_token(uint8_t value)
{
    Token *token = malloc(sizeof(Token));
    assert(token != NULL);
    token->value = value;
    return token;
}

IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line)
{
    IRInstruction* instruction = malloc(sizeof(IRInstruction));
    assert(instruction != NULL);
    instruction->type = type;
    instruction->dest = (dest != NOT_USED) ? create_token(dest) : NULL;
    instruction->src1 = (src1 != NOT_USED) ? create_token(src1) : NULL;
    instruction->src2 = (src2 != NOT_USED) ? create_token(src2) : NULL;
    instruction->src3 = (src3 != NOT_USED) ? create_token(src3) : NULL;
    instruction->line = (*line)++;
    instruction->count = 0;
    instruction->next = NULL;
    return instruction;
}

void free_ir_instruction(IRInstruction *instruction)
{
    while (instruction) {
        IRInstruction *next = instruction->next;
        free(instruction->dest);
        free(instruction->src1);
        free(instruction->src2);
        free(instruction->src3);
        free(instruction);
        instruction = next;
    }
}

State *create_state(void)
{
    State *state = malloc(sizeof(State));
    assert(state != NULL);

    state->map_size = 0;
    state->funcs_size = 0;
    state->symbol_table_size = 0;
    state->directives_size = 0;
    state->stack_size = 0;
    
    srand(time(NULL));
    state->stack_pointer = (rand() % (MEMORY_CAPACITY - STACK_OFFSET - MAX_STACK_SIZE)) / 64 * 64 + STACK_OFFSET + MAX_STACK_SIZE;

    state->map = malloc(MAX_VARS * sizeof(Entry *));
    assert(state->map != NULL);
    for (int i = 0; i < MAX_VARS; i++) {
        state->map[i] = malloc(sizeof(Entry));
        assert(state->map[i] != NULL);
    }

    state->funcs = malloc(MAX_FUNCS * sizeof(Func *));
    assert(state->funcs != NULL);
    for (int i = 0; i < MAX_FUNCS; i++) {
        state->funcs[i] = malloc(sizeof(Func));
        state->funcs[i]->state = NULL;
        assert(state->funcs[i] != NULL);
    }

    state->symbol_table = malloc(MAX_LABELS * sizeof(Label *));
    assert(state->symbol_table != NULL);
    for (int i = 0; i < MAX_LABELS; i++) {
        state->symbol_table[i] = malloc(sizeof(Label));
        assert(state->symbol_table[i] != NULL);
    }

    state->directives = malloc(MAX_DIRECTIVES * sizeof(Directive *));
    assert(state->directives != NULL);
    for (int i = 0; i < MAX_DIRECTIVES; i++) {
        state->directives[i] = malloc(sizeof(Directive));
        assert(state->directives[i] != NULL);
    }

    state->stack = malloc(MAX_STACK_SIZE * sizeof(int64_t));
    assert(state->stack != NULL);

    // state->library = create_ir_program();

    return state;
}

void free_state(State *state) {
    if (state == NULL) {
        return;
    }
    free(state->stack);
    for (int i = 0; i < MAX_DIRECTIVES; i++) {
        free(state->directives[i]);
    }
    for (int i = 0; i < MAX_LABELS; i++) {
        free(state->symbol_table[i]);
    }
    for (int i = 0; i < MAX_FUNCS; i++) {
        free(state->funcs[i]->state);
        free(state->funcs[i]);
    }
    for (int i = 0; i < MAX_VARS; i++) {
        free(state->map[i]);
    }
    free(state->directives);
    free(state->symbol_table);
    free(state->funcs);
    free(state->map);
    free(state);
}


