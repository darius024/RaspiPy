#include "ast.h"
#include <inttypes.h>

// Create functions
Program *create_program(Statements *statements) {
    Program *program = (Program *)malloc(sizeof(Program));
    program->statements = statements;
    return program;
}

Statements *create_statements(Statement *statement) {
    Statements *statements = (Statements *)malloc(sizeof(Statements));
    statements->statement = statement;
    statements->next = NULL;
    return statements;
}

Statements* append_statements(Statements *stmts, Statement *stmt) {
    if (stmts == NULL) {
        return create_statements(stmt);
    } else {
        Statements *current = stmts;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_statements(stmt);
        return stmts;
    }
}

Statement *create_statement(StatementTag tag, void *stmt) {
    Statement *statement = (Statement *)malloc(sizeof(Statement));
    statement->tag = tag;
    switch (tag) {
        case ASSIGNMENT_STMT:
            statement->assignment_stmt = (AssignmentStmt *)stmt;
            break;
        case FLOW_STMT:
            statement->flow_stmt = (FlowStmt *)stmt;
            break;
        case IF_STMT:
            statement->if_stmt = (IfStmt *)stmt;
            break;
        case WHILE_STMT:
            statement->while_stmt = (WhileStmt *)stmt;
            break;
        case FOR_STMT:
            statement->for_stmt = (ForStmt *)stmt;
            break;
        case FUNCTION_DEF:
            statement->function_def = (FunctionDef *)stmt;
            break;
        default:
            free(statement);
            return NULL;
    }
    return statement;
}

AssignmentStmt *create_assignment_stmt(char *name, Expression *expression) {
    AssignmentStmt *assignment_stmt = (AssignmentStmt *)malloc(sizeof(AssignmentStmt));
    assignment_stmt->name = strdup(name);
    assignment_stmt->expression = expression;
    return assignment_stmt;
}

FlowStmt *create_flow_stmt(char *name, Expression *expression) {
    FlowStmt *flow_stmt = (FlowStmt *)malloc(sizeof(FlowStmt));
    flow_stmt->name = strdup(name);
    flow_stmt->expression = expression;
    return flow_stmt;
}

IfStmt *create_if_stmt(Expression *condition, Statements *then_block, Statements *else_block) {
    IfStmt *if_stmt = (IfStmt *)malloc(sizeof(IfStmt));
    if_stmt->condition = condition;
    if_stmt->then_block = then_block;
    if_stmt->else_block = else_block;
    return if_stmt;
}

WhileStmt *create_while_stmt(Expression *condition, Statements *block) {
    WhileStmt *while_stmt = (WhileStmt *)malloc(sizeof(WhileStmt));
    while_stmt->condition = condition;
    while_stmt->block = block;
    return while_stmt;
}

ForStmt *create_for_stmt(char *var, Expression *range, Statements *block) {
    ForStmt *for_stmt = (ForStmt *)malloc(sizeof(ForStmt));
    for_stmt->var = strdup(var);
    for_stmt->range = range;
    for_stmt->block = block;
    return for_stmt;
}

FunctionDef *create_function_def(char *name, Parameters *parameters, Statements *body) {
    FunctionDef *function_def = (FunctionDef *)malloc(sizeof(FunctionDef));
    function_def->name = strdup(name);
    function_def->parameters = parameters;
    function_def->body = body;
    return function_def;
}

Parameters *create_parameters(Name *parameter) {
    Parameters *parameters = (Parameters *)malloc(sizeof(Parameters));
    parameters->parameter = parameter;
    parameters->next = NULL;
    return parameters;
}

Parameters* append_parameters(Parameters *params, Name *name) {
    if (params == NULL) {
        return create_parameters(name);
    } else {
        Parameters *current = params;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_parameters(name);
        return params;
    }
}

Arguments *create_arguments(Expression *expression) {
    Arguments *arguments = (Arguments *)malloc(sizeof(Arguments));
    arguments->arg = expression;
    arguments->next = NULL;
    return arguments;
}

Arguments* append_arguments(Arguments *args, Expression *expr) {
    if (args == NULL) {
        return create_arguments(expr);
    } else {
        Arguments *current = args;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_arguments(expr);
        return args;
    }
}

Name *create_name(char *name) {
    Name *n = (Name *)malloc(sizeof(Name));
    n->name = strdup(name);
    return n;
}

Int *create_int(int64_t value) {
    Int *i = (Int *)malloc(sizeof(Int));
    i->value = value;
    return i;
}

BinaryOp *create_binary_op(char *op, Expression *left, Expression *right) {
    BinaryOp *binary_op = (BinaryOp *)malloc(sizeof(BinaryOp));
    binary_op->op = strdup(op);
    binary_op->left = left;
    binary_op->right = right;
    return binary_op;
}

UnaryOp *create_unary_op(char *op, Expression *expression) {
    UnaryOp *unary_op = (UnaryOp *)malloc(sizeof(UnaryOp));
    unary_op->op = strdup(op);
    unary_op->expression = expression;
    return unary_op;
}

