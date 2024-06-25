#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "ast_to_ir.h"
#include "ir.h"
#include "lexer.h"
#include "parser.h"
#include "print.h"
#include "../src/io.h"

#define testAssignmentsNum 2
#define testLoopsNum 2
#define testFunNum 2
#define testIfNum 2

extern bool sameAST(Statements *ast1, Statements *ast2);
extern bool sameExpression(Expression *e1, Expression *e2);
extern bool sameStatement(Statement *s1, Statement *s2);

// using non-reentrant lexer
extern int yyparse(void);
extern int yylex_destroy (void);


extern Program *program;
extern int yyerrors;

const char *arithmFiles[testAssignmentsNum] = {"testData/arithm1.py", "testData/arithm2.py"};
const char *loopFiles[testLoopsNum] = {"testData/loop1.py", "testData/loop2.py"};
const char *funFiles[testFunNum] = {"testData/fun1.py", "testData/fun2.py"};
const char *ifFiles[testIfNum] = {"testData/if1.py", "testDataa/ifelse1.py"};

const char r = 'r';

bool sameProgram(Program *p1, Program *p2) {
    if (p1 == NULL && p2 == NULL) {
        return true;
    }
    if (p1 == NULL || p2 == NULL) {
        return false;
    }
    return sameAST(p1 -> statements, p2 -> statements);
}

bool sameAST(Statements *ast1, Statements *ast2) {
    if (ast1 == NULL && ast2 == NULL) {
        return true;
    }
    if (ast1 == NULL || ast2 == NULL) {
        return false;
    }
    return sameStatement(ast1->statement, ast2->statement) && sameAST(ast1->next, ast2->next);
}


bool sameExpression(Expression *e1, Expression *e2) {
    if (e1 == NULL && e2 == NULL) {
        return true;
    }
    if (e1 == NULL || e2 == NULL) {
        return false;
    }
    if (e1->tag != e2->tag) {
        return false;
    }

    switch (e1->tag) {
        case EXPR_NAME:
            return strcmp(e1->name->name, e2->name->name) == 0;

        case EXPR_INT:
            return e1->int_value->value == e2->int_value->value;

        case EXPR_BINARY_OP:
            return strcmp(e1->binary_op->op, e2->binary_op->op) == 0 &&
                   sameExpression(e1->binary_op->left, e2->binary_op->left) &&
                   sameExpression(e1->binary_op->right, e2->binary_op->right);

        case EXPR_UNARY_OP:
            return strcmp(e1->unary_op->op, e2->unary_op->op) == 0 &&
                   sameExpression(e1->unary_op->expression, e2->unary_op->expression);

        case EXPR_FUNCTION_CALL:
            if (strcmp(e1->function_call->name, e2->function_call->name) != 0 ||
                e1->function_call->arg_count != e2->function_call->arg_count) {
                return false;
            }

            Arguments *a1 = e1->function_call->args;
            Arguments *a2 = e2->function_call->args;

            while (a1 && a2) {
                if (!sameExpression(a1->arg, a2->arg)) {
                    return false;
                }
                a1 = a1->next;
                a2 = a2->next;
            }
            return a1 == NULL && a2 == NULL;

        default:
            return false;
    }
}

