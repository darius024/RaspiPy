#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "utlis_ir.c"
#include "optimise_ir.c"
#include "AST_to_IR.h"

IRProgram *AST_to_IR(Program *prog, State *state)
{
    // Create an IR program
    IRProgram *program = create_ir_program();
    State *state = create_state();

    int line = 0;

    insertProgram(program, Statements_to_IR(prog->statements, state, &line));

    return program;
}


IRProgram *Statements_to_IR(Statements *statements, State *state, int *line)
{
    // Create an IR program
    IRProgram *program = create_ir_program();

    // Always append the state
    while(statements->next != NULL) {
        switch(statements->statement->tag) {
            case ASSIGNMENT_STMT:
                AssignmentStmt *stmt = statements->statement->assignment_stmt;
                insertInstruction(program, stmt);
                break;
            case FLOW_STMT:
                FlowStmt *stmt = statements->statement->flow_stmt;
                insertInstruction(program, FlowStmt_to_IR(stmt, 0, state, line));
                // Change return address
                break;
            case FOR_STMT:
                ForStmt *stmt = statements->statement->for_stmt;
                insertProgram(program, ForStmt_to_IR(stmt, state, line));
                break;
            case WHILE_STMT:
                WhileStmt *stmt = statements->statement->while_stmt;
                insertProgram(program, WhileStmt_to_IR(stmt, state, line));
                break;
            case IF_STMT:
                IfStmt *stmt = statements->statement->if_stmt;
                insertProgram(program, IfStmt_to_IR(stmt, state, line));
                break;
            case FUNCTION_DEF:
                FunctionDef *stmt = statements->statement->function_def;
                State *callee_state = create_state();
                insertProgram(program, FunctionDef_to_IR(stmt, state, callee_state, line));
                break;
        }
        statements = statements->next;
    }
    return program;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}




// TODO: Flow return_address figuring out
// TODO: Handle function calls in evalExpr
// TODO: Augmented Assign
// Load and Store instrctions
// Directives Handling
// TODO: fix header file errors
// TODO: figure out how to handle function call evaluation
// TODO: decided int64_t* or int64_t output for eval expression given it might not work 
// TODO: implement registers intead of immediate value for statements in a Function def IR
// TODO: more putBits
