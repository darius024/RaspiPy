#include "AST.h"
#include <stdlib.h>
#include <string.h>

// Creation functions
Program *create_program_node(FuncDef *func_defs, Stmts *stmts) {
    Program *node = (Program *)malloc(sizeof(Program));
    node->func_defs = func_defs;
    node->stmts = stmts;
    return node;
}

FuncDef *create_func_def_node(char *name, ParamList *param_list, Stmts *block) {
    FuncDef *node = (FuncDef *)malloc(sizeof(FuncDef));
    node->name = strdup(name);
    node->param_list = param_list;
    node->block = block;
    return node;
}

FuncCall *create_func_call_node(char *name, ExprList *expr_list) {
    FuncCall *node = (FuncCall *)malloc(sizeof(FuncCall));
    node->name = strdup(name);
    node->expr_list = expr_list;
    return node;
}

ParamList *create_param_list_node(ParamList *params, char *name) {
    ParamList *node = (ParamList *)malloc(sizeof(ParamList));
    node->params = params;
    node->name = strdup(name);
    return node;
}

Stmts *create_stmts_node(Stmts *stmts, Stmt *stmt) {
    Stmts *node = (Stmts *)malloc(sizeof(Stmts));
    node->stmts = stmts;
    node->stmt = stmt;
    return node;
}

Stmt *create_assignment_stmt_node(char *name, Expr *expr) {
    AssignmentStmt *assign_stmt = (AssignmentStmt *)malloc(sizeof(AssignmentStmt));
    assign_stmt->name = strdup(name);
    assign_stmt->expr = expr;

    Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
    stmt->tag = is_assignment_stmt;
    stmt->assign_stmt = assign_stmt;
    return stmt;
}

Stmt *create_flow_stmt_node(char *name) {
    FlowStmt *flow_stmt = (FlowStmt *)malloc(sizeof(FlowStmt));
    flow_stmt->name = strdup(name);

    Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
    stmt->tag = is_flow_stmt;
    stmt->flow_stmt = flow_stmt;
    return stmt;
}

Stmt *create_if_stmt_node(Test *test, Stmts *then_block, Stmts *else_block) {
    IfStmt *if_stmt = (IfStmt *)malloc(sizeof(IfStmt));
    if_stmt->test = test;
    if_stmt->then_block = then_block;
    if_stmt->else_block = else_block;

    Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
    stmt->tag = is_if_stmt;
    stmt->if_stmt = if_stmt;
    return stmt;
}

Stmt *create_while_stmt_node(Test *test, Stmts *block) {
    WhileStmt *while_stmt = (WhileStmt *)malloc(sizeof(WhileStmt));
    while_stmt->test = test;
    while_stmt->block = block;

    Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
    stmt->tag = is_while_stmt;
    stmt->while_stmt = while_stmt;
    return stmt;
}

Stmt *create_for_stmt_node(Expr *expr, Test *test, Stmts *block) {
    ForStmt *for_stmt = (ForStmt *)malloc(sizeof(ForStmt));
    for_stmt->expr = expr;
    for_stmt->test = test;
    for_stmt->block = block;

    Stmt *stmt = (Stmt *)malloc(sizeof(Stmt));
    stmt->tag = is_for_stmt;
    stmt->for_stmt = for_stmt;
    return stmt;
}

Test *create_test_node(Test *left, Test *right, char *op) {
    Test *node = (Test *)malloc(sizeof(Test));
    node->left = left;
    node->right = right;
    node->op = strdup(op);
    return node;
}

BExpr *create_bexpr_node(Expr *left, Expr *right, char *op) {
    BExpr *node = (BExpr *)malloc(sizeof(BExpr));
    node->left = left;
    node->right = right;
    node->op = strdup(op);
    return node;
}

Expr *create_expr_node(NodeType tag, void *data) {
    Expr *expr = (Expr *)malloc(sizeof(Expr));
    expr->tag = tag;
    switch (tag) {
        case is_number:
            expr->num = (Number *)data;
            break;
        case is_name:
            expr->name = (Name *)data;
            break;
        case             is_func_call:
            expr->func_call = (FuncCall *)data;
            break;
        case is_binary_expr:
            expr->binary_expr = (BinaryExpr *)data;
            break;
        case is_unary_expr:
            expr->unary_expr = (UnaryExpr *)data;
            break;
        default:
            break;
    }
    return expr;
}

