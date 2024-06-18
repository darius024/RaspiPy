#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "AST_to_IR.h"
#include "utlis_ir.c"
#include "optimise_ir.c"



IRInstruction *AssignmentStmt_to_IR(AssignmentStmt *assign_stmt, State *state, int *line)
{
    int value = EvalExpression(assign_stmt->expression, state);
    int reg = getRegister(assign_stmt->name, state);
    IRInstruction *assign = create_ir_instruction(IR_MOV, reg, value, NOT_USED, NOT_USED, line);
    return assign;
}

IRInstruction *FlowStmt_to_IR(FlowStmt *flow_stmt, int return_address, State *state, int *line)
{
    if (strcmp(flow_stmt->name, "return") == 0 && flow_stmt->expression != NULL) {
        int value = EvalExpression(flow_stmt->expression, state);
        create_ir_instruction(IR_MOV, 0, value, NOT_USED, NOT_USED, line);
    }
    create_ir_instruction(IR_B, return_address, NOT_USED, NOT_USED, NOT_USED, line);
}

IRProgram *ForStmt_to_IR(ForStmt *for_stmt, State *state, int *line)
{
    // Create an IR program
    IRProgram *program = create_ir_program();
    int *args = getArguments(for_stmt->range->function_call);
    // function_call is of form:    range(low_bound, upp_bound)

    // Assignment
    AssignmentStmt *assign = create_assignment_stmt(for_stmt->var, args[0]);
    IRInstruction *assignment = AssignmentStmt_to_IR(assign, state, line);
    insertInstruction(program, assignment);

    // Condition check
    int line_to_return = *line;
    IRInstruction *condition = create_ir_instruction(IR_CMP, assign->name, args[1], NOT_USED, NOT_USED, line);
    insertInstruction(program, condition);
    IRInstruction *condition2 = create_ir_instruction(IR_BC, GE, NOT_USED, NOT_USED, NOT_USED, line);
    insertInstruction(program, condition2);

    // Convert block statements to IR
    IRProgram *block = Statements_to_IR(for_stmt->block, state, line);

    // Update counter
    int r = getRegister(assign->name, state);
    IRInstruction *update_counter = create_ir_instruction(IR_ADD, r, r, 1, NOT_USED, line);

    // Unconditional branch to condition check
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, line_to_return, NOT_USED, NOT_USED, NOT_USED, line);
    insertInstruction(program, branch_uncond);

    // Update branch condition location
    condition2->src1 = *line;

    return program;
}

IRProgram *WhileStmt_to_IR(WhileStmt *while_stmt, State *state, int *line)
{
    assert(while_stmt->condition->tag == EXPR_BINARY_OP);

    // Create an IR program
    IRProgram *program = create_ir_program();

    // Condition check
    int line_to_return = *line;
    BinaryOp *binary_op = while_stmt->condition->binary_op;
    BranchConditional op = getComparison(binary_op);
    int left_op = EvalExpression(binary_op->left, state);
    int right_op = EvalExpression(binary_op->right, state);
    IRInstruction *condition = create_ir_instruction(IR_CMP, left_op, right_op, NOT_USED, NOT_USED, *line);
    insertInstruction(program, condition);
    IRInstruction *condition2 = create_ir_instruction(IR_BC, op, NOT_USED, NOT_USED, NOT_USED, *line);
    insertInstruction(program, condition2);

    // Convert block statements to IR
    IRProgram *block = Statements_to_IR(while_stmt->block, state, line);

    // Unconditional branch to condition check
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, line_to_return, NOT_USED, NOT_USED, NOT_USED, line);
    insertInstruction(program, branch_uncond);

    // Update branch condition location
    condition2->src1 = *line;
}

IRProgram *IfStmt_to_IR(IfStmt *if_stmt, State *state, int *line)
{
    // Create an IR program
    IRProgram *program = create_ir_program();

    // Condition check
    BinaryOp *binary_op = if_stmt->condition->binary_op;
    BranchConditional op = getNegatedComparison(binary_op);
    int left_op = EvalExpression(binary_op->left, state);
    int right_op = EvalExpression(binary_op->right, state);
    IRInstruction *condition = create_ir_instruction(IR_CMP, left_op, right_op, NOT_USED, NOT_USED, *line);
    insertInstruction(program, condition);
    IRInstruction *condition2 = create_ir_instruction(IR_BC, op, NOT_USED, NOT_USED, NOT_USED, *line);
    insertInstruction(program, condition2);

    // Then block
    IRProgram *then_block = Statements_to_IR(if_stmt->then_block, state, line);

    // Unconditional branch past the else block
    IRInstruction *branch_uncond = create_ir_instruction(IR_B, NOT_USED, NOT_USED, NOT_USED, NOT_USED, *line);
    insertInstruction(program, branch_uncond);

    // Update branch condition location for the else block
    condition2->src1 = *line;

    // Else block
    IRProgram *else_block = Statements_to_IR(if_stmt->else_block, state, line);
    appendPrograms(program, else_block);

    // Update the unconditional branch to skip the else block
    branch_uncond->dest = *line;

    return program;
}

IRProgram *FunctionDef_to_IR(FunctionDef *function_def, State *caller_state, State *state, int *line)
{
    // Create an IR program
    IRProgram *program = create_ir_program();

    // def fun(x, y) -- x1, x2

    // Create instructions for function parameters
    Parameters *param = function_def->parameters;
    int param_count = 1;
    while (param != NULL) {
        int i;
        for (i = 0; i < caller_state->map_size && caller_state->map[i]->reg != param_count; i++);
        assert(i <= caller_state->map_size);
        strcpy(state->map[state->map_size]->name, param->parameter);
        state->map[state->map_size]->reg = param_count++;
        state->map[state->map_size++]->value = caller_state->map[i]->value;
        param = param->next;
    }

    // Convert function body statements to IR
    IRProgram *body = Statements_to_IR(function_def->body, state, line);

    return program;
}