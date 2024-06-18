#ifndef AST_TO_IR_H
#define AST_TO_IR_H


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "utlis_ir.c"
#include "optimise_ir.c"



IRInstruction *AssignmentStmt_to_IR(AssignmentStmt *assign_stmt, State *state, int *line);


IRInstruction *FlowStmt_to_IR(FlowStmt *flow_stmt, int return_address, State *state, int *line);


IRProgram *ForStmt_to_IR(ForStmt *for_stmt, State *state, int *line);


IRProgram *WhileStmt_to_IR(WhileStmt *while_stmt, State *state, int *line);


IRProgram *FunctionDef_to_IR(FunctionDef *function_def, State *caller_state, State *state, int *line);


#endif