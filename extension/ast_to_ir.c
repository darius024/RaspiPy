#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "ast_to_ir.h"
#include "eval.h"
#include "ir.h"
#include "rpy.h"
#include "utils_ir.h"

#define INITIAL_COUNT 1

int64_t registers[NUM_REGISTERS];

static void statements_to_ir(IRProgram *program, Statements *statements, State *state, int *line, int count_update);

static void assignment_to_ir(IRProgram *program, AssignmentStmt *assign_stmt, State *state, int *line, int count_update)
{
    if (check_gpio_assignment(program, assign_stmt, state, line)) return;
    if (check_wait(program, assign_stmt, state, line)) return;

    uint8_t reg;
    IRInstruction *instr;
    if (assign_stmt->expression->tag == EXPR_INT && assign_stmt->expression->int_value->value < MOVZ_MAX) {
        reg = (strcmp(assign_stmt->name, "ret") == 0)
            ? X0 : get_register(state, assign_stmt->name);
        instr = create_ir_instruction(IR_MOVZ, reg, assign_stmt->expression->int_value->value, NOT_USED, NOT_USED, line);
        instr->dest->type = REGW;
        instr->src1->type = IMM;
        update_state(state, reg, assign_stmt->expression->int_value->value);
        insert_instruction(program, instr, count_update);
    } else {
        uint8_t expr_reg = eval_expression(program, assign_stmt->expression, state, line, count_update);
        reg = (strcmp(assign_stmt->name, "ret") == 0)
            ? X0 : get_register(state, assign_stmt->name);
        if (reg != expr_reg) {
            instr = create_ir_instruction(IR_MOV, reg, expr_reg, NOT_USED, NOT_USED, line);
            instr->dest->type = get_operation_type(&reg, &expr_reg, NULL);
            instr->src1->type = get_operation_type(&reg, &expr_reg, NULL);
            insert_instruction(program, instr, count_update);
            update_state(state, reg, registers[expr_reg]);
        }
    }
}

static void flow_to_ir(IRProgram *program, FlowStmt *flow_stmt, State *state, int *line, int count_update, IRInstruction *instr)
{
    if (!strcmp(flow_stmt->name, "return")) { // Return statement
        if (check_loop_return(program, flow_stmt, state, line)) return;
        if (flow_stmt->expression != NULL) {
            AssignmentStmt *assign = create_assignment_stmt("ret", flow_stmt->expression);
            assignment_to_ir(program, assign, state, line, count_update);
            free_assignment_stmt(assign);
        }
        //  Jump to return address
        IRInstruction *branch_to_func_call = create_ir_instruction(IR_BR, RP, NOT_USED, NOT_USED, NOT_USED, line);
        branch_to_func_call->dest->type = REGX;
        insert_instruction(program, branch_to_func_call, count_update);
    } else { // Break / Continue statement
        IRInstruction *flow_instr = create_ir_instruction(IR_B, 0, NOT_USED, NOT_USED, NOT_USED, line);
        flow_instr->dest->type = LABEL;
        insert_instruction(program, flow_instr, count_update);
    }
}


static void for_to_ir(IRProgram *program, ForStmt *for_stmt, State *state, int *line, int count_update)
{
    Arguments *args = for_stmt->range->function_call->args;
    assert(args != NULL); // low_bound
    assert(args->next != NULL); // upp_bound

    int low_bound = args->arg->int_value->value;
    int upp_bound = args->next->arg->int_value->value;
    // function_call is of form:    range(low_bound, upp_bound)

    // Assignment
    AssignmentStmt *assign = create_assignment_stmt(for_stmt->var, args->arg);
    assignment_to_ir(program, assign, state, line, count_update);

    // Condition check
    int64_t line_to_return = *line;
    uint8_t var_reg = get_register(state, assign->name);
    IRInstruction *condition;
    if (upp_bound < MOVZ_MAX) {
        condition = create_ir_instruction(IR_CMP, var_reg, upp_bound, NOT_USED, NOT_USED, line);
        condition->dest->type = REGW;
        condition->src1->type = IMM;
    } else {
        uint8_t right_op = eval_expression(program, args->next->arg, state, line, count_update);
        condition = create_ir_instruction(IR_CMP, var_reg, right_op, NOT_USED, NOT_USED, line);
        condition->dest->type = get_operation_type(&var_reg, &right_op, NULL);
        condition->src1->type = get_operation_type(&var_reg, &right_op, NULL);
    }
    insert_instruction(program, condition, count_update);

    IRInstruction *condition2 = create_ir_instruction(IR_BCOND, B_GE, 0, NOT_USED, NOT_USED, line);
    condition2->dest->type = BC;
    condition2->src1->type = LABEL;
    insert_instruction(program, condition2, count_update);

    // Convert block statements to IR
    statements_to_ir(program, for_stmt->block, state, line, count_update + upp_bound - low_bound);

    // Update counter
    IRInstruction *update_counter = create_ir_instruction(IR_ADD, var_reg, var_reg, 1, NOT_USED, line);
    update_counter->dest->type = get_operation_type(&var_reg, &var_reg, NULL);
    update_counter->src1->type = get_operation_type(&var_reg, &var_reg, NULL);
    update_counter->src2->type = IMM;
    insert_instruction(program, update_counter, count_update);


    // Unconditional branch to condition check
    add_label(state, line_to_return, NULL);
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, get_label_address(state), NOT_USED, NOT_USED, NOT_USED, line);
    branch_uncond->dest->type = LABEL;
    insert_instruction(program, branch_uncond, count_update);

    // Update branch condition location
    add_label(state, *line, NULL);
    condition2->src1->value = get_label_address(state);

    free_assignment_stmt(assign);
}

