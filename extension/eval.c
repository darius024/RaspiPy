#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "ir.h"
#include "eval.h"
#include "optimise_ir.h"
#include "utils_ir.h"

/*
 * Evaluate expression
 * Assume that we can always compute the value
 * Apply constand propagation and folding optimizations here
*/
uint8_t evalExpression(IRProgram *program, Expression *expression, State *state, int *line, int count_update)
{
    // Apply optimizations
    expression = const_prop(expression, state);
    switch (expression->tag) {
        case EXPR_NAME: {
            return getRegister(expression->name, state);
        }
        case EXPR_INT: {
            uint8_t reg = getNextFreeRegister();
            IRInstruction *instr = create_ir_instruction(IR_MOV, reg, expression->int_value->value, NOT_USED, NOT_USED, line);
            instr->dest->type = REG;
            instr->src1->type = IMM;
            insertInstruction(program, instr, count_update);
            updateState(state, reg, expression->int_value->value);
            return reg;
        }
        case EXPR_BINARY_OP: {
            uint8_t left_reg = evalExpression(program, expression->binary_op->left, state, line, count_update);
            uint8_t right_reg = evalExpression(program, expression->binary_op->right, state, line, count_update);
            uint8_t dest_reg = getNextFreeRegister();
            IRType type;
            char *op = expression->binary_op->op;
            // Do not support / and % yet,nor << and >> yet
            if(strcmp(op, "+") == 0) {
                type = IR_ADD;
            } else if(strcmp(op, "-") == 0) {
                type = IR_SUB;
            } else if(strcmp(op, "*") == 0) {
                type = IR_MUL;
            } else if(strcmp(op, "|") == 0) {
                type = IR_ORR;
            } else if(strcmp(op, "^") == 0) {
                type = IR_EOR;
            } else if(strcmp(op, "&") == 0) {
                type = IR_AND;
            } else {
                exit(EXIT_FAILURE);
            }
            IRInstruction *instr = create_ir_instruction(type, dest_reg, left_reg, right_reg, NOT_USED, line);
            instr->dest->type = REG;
            instr->src1->type = REG;
            instr->src2->type = REG;
            insertInstruction(program, instr, count_update);
            freeNonVarRegister(state, left_reg);
            freeNonVarRegister(state, right_reg);
            return dest_reg;
        }
        case EXPR_UNARY_OP: {
            int src_reg = evalExpression(program, expression->unary_op->expression, state, line, count_update);
            int dest_reg = getNextFreeRegister();
            IRType type;
            switch(expression->unary_op->op[0]) {
                case '-':
                    type = IR_NEG;
                    break;
                case '~':
                    type = IR_MVN;
                    break;
                default:
                    exit(EXIT_FAILURE);
            }
            IRInstruction *instr = create_ir_instruction(type, dest_reg, src_reg, NOT_USED, NOT_USED, line);
            instr->dest->type = REG;
            instr->src1->type = REG;
            insertInstruction(program, instr, count_update);
            freeNonVarRegister(state, src_reg);
            return dest_reg;
        }
        case EXPR_FUNCTION_CALL: {
            int branch_line = NOT_USED;
            for (int i = 0; i < state->funcs_size; i++) {
                if (strcmp(state->funcs[i].name, expression->function_call->name) == 0) {
                    branch_line = state->funcs[i].line;
                    break;
                }
            }
            assert(branch_line != NOT_USED);
            // Save return address
            push_to_stack(program, state, SP, line);
            IRInstruction *save_return_addr = create_ir_instruction(IR_MOVZ, SP, *line + 1, NOT_USED, NOT_USED, line);
            save_return_addr->dest->type = REG;
            save_return_addr->src1->type = IMM;
            insertInstruction(program, save_return_addr, count_update);
            // Save return register
            saveRegister(program, state, X0, line);
            // Store arguments in registers
            Arguments *arg = expression->function_call->args;
            int arg_reg;
            int arg_count = 1;
            while (arg != NULL && arg_count < MAX_ARGS) {
                arg_reg = evalExpression(program, arg->arg, state, line, count_update);
                saveRegister(program, state, arg_count,  line);
                IRInstruction *store_arg = create_ir_instruction(IR_MOV, arg_count, arg_reg, NOT_USED, NOT_USED, line);
                insertInstruction(program, store_arg, count_update);
                arg = arg->next;
                arg_count++;
            }
            // Call function
            IRInstruction *call_instr = create_ir_instruction(IR_B, branch_line, NOT_USED, NOT_USED, NOT_USED, line);
            call_instr->dest->type = LABEL;
            insertInstruction(program, call_instr, count_update);
            // Returning and setting X0 is done by the return statement
            // Restore arguments in registers
            arg_count--;
            while (arg_count > 0) {
                restoreRegister(program, state, arg_count, line);
                arg_count--;
            }
            restoreRegister(program, state, X0, line);
            return X0;
        }
        default: {
            exit(EXIT_FAILURE);
        }
    }
}
