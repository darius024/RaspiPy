#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "eval.h"
#include "ir.h"
#include "optimise_ir.h"
#include "rpy.h"
#include "utils_ir.h"

extern int64_t registers[NUM_REGISTERS];

/*
 * Evaluate expression
 * Assume that we can always compute the value
 * Apply constand propagation and folding optimizations here
*/
uint8_t eval_expression(IRProgram *program, Expression *expression, State *state, int *line, int count_update)
{
    // Apply optimizations
    // expression = const_prop(expression, state);
    switch (expression->tag) {
        case EXPR_NAME: {
            return get_register(state, expression->name->name);
        }
        case EXPR_INT: {
            uint8_t reg = get_free_intermediary_register(state);
            IRInstruction *instr;
            if (expression->int_value->value < MOVZ_MAX) {
                instr = create_ir_instruction(IR_MOVZ, reg, expression->int_value->value, NOT_USED, NOT_USED, line);
                instr->dest->type = REGW;
                instr->src1->type = IMM;
            } else {
                push_directive(state, expression->int_value->value, NULL);
                instr = create_ir_instruction(IR_LDR, reg, get_directive_address(state), NOT_USED, NOT_USED, line);
                instr->dest->type = REGX;
                instr->src1->type = DIR;
            }
            insert_instruction(program, instr, count_update);
            update_state(state, reg, expression->int_value->value);
            return reg;
        }
        case EXPR_BINARY_OP: {
            IRType type;
            uint8_t left_reg = eval_expression(program, expression->binary_op->left, state, line, count_update);
            uint8_t right_reg = eval_expression(program, expression->binary_op->right, state, line, count_update);
            uint8_t dest_reg = get_free_intermediary_register(state);
            char *op = expression->binary_op->op;
            // Do not support / and % yet,nor << and >> yet
            if(strcmp(op, "+") == 0) {
                type = IR_ADD;
                update_state(state, dest_reg, left_reg + right_reg);
            } else if(strcmp(op, "-") == 0) {
                type = IR_SUB;
                update_state(state, dest_reg, left_reg - right_reg);
            } else if(strcmp(op, "*") == 0) {
                type = IR_MUL;
                update_state(state, dest_reg, left_reg * right_reg);
            } else if(strcmp(op, "|") == 0) {
                type = IR_ORR;
                update_state(state, dest_reg, left_reg | right_reg);
            } else if(strcmp(op, "^") == 0) {
                type = IR_EOR;
                update_state(state, dest_reg, left_reg ^ right_reg);
            } else if(strcmp(op, "&") == 0) {
                type = IR_AND;
                update_state(state, dest_reg, left_reg & right_reg);
            } else {
                exit(EXIT_FAILURE);
            }

            IRInstruction *instr = create_ir_instruction(type, dest_reg, left_reg, right_reg, NOT_USED, line);
            instr->dest->type = get_operation_type(&dest_reg, &left_reg, &right_reg);
            instr->src1->type = get_operation_type(&dest_reg, &left_reg, &right_reg);
            instr->src2->type = get_operation_type(&dest_reg, &left_reg, &right_reg);
            insert_instruction(program, instr, count_update);
            free_intermediary_register(state, left_reg);
            free_intermediary_register(state, right_reg);
            return dest_reg;
        }
        case EXPR_UNARY_OP: {
            IRType type;
            uint8_t src_reg = eval_expression(program, expression->unary_op->expression, state, line, count_update);
            uint8_t dest_reg = get_free_intermediary_register(state);
            switch(expression->unary_op->op[0]) {
                case '-': type = IR_NEG; update_state(state, dest_reg, -registers[src_reg]); break;
                case '~': type = IR_MVN; update_state(state, dest_reg, ~registers[src_reg]); break;
                default: exit(EXIT_FAILURE);
            }

            IRInstruction *instr = create_ir_instruction(type, dest_reg, src_reg, NOT_USED, NOT_USED, line);
            instr->dest->type = get_operation_type(&dest_reg, &src_reg, NULL);
            instr->src1->type = get_operation_type(&dest_reg, &src_reg, NULL);
            insert_instruction(program, instr, count_update);
            free_intermediary_register(state, src_reg);
            return dest_reg;
        }
        case EXPR_FUNCTION_CALL: {
            if (check_loop_call(program, expression->function_call, state, line)) return X0;

            int branch_line = get_fun_address(state, expression->function_call->name);
            // Save return address
            push_to_stack(program, state, RP, line);
            IRInstruction *save_return_addr = create_ir_instruction(IR_MOVZ, RP, 8 * (*line + 1), NOT_USED, NOT_USED, line);
            save_return_addr->dest->type = REGX;
            save_return_addr->src1->type = IMM;
            insert_instruction(program, save_return_addr, count_update);
            // Store arguments in registers
            Arguments *args = expression->function_call->args;
            uint8_t arg_reg;
            int arg_count = 0;
            while (args != NULL && arg_count < MAX_ARGS) {
                save_register(program, state, arg_count, line);
                IRInstruction *store_arg;
                if (args->arg->tag == EXPR_INT && args->arg->int_value->value < MOVZ_MAX) {
                    store_arg = create_ir_instruction(IR_MOVZ, arg_count, args->arg->int_value->value, NOT_USED, NOT_USED, line);
                    store_arg->src1->type = IMM;
                    update_state(state, arg_count, args->arg->int_value->value);
                } else {
                    arg_reg = eval_expression(program, args->arg, state, line, count_update);
                    store_arg = create_ir_instruction(IR_MOV, arg_count, arg_reg, NOT_USED, NOT_USED, line);
                    store_arg->src1->type = REGX;
                    update_state(state, arg_count, registers[arg_reg]);
                }
                store_arg->dest->type = REGX;
                if (arg_count != arg_reg) {
                    insert_instruction(program, store_arg, count_update);
                } else {
                    free_ir_instruction(store_arg);
                }
                args = args->next;
                arg_count++;
            }
            // Call function
            IRInstruction *call_instr = create_ir_instruction(IR_B, branch_line, NOT_USED, NOT_USED, NOT_USED, line);
            call_instr->dest->type = LABEL;
            insert_instruction(program, call_instr, count_update);
            // Returning and setting X0 is done by the return statement
            // Restore arguments in registers
            arg_count--;
            while (arg_count >= 0) {
                restore_register(program, state, arg_count, line);
                arg_count--;
            }

            // Restore return address
            pop_from_stack(program, state, RP, line);
            return X0;
        }
        default: {
            exit(EXIT_FAILURE);
        }
    }
}