static void while_to_ir(IRProgram *program, WhileStmt *while_stmt, State *state, int *line, int count_update)
{
    assert(while_stmt->condition->tag == EXPR_BINARY_OP);

    // Condition check
    int line_to_return = *line;
    BinaryOp *binary_op = while_stmt->condition->binary_op;
    uint8_t left_op = eval_expression(program, binary_op->left, state, line, count_update);
    IRInstruction *condition;
    if (binary_op->right->tag == EXPR_INT && binary_op->right->int_value->value < MOVZ_MAX) {
        condition = create_ir_instruction(IR_CMP, left_op, binary_op->right->int_value->value, NOT_USED, NOT_USED, line);
        condition->dest->type = REGW;
        condition->src1->type = IMM;
    } else {
        uint8_t right_op = eval_expression(program, binary_op->right, state, line, count_update);
        condition = create_ir_instruction(IR_CMP, left_op, right_op, NOT_USED, NOT_USED, line);
        condition->dest->type = get_operation_type(&left_op, &right_op, NULL);
        condition->src1->type = get_operation_type(&left_op, &right_op, NULL);
    }
    insert_instruction(program, condition, count_update);

    IRInstruction *condition2 = create_ir_instruction(IR_BCOND, get_negated_comparison(binary_op), 0, NOT_USED, NOT_USED, line);
    condition2->dest->type = BC;
    insert_instruction(program, condition2, count_update);

    // Convert block statements to IR
    statements_to_ir(program, while_stmt->block, state, line, count_update);

    // Unconditional branch to condition check
    add_label(state, line_to_return, NULL);
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, get_label_address(state), NOT_USED, NOT_USED, NOT_USED, line);
    branch_uncond->dest->type = LABEL;
    insert_instruction(program, branch_uncond, count_update);

    // Update branch condition location
    add_label(state, *line, NULL);
    condition2->src1->value = get_label_address(state);
    condition2->src1->type = LABEL;
}

static void if_to_ir(IRProgram *program, IfStmt *if_stmt, State *state, int *line, int count_update)
{
    // Condition check
    BinaryOp *binary_op = if_stmt->condition->binary_op;
    uint8_t left_op = eval_expression(program, binary_op->left, state, line, count_update);

    IRInstruction *condition;
    if (binary_op->right->tag == EXPR_INT && binary_op->right->int_value->value < MOVZ_MAX) {
        condition = create_ir_instruction(IR_CMP, left_op, binary_op->right->int_value->value, NOT_USED, NOT_USED, line);
        condition->dest->type = REGW;
        condition->src1->type = IMM;
    } else {
        uint8_t right_op = eval_expression(program, binary_op->right, state, line, count_update);
        condition = create_ir_instruction(IR_CMP, left_op, right_op, NOT_USED, NOT_USED, line);
        condition->dest->type = get_operation_type(&left_op, &right_op, NULL);
        condition->src1->type = get_operation_type(&left_op, &right_op, NULL);
    }
    insert_instruction(program, condition, count_update);

    IRInstruction *condition2 = create_ir_instruction(IR_BCOND, get_negated_comparison(binary_op), 0, NOT_USED, NOT_USED, line);
    condition2->dest->type = BC;
    condition2->src1->type = LABEL;
    insert_instruction(program, condition2, count_update);

    // Then block
    statements_to_ir(program, if_stmt->then_block, state, line, count_update);

    // Unconditional branch past the else block
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, 0, NOT_USED, NOT_USED, NOT_USED, line);
    branch_uncond->dest->type = LABEL;
    insert_instruction(program, branch_uncond, count_update);

    // Update branch condition location for the else block
    add_label(state, *line, NULL);
    condition2->src1->value = get_label_address(state);

    // Else block
    statements_to_ir(program, if_stmt->else_block, state, line, count_update);

    // Update the unconditional branch to skip the else block
    add_label(state, *line, NULL);
    branch_uncond->dest->value = get_label_address(state);
}