bool sameStatement(Statement *s1, Statement *s2) {
    if (s1->tag != s2->tag) {
        return false;
    }

    switch (s1->tag) {
        case ASSIGNMENT_STMT:
            return strcmp(s1->assignment_stmt->name, s2->assignment_stmt->name) == 0 &&
                   sameExpression(s1->assignment_stmt->expression, s2->assignment_stmt->expression);

        case FLOW_STMT:
            return strcmp(s1->flow_stmt->name, s2->flow_stmt->name) == 0 &&
                   sameExpression(s1->flow_stmt->expression, s2->flow_stmt->expression);

        case IF_STMT:
            return sameExpression(s1->if_stmt->condition, s2->if_stmt->condition) &&
                   sameAST(s1->if_stmt->then_block, s2->if_stmt->then_block) &&
                   sameAST(s1->if_stmt->else_block, s2->if_stmt->else_block);

        case WHILE_STMT:
            return sameExpression(s1->while_stmt->condition, s2->while_stmt->condition) &&
                   sameAST(s1->while_stmt->block, s2->while_stmt->block);

        case FOR_STMT:
            return strcmp(s1->for_stmt->var, s2->for_stmt->var) == 0 &&
                   sameExpression(s1->for_stmt->range, s2->for_stmt->range) &&
                   sameAST(s1->for_stmt->block, s2->for_stmt->block);

        case FUNCTION_DEF:
            if (strcmp(s1->function_def->name, s2->function_def->name) != 0 ||
                s1->function_def->param_count != s2->function_def->param_count ||
                !sameAST(s1->function_def->body, s2->function_def->body)) {
                return false;
            }

            Parameters *p1 = s1->function_def->parameters;
            Parameters *p2 = s2->function_def->parameters;

            while (p1 && p2) {
                if (strcmp(p1->parameter->name, p2->parameter->name) != 0) {
                    return false;
                }
                p1 = p1->next;
                p2 = p2->next;
            }
            return p1 == NULL && p2 == NULL;

        default:
            return false;
    }
}

