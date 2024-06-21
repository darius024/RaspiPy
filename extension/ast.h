#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Program Program;
typedef struct Statements Statements;
typedef struct Statement Statement;
typedef struct AssignmentStmt AssignmentStmt;
typedef struct FlowStmt FlowStmt;
typedef struct IfStmt IfStmt;
typedef struct WhileStmt WhileStmt;
typedef struct ForStmt ForStmt;
typedef struct FunctionDef FunctionDef;
typedef struct Parameters Parameters;
typedef struct Arguments Arguments;
typedef struct Expression Expression;
typedef struct Name Name;
typedef struct Int Int;
typedef struct BinaryOp BinaryOp;
typedef struct UnaryOp UnaryOp;
typedef struct FunctionCall FunctionCall;

enum StatementTag {
    ASSIGNMENT_STMT,
    FLOW_STMT,
    IF_STMT,
    WHILE_STMT,
    FOR_STMT,
    FUNCTION_DEF
};

enum ExpressionTag {
    EXPR_NAME,
    EXPR_INT,
    EXPR_BINARY_OP,
    EXPR_UNARY_OP,
    EXPR_FUNCTION_CALL
};

typedef enum StatementTag StatementTag;
typedef enum ExpressionTag ExpressionTag;


struct Program {
    Statements *statements;
};

struct Statements {
    Statement *statement;
    Statements *next;
};

struct Statement {
    StatementTag tag;
    union {
        AssignmentStmt *assignment_stmt;
        FlowStmt *flow_stmt;
        IfStmt *if_stmt;
        WhileStmt *while_stmt;
        ForStmt *for_stmt;
        FunctionDef *function_def;
    };
};

struct AssignmentStmt {
    char *name;
    Expression *expression;
};

struct FlowStmt {
    char *name; // return, break, continue
    Expression *expression;
};

struct IfStmt {
    Expression *condition;
    Statements *then_block;
    Statements *else_block;
};

struct WhileStmt {
    Expression *condition;
    Statements *block;
};

struct ForStmt {
    char *var;
    Expression *range;
    Statements *block;
};

struct FunctionDef {
    char *name;
    Parameters *parameters;
    int param_count;
    Statements *body;
};

struct Parameters {
    Name *parameter;
    Parameters *next;
};

struct Arguments {
    Expression *arg;
    Arguments *next;
};

struct Expression {
    ExpressionTag tag;
    union {
        Name *name;
        Int *int_value;
        BinaryOp *binary_op;
        UnaryOp *unary_op;
        FunctionCall *function_call;
    };
};

struct Name {
    char *name;
};

struct Int {
    int64_t value;
};

struct BinaryOp {
    char *op;
    Expression *left;
    Expression *right;
};

struct UnaryOp {
    char *op;
    Expression *expression;
};

struct FunctionCall {
    char *name;
    Arguments *args;
    int arg_count;
};

// Function prototypes
Program *create_program(Statements *statements);
Statements *create_statements(Statement *statement);
Statements* append_statements(Statements *stmts, Statement *stmt);

AssignmentStmt *create_assignment_stmt(char *name, Expression *expression);
FlowStmt *create_flow_stmt(char *name, Expression *expression);
IfStmt *create_if_stmt(Expression *condition, Statements *then_block, Statements *else_block);
WhileStmt *create_while_stmt(Expression *condition, Statements *block);
ForStmt *create_for_stmt(char *var, Expression *range, Statements *block);
FunctionDef *create_function_def(char *name, Parameters *parameters, Statements *body);
Statement *create_statement(StatementTag tag, void *stmt);

Parameters *create_parameters(Name *parameter);
Parameters* append_parameters(Parameters *params, Name *name);
Arguments *create_arguments(Expression *expression);
Arguments* append_arguments(Arguments *args, Expression *expr);

Name *create_name(char *name);
Int *create_int(int64_t value);
BinaryOp *create_binary_op(char *op, Expression *left, Expression *right);
UnaryOp *create_unary_op(char *op, Expression *expression);
FunctionCall *create_function_call(char *name, Arguments *args);
Expression *create_expression(ExpressionTag tag, void *value);


void free_program(Program *program);
void free_statements(Statements *statements);

void free_statement(Statement *statement);
void free_assignment_stmt(AssignmentStmt *assignment_stmt);
void free_flow_stmt(FlowStmt *flow_stmt);
void free_if_stmt(IfStmt *if_stmt);
void free_while_stmt(WhileStmt *while_stmt);
void free_for_stmt(ForStmt *for_stmt);
void free_function_def(FunctionDef *function_def);

void free_parameters(Parameters *parameters);
void free_arguments(Arguments *arguments);

void free_name(Name *name);
void free_int(Int *int_value);
void free_binary_op(BinaryOp *binary_op);
void free_unary_op(UnaryOp *unary_op);
void free_function_call(FunctionCall *function_call);
void free_expression(Expression *expression);

// void print_ast(Program *)

#endif