FunctionCall *create_function_call(char *name, Arguments *args) {
    FunctionCall *function_call = (FunctionCall *)malloc(sizeof(FunctionCall));
    function_call->name = strdup(name);
    function_call->args = args;
    int count = 0;
    for (Arguments *current = args ; current != NULL ; current = current -> next) {
        count++;
    }
    function_call->arg_count = count;
    return function_call;
}

Expression *create_expression(ExpressionTag tag, void *value) {
    Expression *expr = (Expression *)malloc(sizeof(Expression));
    expr->tag = tag;
    switch (tag) {
        case EXPR_NAME:
            expr->name = (Name *)value;
            break;
        case EXPR_INT:
            expr->int_value = (Int *)value;
            break;
        case EXPR_BINARY_OP:
            expr->binary_op = (BinaryOp *)value;
            break;
        case EXPR_UNARY_OP:
            expr->unary_op = (UnaryOp *)value;
            break;
        case EXPR_FUNCTION_CALL:
            expr->function_call = (FunctionCall *)value;
            break;
        default:
            free(expr);
            return NULL;
    }
    return expr;
}

// Free functions
void free_program(Program *program) {
    if (program) {
        free_statements(program->statements);
        free(program);
    }
}

void free_statements(Statements *statements) {
    if (statements) {
        free_statement(statements->statement);
        free_statements(statements->next);
        free(statements);
    }
}

void free_statement(Statement *statement) {
    if (statement) {
        switch (statement->tag) {
            case ASSIGNMENT_STMT:
                free_assignment_stmt(statement->assignment_stmt);
                break;
            case FLOW_STMT:
                free_flow_stmt(statement->flow_stmt);
                break;
            case IF_STMT:
                free_if_stmt(statement->if_stmt);
                break;
            case WHILE_STMT:
                free_while_stmt(statement->while_stmt);
                break;
            case FOR_STMT:
                free_for_stmt(statement->for_stmt);
                break;
            case FUNCTION_DEF:
                free_function_def(statement->function_def);
                break;
        }
        free(statement);
    }
}

void free_assignment_stmt(AssignmentStmt *assignment_stmt) {
    if (assignment_stmt) {
        free(assignment_stmt->name);
        free_expression(assignment_stmt->expression);
        free(assignment_stmt);
    }
}

void free_flow_stmt(FlowStmt *flow_stmt) {
    if (flow_stmt) {
        free(flow_stmt->name);
        free_expression(flow_stmt->expression);
        free(flow_stmt);
    }
}

void free_if_stmt(IfStmt *if_stmt) {
    if (if_stmt) {
        free_expression(if_stmt->condition);
        free_statements(if_stmt->then_block);
        free_statements(if_stmt->else_block);
        free(if_stmt);
    }
}

void free_while_stmt(WhileStmt *while_stmt) {
    if (while_stmt) {
        free_expression(while_stmt->condition);
        free_statements(while_stmt->block);
        free(while_stmt);
    }
}

void free_for_stmt(ForStmt *for_stmt) {
    if (for_stmt) {
        free(for_stmt->var);
        free_expression(for_stmt->range);
        free_statements(for_stmt->block);
        free(for_stmt);
    }
}

void free_function_def(FunctionDef *function_def) {
    if (function_def) {
        free(function_def->name);
        free_parameters(function_def->parameters);
        free_statements(function_def->body);
        free(function_def);
    }
}

void free_name(Name *name) {
    if (name) {
        free(name->name);
        free(name);
    }
}

void free_int(Int *int_value) {
    if (int_value) {
        free(int_value);
    }
}

void free_binary_op(BinaryOp *binary_op) {
    if (binary_op) {
        free(binary_op->op);
        free_expression(binary_op->left);
        free_expression(binary_op->right);
        free(binary_op);
    }
}

void free_unary_op(UnaryOp *unary_op) {
    if (unary_op) {
        free(unary_op->op);
        free_expression(unary_op->expression);
        free(unary_op);
    }
}

void free_function_call(FunctionCall *function_call) {
    if (function_call) {
        free(function_call->name);
        free_arguments(function_call->args);
        free(function_call);
    }
}

void free_expression(Expression *expression) {
    if (expression) {
        switch (expression->tag) {
            case EXPR_NAME:
                free_name(expression->name);
                break;
            case EXPR_INT:
                free_int(expression->int_value);
                break;
            case EXPR_BINARY_OP:
                free_binary_op(expression->binary_op);
                break;
            case EXPR_UNARY_OP:
                free_unary_op(expression->unary_op);
                break;
            case EXPR_FUNCTION_CALL:
                free_function_call(expression->function_call);
                break;
        }
        free(expression);
    }
}

void free_parameters(Parameters *parameters) {
    while (parameters) {
        Parameters *next = parameters->next;
        free_name(parameters->parameter);
        free(parameters);
        parameters = next;
    }
}

void free_arguments(Arguments *arguments) {
    while (arguments) {
        Arguments *next = arguments->next;
        free_expression(arguments->arg);
        free(arguments);
        arguments = next;
    }
}