Program *testAssignments() {
    // Creating expected results
    Program *expectedPrograms[testAssignmentsNum];

    // arithm1.py
    // a = 2
    Int *arithm1_int_2 = create_int(2);
    Expression *arithm1_expr_2 = create_expression(EXPR_INT, arithm1_int_2);
    AssignmentStmt *arithm1_assign_a_2 = create_assignment_stmt("a", arithm1_expr_2);
    Statement *arithm1_stmt1 = create_statement(ASSIGNMENT_STMT, arithm1_assign_a_2);

    // b = 3
    Int *arithm1_int_3 = create_int(3);
    Expression *arithm1_expr_3 = create_expression(EXPR_INT, arithm1_int_3);
    AssignmentStmt *arithm1_assign_b_3 = create_assignment_stmt("b", arithm1_expr_3);
    Statement *arithm1_stmt2 = create_statement(ASSIGNMENT_STMT, arithm1_assign_b_3);

    // a = a + b
    Name *arithm1_name_a2 = create_name("a");
    Name *arithm1_name_b = create_name("b");
    Expression *arithm1_expr_a = create_expression(EXPR_NAME, arithm1_name_a2);
    Expression *arithm1_expr_b = create_expression(EXPR_NAME, arithm1_name_b);
    BinaryOp *arithm1_binary_op = create_binary_op("+", arithm1_expr_a, arithm1_expr_b);
    Expression *arithm1_expr_a_plus_b = create_expression(EXPR_BINARY_OP, arithm1_binary_op);
    AssignmentStmt *arithm1_assign_a_a_plus_b = create_assignment_stmt("a", arithm1_expr_a_plus_b);
    Statement *arithm1_stmt3 = create_statement(ASSIGNMENT_STMT, arithm1_assign_a_a_plus_b);

    // Link statements
    Statements *arithm1_statements1 = create_statements(arithm1_stmt1);
    Statements *arithm1_statements2 = append_statements(arithm1_statements1, arithm1_stmt2);
    Statements *arithm1_statements3 = append_statements(arithm1_statements2, arithm1_stmt3);

    // Create program
    expectedPrograms[0] = create_program(arithm1_statements3);

    // arithm2.py

    // a = 3 * 4
    Int *arithm2_int_3 = create_int(3);
    Int *arithm2_int_4 = create_int(4);
    Expression *arithm2_expr_3 = create_expression(EXPR_INT, arithm2_int_3);
    Expression *arithm2_expr_4 = create_expression(EXPR_INT, arithm2_int_4);
    BinaryOp *arithm2_binary_op_mult = create_binary_op("*", arithm2_expr_3, arithm2_expr_4);
    Expression *arithm2_expr_3_mult_4 = create_expression(EXPR_BINARY_OP, arithm2_binary_op_mult);
    AssignmentStmt *arithm2_assign_a_3_mult_4 = create_assignment_stmt("a", arithm2_expr_3_mult_4);
    Statement *arithm2_stmt1 = create_statement(ASSIGNMENT_STMT, arithm2_assign_a_3_mult_4);

    // b = a - 3
    Name *arithm2_name_a = create_name("a");
    Expression *arithm2_expr_a = create_expression(EXPR_NAME, arithm2_name_a);
    Expression *arithm2_expr_int_3 = create_expression(EXPR_INT, arithm2_int_3);
    BinaryOp *arithm2_binary_op_sub = create_binary_op("-", arithm2_expr_a, arithm2_expr_int_3);
    Expression *arithm2_expr_a_sub_3 = create_expression(EXPR_BINARY_OP, arithm2_binary_op_sub);
    AssignmentStmt *arithm2_assign_b_a_sub_3 = create_assignment_stmt("b", arithm2_expr_a_sub_3);
    Statement *arithm2_stmt2 = create_statement(ASSIGNMENT_STMT, arithm2_assign_b_a_sub_3);

    // Link statements
    Statements *arithm2_statements1 = create_statements(arithm2_stmt1);
    Statements *arithm2_statements2 = append_statements(arithm2_statements1, arithm2_stmt2);

    // Create program
    expectedPrograms[1] = create_program(arithm2_statements2);

    // comparing
    for (int i = 0 ; i < testAssignmentsNum ; i++) {
        yyin = loadInputFile(arithmFiles[i], NULL, &r);
        if (yyparse() == 0) {
            if (sameProgram(expectedPrograms[i], program)) {
                printf("TEST %d OK\n", i);
                print_program(program);
            }
            else {
                printf("FAIL\n");
                printf("  EXPECTED\n");
                print_program(expectedPrograms[i]);
                printf("    GOT\n");
                print_program(program);
            }
        } 
        else {
            fprintf(stderr, "Parsing failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Free the expected programs
    for (int i = 0; i < testAssignmentsNum; i++) {
        free_program(expectedPrograms[i]);
    }

    return program;
}

Program *testLoops() {
    // Creating expected results
    Program *expectedPrograms[testLoopsNum];


    // loop1.py
    // for i in range(0, 3):
    //     count = count + 1

    // Create function call range(0, 3)
    Int *fun1_int_0 = create_int(0);
    Int *fun1_int_3 = create_int(3);
    Expression *fun1_expr_0 = create_expression(EXPR_INT, fun1_int_0);
    Expression *fun1_expr_3 = create_expression(EXPR_INT, fun1_int_3);
    Arguments *fun1_args_0 = create_arguments(fun1_expr_0);
    Arguments *fun1_args_3 = append_arguments(fun1_args_0, fun1_expr_3);
    FunctionCall *fun1_call_range = create_function_call("range", fun1_args_3);
    Expression *fun1_expr_call_range = create_expression(EXPR_FUNCTION_CALL, fun1_call_range);

    // Create increment count expression
    Name *loop1_name_count2 = create_name("count");
    Expression *loop1_expr_count2 = create_expression(EXPR_NAME, loop1_name_count2);
    Int *loop1_int_1 = create_int(1);
    Expression *loop1_expr_1 = create_expression(EXPR_INT, loop1_int_1);
    BinaryOp *loop1_binary_op_plus = create_binary_op("+", loop1_expr_count2, loop1_expr_1);
    Expression *loop1_expr_count_plus_1 = create_expression(EXPR_BINARY_OP, loop1_binary_op_plus);
    AssignmentStmt *loop1_assign_count = create_assignment_stmt("count", loop1_expr_count_plus_1);
    Statement *loop1_stmt_body = create_statement(ASSIGNMENT_STMT, loop1_assign_count);

    // Create for loop statement
    Statements *loop1_body = create_statements(loop1_stmt_body);
    ForStmt *loop1_for_stmt = create_for_stmt("i", fun1_expr_call_range, loop1_body);
    Statement *loop1_stmt = create_statement(FOR_STMT, loop1_for_stmt);

    // Link statements
    Statements *loop1_statements = create_statements(loop1_stmt);

    // Create program
    expectedPrograms[0] = create_program(loop1_statements);

    // loop2.py
    // count = 0
    // while (count < 5):
    //     count = count + 1

    // Create initial count assignment
    Int *loop2_int_0 = create_int(0);
    Expression *loop2_expr_0 = create_expression(EXPR_INT, loop2_int_0);
    AssignmentStmt *loop2_assign_count_0 = create_assignment_stmt("count", loop2_expr_0);
    Statement *loop2_stmt1 = create_statement(ASSIGNMENT_STMT, loop2_assign_count_0);

    // Create while loop condition
    Name *loop2_name_count1 = create_name("count");
    Expression *loop2_expr_count1 = create_expression(EXPR_NAME, loop2_name_count1);
    Int *loop2_int_5 = create_int(5);
    Expression *loop2_expr_5 = create_expression(EXPR_INT, loop2_int_5);
    BinaryOp *loop2_binary_op_less = create_binary_op("<", loop2_expr_count1, loop2_expr_5);
    Expression *loop2_expr_count_less_5 = create_expression(EXPR_BINARY_OP, loop2_binary_op_less);

    // Create increment count expression inside while loop
    Name *loop2_name_count3 = create_name("count");
    Expression *loop2_expr_count3 = create_expression(EXPR_NAME, loop2_name_count3);
    Int *loop2_int_1 = create_int(1);
    Expression *loop2_expr_1 = create_expression(EXPR_INT, loop2_int_1);
    BinaryOp *loop2_binary_op_plus = create_binary_op("+", loop2_expr_count3, loop2_expr_1);
    Expression *loop2_expr_count_plus_1 = create_expression(EXPR_BINARY_OP, loop2_binary_op_plus);
    AssignmentStmt *loop2_assign_count = create_assignment_stmt("count", loop2_expr_count_plus_1);
    Statement *loop2_stmt_body = create_statement(ASSIGNMENT_STMT, loop2_assign_count);

    // Create while loop statement
    Statements *loop2_body = create_statements(loop2_stmt_body);
    WhileStmt *loop2_while_stmt = create_while_stmt(loop2_expr_count_less_5, loop2_body);
    Statement *loop2_stmt2 = create_statement(WHILE_STMT, loop2_while_stmt);

    // Link statements
    Statements *loop2_statements1 = create_statements(loop2_stmt1);
    Statements *loop2_statements2 = append_statements(loop2_statements1, loop2_stmt2);

    // Create program
    expectedPrograms[1] = create_program(loop2_statements2);

    // Printing for debugging
    print_program(expectedPrograms[0]);
    
    // Comparing
    for (int i = 0; i < testLoopsNum; i++) {
        yyin = loadInputFile(loopFiles[i], NULL, &r);
        if (yyparse() == 0) {
            if (sameProgram(expectedPrograms[i], program)) {
                printf("TEST %d OK\n", i);
            } else {
                printf("FAIL\n");
                printf("  EXPECTED\n");
                print_program(expectedPrograms[i]);
                printf("    GOT\n");
                print_program(program);
            }
        } else {
            fprintf(stderr, "Parsing failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Free the expected programs
    for (int i = 0; i < testLoopsNum; i++) {
        free_program(expectedPrograms[i]);
    }

    return program;
}


Program *testFunDefs() {
    // Creating expected results
    Program *expectedPrograms[testFunNum];

    // fun1.py
    // def add(a, b):
    //    return a + b
    // a = 0
    // a = add(1, 2)

    // Create return expression for add
    Name *fun1_name_a1 = create_name("a");
    Name *fun1_name_b1 = create_name("b");
    Expression *fun1_expr_a1 = create_expression(EXPR_NAME, fun1_name_a1);
    Expression *fun1_expr_b1 = create_expression(EXPR_NAME, fun1_name_b1);
    BinaryOp *fun1_binary_op_add = create_binary_op("+", fun1_expr_a1, fun1_expr_b1);
    Expression *fun1_expr_add = create_expression(EXPR_BINARY_OP, fun1_binary_op_add);
    FlowStmt *fun1_return_stmt_add = create_flow_stmt("return", fun1_expr_add);
    Statement *fun1_stmt_return_add = create_statement(FLOW_STMT, fun1_return_stmt_add);

    // Create function add definition
    Parameters *fun1_param_a = create_parameters(fun1_name_a1);
    Parameters *fun1_param_b = append_parameters(fun1_param_a, fun1_name_b1);
    Statements *fun1_body_add = create_statements(fun1_stmt_return_add);
    FunctionDef *fun1_function_add = create_function_def("add", fun1_param_b, fun1_body_add);
    Statement *fun1_stmt_function_add = create_statement(FUNCTION_DEF, fun1_function_add);

    // Create initial assignment a = 0
    Int *fun1_int_0 = create_int(0);
    Expression *fun1_expr_0 = create_expression(EXPR_INT, fun1_int_0);
    AssignmentStmt *fun1_assign_a_0 = create_assignment_stmt("a", fun1_expr_0);
    Statement *fun1_stmt_assign_a_0 = create_statement(ASSIGNMENT_STMT, fun1_assign_a_0);

    // Create assignment a = add(1, 2)
    Int *fun1_int_1 = create_int(1);
    Int *fun1_int_2 = create_int(2);
    Expression *fun1_expr_1 = create_expression(EXPR_INT, fun1_int_1);
    Expression *fun1_expr_2 = create_expression(EXPR_INT, fun1_int_2);
    Arguments *fun1_args_1 = create_arguments(fun1_expr_1);
    Arguments *fun1_args_2 = append_arguments(fun1_args_1, fun1_expr_2);
    FunctionCall *fun1_call_add = create_function_call("add", fun1_args_2);
    Expression *fun1_expr_call_add = create_expression(EXPR_FUNCTION_CALL, fun1_call_add);
    AssignmentStmt *fun1_assign_a_add = create_assignment_stmt("a", fun1_expr_call_add);
    Statement *fun1_stmt_assign_a_add = create_statement(ASSIGNMENT_STMT, fun1_assign_a_add);

    // Link statements
    Statements *fun1_statements1 = create_statements(fun1_stmt_function_add);
    Statements *fun1_statements2 = append_statements(fun1_statements1, fun1_stmt_assign_a_0);
    Statements *fun1_statements3 = append_statements(fun1_statements2, fun1_stmt_assign_a_add);

    // Create program
    expectedPrograms[0] = create_program(fun1_statements3);

    // fun2.py
    // def sub(a, b):
    //    return a - b
    // a = 0
    // a = sub(a, 3)

    // Create return expression for sub
    Name *fun2_name_a1 = create_name("a");
    Name *fun2_name_b1 = create_name("b");
    Expression *fun2_expr_a1 = create_expression(EXPR_NAME, fun2_name_a1);
    Expression *fun2_expr_b1 = create_expression(EXPR_NAME, fun2_name_b1);
    BinaryOp *fun2_binary_op_sub = create_binary_op("-", fun2_expr_a1, fun2_expr_b1);
    Expression *fun2_expr_sub = create_expression(EXPR_BINARY_OP, fun2_binary_op_sub);
    FlowStmt *fun2_return_stmt_sub = create_flow_stmt("return", fun2_expr_sub);
    Statement *fun2_stmt_return_sub = create_statement(FLOW_STMT, fun2_return_stmt_sub);

    // Create function sub definition
    Parameters *fun2_param_a = create_parameters(fun2_name_a1);
    Parameters *fun2_param_b = append_parameters(fun2_param_a, fun2_name_b1);
    Statements *fun2_body_sub = create_statements(fun2_stmt_return_sub);
    FunctionDef *fun2_function_sub = create_function_def("sub", fun2_param_b, fun2_body_sub);
    Statement *fun2_stmt_function_sub = create_statement(FUNCTION_DEF, fun2_function_sub);

    // Create initial assignment a = 0
    Int *fun2_int_0 = create_int(0);
    Expression *fun2_expr_0 = create_expression(EXPR_INT, fun2_int_0);
    AssignmentStmt *fun2_assign_a_0 = create_assignment_stmt("a", fun2_expr_0);
    Statement *fun2_stmt_assign_a_0 = create_statement(ASSIGNMENT_STMT, fun2_assign_a_0);

    // Create assignment a = sub(a, 3)
    Name *fun2_name_a2 = create_name("a");
    Expression *fun2_expr_a2 = create_expression(EXPR_NAME, fun2_name_a2);
    Int *fun2_int_3 = create_int(3);
    Expression *fun2_expr_3 = create_expression(EXPR_INT, fun2_int_3);
    Arguments *fun2_args_a = create_arguments(fun2_expr_a2);
    Arguments *fun2_args_3 = append_arguments(fun2_args_a, fun2_expr_3);
    FunctionCall *fun2_call_sub = create_function_call("sub", fun2_args_3);
    Expression *fun2_expr_call_sub = create_expression(EXPR_FUNCTION_CALL, fun2_call_sub);
    AssignmentStmt *fun2_assign_a_sub = create_assignment_stmt("a", fun2_expr_call_sub);
    Statement *fun2_stmt_assign_a_sub = create_statement(ASSIGNMENT_STMT, fun2_assign_a_sub);

    // Link statements
    Statements *fun2_statements1 = create_statements(fun2_stmt_function_sub);
    Statements *fun2_statements2 = append_statements(fun2_statements1, fun2_stmt_assign_a_0);
    Statements *fun2_statements3 = append_statements(fun2_statements2, fun2_stmt_assign_a_sub);

    // Create program
    expectedPrograms[1] = create_program(fun2_statements3);

    // Comparing
    for (int i = 0; i < testFunNum; i++) {
        yyin = loadInputFile(funFiles[i], NULL, &r);
        if (yyparse() == 0) {
            if (sameProgram(expectedPrograms[i], program)) {
                printf("TEST %d OK\n", i);
            } else {
                printf("FAIL\n");
                printf("  EXPECTED\n");
                print_program(expectedPrograms[i]);
                printf("    GOT\n");
                print_program(program);
            }
        } else {
            fprintf(stderr, "Parsing failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Free the expected programs
    for (int i = 0; i < testFunNum; i++) {
        free_program(expectedPrograms[i]);
    }

    return program;
}


int main(void) {
    printf("Parsing assignment tests.\n");
    Program *assignProgram = testAssignments();
    Program *loopProgram = testLoops();

    State *assignState = create_state();
    IRProgram *assignIR = ast_to_ir(assignProgram, assignState);
    State *loopState = create_state();
    IRProgram *loopIR = ast_to_ir(loopProgram, loopState);

    printf("\n");
    printf("Compiling to assembly.\n");

    print_ir_program(assignIR, assignState, stdout);
    free_state(assignState);
    free_program(assignProgram);

    printf("\n");

    print_ir_program(loopIR, loopState, stdout);
    free_state(loopState);
    free_program(loopProgram);

    // testFunDefs();
    return EXIT_SUCCESS;
}