static void function_def_to_ir(IRProgram *program, FunctionDef *function_def, State *state, int *line, int count_update)
{
    // Add function to the state
    add_fun(state, *line, function_def->name);
    
    // Create self-contained callee state
    State *callee_state = create_state();
    append_state(state, callee_state);

    // Create instructions for function parameters
    Parameters *params = function_def->parameters;
    int param_count = 0;
    while (params != NULL) {
        add_name(callee_state, param_count++, params->parameter->name);
        params = params->next;
    }

    // Convert function body statements to IR
    statements_to_ir(program, function_def->body, callee_state, line, count_update);

    // Update the function with its callee state
    update_state_fun(state, function_def->name, callee_state);
    update_state_fun(callee_state, function_def->name, callee_state);
    // Append states (directives  + maybe symtable)

    // Returning is handle by the return statement
}

static void statements_to_ir(IRProgram *program, Statements *statements, State *state, int *line, int count_update)
{
    while(statements != NULL) {
        switch(statements->statement->tag) {
            case ASSIGNMENT_STMT: {
                AssignmentStmt *assignment_stmt = statements->statement->assignment_stmt;
                assignment_to_ir(program, assignment_stmt, state, line, count_update);
                break;
            }
            case FLOW_STMT: {
                FlowStmt *flow_stmt = statements->statement->flow_stmt;
                IRInstruction *instr = NULL;
                flow_to_ir(program, flow_stmt, state, line, count_update, instr);
                break;
            }
            case FOR_STMT: {
                ForStmt *for_stmt = statements->statement->for_stmt;
                for_to_ir(program, for_stmt, state, line, count_update);
                break;
            }
            case WHILE_STMT: {
                WhileStmt *while_stmt = statements->statement->while_stmt;
                while_to_ir(program, while_stmt, state, line, count_update);
                break;
            }
            case IF_STMT: {
                IfStmt *if_stmt = statements->statement->if_stmt;
                if_to_ir(program, if_stmt, state, line, count_update);
                break;
            }
            case FUNCTION_DEF: {
                break;
            }
        }
        statements = statements->next;
    }
}


IRProgram *ast_to_ir(Program *prog, State *state)
{
    // Create an IR program
    IRProgram *program = create_ir_program();

    // Clear the global registers
    for(int i = 0; i < NUM_REGISTERS; i++) {
        registers[i] = NOT_USED;
    }

    int line = 0;
    // Branch to the main program (Step over function definitions)
    IRInstruction *branch_to_main = create_ir_instruction(IR_B, 0, NOT_USED, NOT_USED, NOT_USED, &line);
    branch_to_main->dest->type = LABEL;
    insert_instruction(program, branch_to_main, INITIAL_COUNT);

    // Traverse function defintions
    Statements *statements = prog->statements;
    while(statements != NULL && statements->statement->tag == FUNCTION_DEF) {
        FunctionDef *function_definition = statements->statement->function_def;
        function_def_to_ir(program, function_definition, state, &line, INITIAL_COUNT);
        statements = statements->next;
    }

    // Set up "main" label
    add_label(state, line, "main");
    branch_to_main->dest->value = get_label_address(state);

    // Set up SP
    push_directive(state, state->stack_pointer, "stack_pointer");
    IRInstruction *set_sp = create_ir_instruction(IR_LDR, SP, get_directive_address(state), NOT_USED, NOT_USED, &line);
    set_sp->dest->type = REGX;
    set_sp->src1->type = DIR;
    insert_instruction(program, set_sp, INITIAL_COUNT);

    // Traverse the main function
    statements_to_ir(program, prog->statements, state, &line, INITIAL_COUNT);

    // Inser the halt instruction at the end
    IRInstruction *halt = create_ir_instruction(IR_AND, 0, 0, 0, NOT_USED, &line);
    halt->dest->type = REGX;
    halt->src1->type = REGX;
    halt->src2->type = REGX;
    insert_instruction(program, halt, INITIAL_COUNT);

    return program;
}
