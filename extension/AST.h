#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdint.h>

typedef enum {
    is_program,
    is_func_def,
    is_func_call,
    is_param_list,
    is_stmts,
    is_assignment_stmt,
    is_flow_stmt,
    is_if_stmt,
    is_while_stmt,
    is_for_stmt,
    is_test,
    is_bexpr,
    is_expr,
    is_binary_expr,
    is_unary_expr,
    is_factor,
    is_name,
    is_number,
} NodeType;

typedef struct Program Program;
typedef struct FuncDef FuncDef;
typedef struct FuncCall FuncCall;
typedef struct ParamList ParamList;
typedef struct Stmts Stmts;
typedef struct Stmt Stmt;
typedef struct AssignmentStmt AssignmentStmt;
typedef struct FlowStmt FlowStmt;
typedef struct IfStmt IfStmt;
typedef struct WhileStmt WhileStmt;
typedef struct ForStmt ForStmt;
typedef struct Test Test;
typedef struct BExpr BExpr;
typedef struct Expr Expr;
typedef struct ExprList ExprList;
typedef struct BinaryExpr BinaryExpr;
typedef struct UnaryExpr UnaryExpr;
typedef struct Name Name;
typedef struct Number Number;

struct Program {
    FuncDef *func_defs;
    Stmts *stmts;
};

struct FuncDef {
    char *name;
    ParamList *param_list;
    Stmts *block;
};

struct FuncCall {
    char *name;
    ExprList *expr_list;
};

struct ParamList {
    ParamList *params;
    char *name;
};

struct Stmts {
    Stmts *stmts;
    Stmt *stmt;
};

struct Stmt {
    NodeType tag;
    union {
        AssignmentStmt *assign_stmt;
        FlowStmt *flow_stmt;
        IfStmt *if_stmt;
        WhileStmt *while_stmt;
        ForStmt *for_stmt;
    };
};

struct AssignmentStmt {
    char *name;
    Expr *expr;
};

struct FlowStmt {
    char *name;
};

struct IfStmt {
    Test *test;
    Stmts *then_block;
    Stmts *else_block;
};

struct WhileStmt {
    Test *test;
    Stmts *block;
};

struct ForStmt {
    Expr *expr;
    Test *test;
    Stmts *block;
};

struct Test {
    Test *left;
    Test *right;
    char *op;
};

struct BExpr {
    Expr *left;
    Expr *right;
    char *op;
};

struct Expr {
    NodeType tag;
    union {
        Number *num;
        Name *name;
        FuncCall *func_call;
        BinaryExpr *binary_expr;
        UnaryExpr *unary_expr;
    };
};

struct ExprList {
    Expr *expr;
    ExprList *next;
};

struct BinaryExpr {
    Expr *left;
    Expr *right;
    char *op;
};

struct UnaryExpr {
    Expr *expr;
    char *op;
};

struct Name {
    char *name;
};

struct Number {
    int value;
};

// Function declarations
Program *create_program_node(FuncDef *func_defs, Stmts *stmts);
FuncDef *create_func_def_node(char *name, ParamList *param_list, Stmts *block);
FuncCall *create_func_call_node(char *name, ExprList *expr_list);
ParamList *create_param_list_node(ParamList *params, char *name);
Stmts *create_stmts_node(Stmts *stmts, Stmt *stmt);
Stmt *create_assignment_stmt_node(char *name, Expr *expr);
Stmt *create_flow_stmt_node(char *name);
Stmt *create_if_stmt_node(Test *test, Stmts *then_block, Stmts *else_block);
Stmt *create_while_stmt_node(Test *test, Stmts *block);
Stmt *create_for_stmt_node(Expr *expr, Test *test, Stmts *block);
Test *create_test_node(Test *left, Test *right, char *op);
BExpr *create_bexpr_node(Expr *left, Expr *right, char *op);
Expr *create_expr_node(NodeType tag, void *data);
ExprList *create_expr_list_node(Expr *expr, ExprList *next);

// Free functions
void free_program_node(Program *node);
void free_func_def_node(FuncDef *node);
void free_func_call_node(FuncCall *node);
void free_param_list_node(ParamList *node);
void free_stmts_node(Stmts *node);
void free_stmt_node(Stmt *node);
void free_assignment_stmt_node(AssignmentStmt *node);
void free_flow_stmt_node(FlowStmt *node);
void free_if_stmt_node(IfStmt *node);
void free_while_stmt_node(WhileStmt *node);
void free_for_stmt_node(ForStmt *node);
void free_test_node(Test *node);
void free_bexpr_node(BExpr *node);
void free_expr_node(Expr *node);
void free_expr_list_node(ExprList *node);
void free_binary_expr_node(BinaryExpr *node);
void free_unary_expr_node(UnaryExpr *node);
void free_name_node(Name *node);
void free_number_node(Number *node);

#endif
