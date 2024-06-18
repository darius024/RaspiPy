#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "utils_ir.h"

int registers[31] = {0};


Entry **create_Map(int size){
    Entry *map = calloc(size, sizeof(Entry));
    return map;
}

HotMap *create_HotMap(){
    Entry *hot_map = calloc(MAX_HOTSPOTS,sizeof(HotMap));
    return hot_map;
}


State *create_state(){
    State* state = malloc(sizeof(State));
    if (state == NULL) {
        perror("couldnt malloc");
        exit(EXIT_FAILURE);
    }
    Entry *map = create_Map(MAX_VAR);
    if(map == NULL) {
        free(state);
        perror("couldnt malloc Entry");
        exit(EXIT_FAILURE);
    }
    Entry *funcs = create_Map(MAX_FUNC);
    if(funcs == NULL) {
        free(state);
        free(map);
        perror("couldnt malloc Entry");
        exit(EXIT_FAILURE);
    }

    HotMap *hotspots = create_HotMap();
    if(hotspots == NULL) {
        free(state);
        free(map);
        free(funcs);
        perror("couldnt malloc hotmap");
        exit(EXIT_FAILURE);
    }

    return state;
}


void free_State(State *state) {
    free(state);
} 



BranchConditional getComparison(BinaryOp *binary_op)
{
    if (strcmp(binary_op->op, "==")) {
        return EQ;
    }
    if (strcmp(binary_op->op, "!=")) {
        return NE;
    }
    if (strcmp(binary_op->op, ">=")) {
        return GE;
    }
    if (strcmp(binary_op->op, ">")) {
        return GT;
    }
    if (strcmp(binary_op->op, "<=")) {
        return LE;
    }
    if (strcmp(binary_op->op, "<")) {
        return LT;
    }
    exit(EXIT_FAILURE);
}

BranchConditional getNegatedComparison(BinaryOp *binary_op)
{
    if (strcmp(binary_op->op, "==")) {
        return NE;
    }
    if (strcmp(binary_op->op, "!=")) {
        return EQ;
    }
    if (strcmp(binary_op->op, ">=")) {
        return LT;
    }
    if (strcmp(binary_op->op, ">")) {
        return LE;
    }
    if (strcmp(binary_op->op, "<=")) {
        return GT;
    }
    if (strcmp(binary_op->op, "<")) {
        return GE;
    }
    exit(EXIT_FAILURE);
}

int getRegister(Name *name, State *state)
{
    for (int i = 0; i < state->map_size; i++) {
        if (strcmp(name->name, state->map[i].name) == 0) {
            return state->map[i].reg;
        }
    }
    // Not found
    // Create new register for that variable
    strcpy(state->map[state->map_size].name, name);
    state->map[state->map_size].value = 0;
    state->map[state->map_size].reg = getNextFreeRegister();
    return state->map[state->map_size++].reg;
}

IRProgram* create_ir_program()
{
    IRProgram *program = (IRProgram *)malloc(sizeof(IRProgram));
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

IRInstruction *create_ir_instruction(IRType type, int dest, int src1, int src2, int src3, int *line)
{
    IRInstruction* instruction = malloc(sizeof(IRInstruction));
    assert(instruction != NULL);
    instruction->type = type;
    instruction->dest = dest;
    instruction->src1 = src1;
    instruction->src2 = src2;
    instruction->src3 = src3;
    instruction->line = (*line)++;
    instruction->count = 0;
    instruction->next = NULL;
    return instruction;
}

void free_ir_instruction(IRInstruction *instruction)
{
    while (instruction) {
        IRInstruction* next = instruction->next;
        free(instruction);
        instruction = next;
    }
}


int getNextFreeRegister() {
    for (int i = 0; i < 30; i++) {
        if (registers[i] == 0) {
            registers[i] = 1;
            return i;
        }
    }
    exit(EXIT_FAILURE);
}

void freeRegister(int i) {
    registers[i] = 0;
}

Expression *getArguments(FunctionCall *function_call) {

    int64_t *arguments = malloc(sizeof(int) * MAX_ARGS);
    for (int i = 0; i < MAX_ARGS; i++) {
        arguments[i] = NOT_USED;
    }
    int ct = 0;
    Arguments *args = function_call->args;
    while(args != NULL) {
        arguments[ct++] = args->arg;
        args = args->next;
    }
    return arguments;
}

void insertInstruction(IRProgram *program, IRInstruction *instruction)
{
    if (program->head == NULL) {
        program->head = instruction;
        program->tail = instruction;
    } else {
        program->tail->next = instruction;
        program->tail = instruction;
    }
}

void insertProgram(IRProgram *program, IRProgram *block)
{
    if (program->head == NULL) {
        program->head = block->head;
        program->tail = block->tail;
    } else {
        program->tail->next = block->head;
        program->tail = block->tail;
    }
}


int64_t *search_vars(char *name, State *State) {
    if (State->map_size > 0) {
        for(int i = 0; i < State->map_size; i++) {
            if (State->map[i].name == name) {
                return &(State->map[i].value);
            }    
        }
    }
    return NULL;
}