ExprList *create_expr_list_node(Expr *expr, ExprList *next) {
    ExprList *node = (ExprList *)malloc(sizeof(ExprList));
    node->expr = expr;
    node->next = next;
    return node;
}

// Free functions
void free_program_node(Program *node) {
    if (!node) return;
    free_func_def_node(node->func_defs);
    free_stmts_node(node->stmts);
    free(node);
}

void free_func_def_node(FuncDef *node) {
    if (!node) return;
    free(node->name);
    free_param_list_node(node->param_list);
    free_stmts_node(node->block);
    free(node);
}

void free_func_call_node(FuncCall *node) {
    if (!node) return;
    free(node->name);
    free_expr_list_node(node->expr_list);
    free(node);
}

void free_param_list_node(ParamList *node) {
    if (!node) return;
    free(node->name);
    free_param_list_node(node->params);
    free(node);
}

void free_stmts_node(Stmts *node) {
    if (!node) return;
    free_stmts_node(node->stmts);
    free_stmt_node(node->stmt);
    free(node);
}

void free_stmt_node(Stmt *node) {
    if (!node) return;
    switch (node->tag) {
        case is_assignment_stmt:
            free_assignment_stmt_node(node->assign_stmt);
            break;
        case is_flow_stmt:
            free_flow_stmt_node(node->flow_stmt);
            break;
        case is_if_stmt:
            free_if_stmt_node(node->if_stmt);
            break;
        case is_while_stmt:
            free_while_stmt_node(node->while_stmt);
            break;
        case is_for_stmt:
            free_for_stmt_node(node->for_stmt);
            break;
        default:
            break;
    }
    free(node);
}

void free_assignment_stmt_node(AssignmentStmt *node) {
    if (!node) return;
    free(node->name);
    free_expr_node(node->expr);
    free(node);
}

void free_flow_stmt_node(FlowStmt *node) {
    if (!node) return;
    free(node->name);
    free(node);
}

void free_if_stmt_node(IfStmt *node) {
    if (!node) return;
    free_test_node(node->test);
    free_stmts_node(node->then_block);
    free_stmts_node(node->else_block);
    free(node);
}

void free_while_stmt_node(WhileStmt *node) {
    if (!node) return;
    free_test_node(node->test);
    free_stmts_node(node->block);
    free(node);
}

void free_for_stmt_node(ForStmt *node) {
    if (!node) return;
    free_expr_node(node->expr);
    free_test_node(node->test);
    free_stmts_node(node->block);
    free(node);
}

void free_test_node(Test *node) {
    if (!node) return;
    free_test_node(node->left);
    free_test_node(node->right);
    free(node->op);
    free(node);
}

void free_bexpr_node(BExpr *node) {
    if (!node) return;
    free_expr_node(node->left);
    free_expr_node(node->right);
    free(node->op);
    free(node);
}

void free_expr_node(Expr *node) {
    if (!node) return;
    switch (node->tag) {
        case is_number:
            free_number_node(node->num);
            break;
        case is_name:
            free_name_node(node->name);
            break;
        case is_func_call:
            free_func_call_node(node->func_call);
            break;
        case is_binary_expr:
            free_binary_expr_node(node->binary_expr);
            break;
        case is_unary_expr:
            free_unary_expr_node(node->unary_expr);
            break;
        default:
            break;
    }
    free(node);
}

void free_expr_list_node(ExprList *node) {
    if (!node) return;
    free_expr_node(node->expr);
    free_expr_list_node(node->next);
    free(node);
}

void free_binary_expr_node(BinaryExpr *node) {
    if (!node) return;
    free_expr_node(node->left);
    free_expr_node(node->right);
    free(node->op);
    free(node);
}

void free_unary_expr_node(UnaryExpr *node) {
    if (!node) return;
    free_expr_node(node->expr);
    free(node->op);
    free(node);
}

void free_name_node(Name *node) {
    if (!node) return;
    free(node->name);
    free(node);
}

void free_number_node(Number *node) {
    if (!node) return;
    free(node);
}

