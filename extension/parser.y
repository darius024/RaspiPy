/*
 * RaspiPy - A python like language for easier use with Raspberry Pis
 * Parser Rules - Grammar
*/

/*
extern int yywrap(void);
extern void yyerror(const char *s);
extern int lineno;
extern Program *program;
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"


extern int yylex(void);

Program *program = NULL;
int yyerrors = 0;

void yyerror(const char *str) {
    //fprintf(stderr, "Error on line %d: %s\n", yylineno, str);
    yyerrors++;
}

int yywrap(void) {
    return 1;
}

%}
// all possible types of data associated with tokens
%union {
    int num;
    char *str;
    struct Program *program;
    struct Statements *statements;
    struct Statement *statement;
    struct AssignmentStmt *assignment_stmt;
    struct FlowStmt *flow_stmt;
    struct IfStmt *if_stmt;
    struct WhileStmt *while_stmt;
    struct ForStmt *for_stmt;
    struct FunctionDef *function_def;
    struct Parameters *parameters;
    struct Arguments *arguments;
    struct Expression *expression;
    struct Name *name;
    struct Int *int_value;
    struct BinaryOp *binary_op;
    struct UnaryOp *unary_op;
    struct FunctionCall *function_call;
}

%token <num> DEC_INTEGER HEX_INTEGER 
%token <str> NAME
%token AND BREAK CONTINUE DEF ELIF ELSE FALSE FOR GLOBAL
%token IF IN NOT OR RETURN TRUE WHILE
%token L_PAREN R_PAREN COMMA COLON SEMI_COLON ASSIGN NEG
%token BITWISE_OR BITWISE_XOR BITWISE_AND
%token LEFT_SHIFT RIGHT_SHIFT ADD SUB MUL DIV MOD
%token LT GT EQ GE LE NE
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token LEFT_SHIFT_ASSIGN RIGHT_SHIFT_ASSIGN
%token INDENT DEDENT

%type <program> program
%type <statements> statements block
%type <statement> statement simple_stmt compound_stmt
%type <assignment_stmt> assignment_stmt
%type <flow_stmt> return_stmt
%type <if_stmt> if_stmt
%type <while_stmt> while_stmt
%type <for_stmt> for_stmt
%type <function_def> function_def
%type <parameters> parameters
%type <arguments> arguments
%type <expression> expression atom
%type <expression> disjunction conjunction inversion comparison bitwise_or bitwise_xor bitwise_and shift_expr sum term
%type <expression> factor
%type <expression> function_call


%right RETURN

%start program

%%
program
    : statements { program = create_program($1); }
    ;

statements
    : /* empty */ { $$ = NULL; }
    | statements statement { $$ = create_statements($2, $1); }
    ;

statement
    : compound_stmt { $$ = $1; }
    | simple_stmt { $$ = $1; }
    ;

simple_stmt
    : assignment_stmt { $$ = create_statement(ASSIGNMENT_STMT, $1); }
    | return_stmt { $$ = create_statement(FLOW_STMT, $1); }
    | BREAK { $$ = create_statement(FLOW_STMT, create_flow_stmt("break", NULL)); }
    | CONTINUE { $$ = create_statement(FLOW_STMT, create_flow_stmt("continue", NULL)); }
    ;

assignment_stmt
    : NAME ASSIGN expression { $$ = create_assignment_stmt($1, $3); }
    | NAME SUB_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("-=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME ADD_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("+=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME MUL_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("*=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME DIV_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("/=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME MOD_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("%=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME AND_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("&=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME OR_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("|=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME XOR_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("^=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME LEFT_SHIFT_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op("<<=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    | NAME RIGHT_SHIFT_ASSIGN expression { $$ = create_assignment_stmt($1, create_expression(EXPR_BINARY_OP, create_binary_op(">>=", create_expression(EXPR_NAME, create_name($1)), $3))); }
    ;

return_stmt
    : RETURN expression { $$ = create_flow_stmt("return", $2); }
    | RETURN { $$ = create_flow_stmt("return", NULL); }
    ;

compound_stmt
    : if_stmt { $$ = create_statement(IF_STMT, $1); }
    | while_stmt { $$ = create_statement(WHILE_STMT, $1); }
    | for_stmt { $$ = create_statement(FOR_STMT, $1); }
    | function_def { $$ = create_statement(FUNCTION_DEF, $1); }
    ;

if_stmt
    : IF expression COLON block ELSE COLON block { $$ = create_if_stmt($2, $4, $7); }
    | IF expression COLON block { $$ = create_if_stmt($2, $4, NULL); }
    ;

while_stmt
    : WHILE expression COLON block { $$ = create_while_stmt($2, $4); }
    ;

for_stmt
    : FOR NAME IN expression COLON block { $$ = create_for_stmt($2, $4, $6); }
    ;

function_def
    : DEF NAME L_PAREN parameters R_PAREN COLON block { $$ = create_function_def($2, $4, $7); }
    ;

parameters
    : parameters COMMA NAME { $$ = create_parameters(create_name($3), $1); }
    | NAME { $$ = create_parameters(create_name($1), NULL); }
    | /* empty */ { $$ = NULL; }
    ;

block
    : INDENT statements DEDENT { $$ = $2; }
    ;

expression
    : disjunction { $$ = create_expression(EXPR_BINARY_OP, $1); }
    ;

disjunction
    : disjunction OR conjunction { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("or", $1, $3)); }
    | conjunction { $$ = $1; }
    ;

conjunction
    : conjunction AND inversion { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("and", $1, $3)); }
    | inversion { $$ = $1; }
    ;

inversion
    : NOT inversion { $$ = create_expression(EXPR_UNARY_OP, create_unary_op("not", $2)); }
    | comparison { $$ = $1; }
    ;

comparison
    : bitwise_or EQ bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("==", $1, $3)); }
    | bitwise_or NE bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("!=", $1, $3)); }
    | bitwise_or GT bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op(">", $1, $3)); }
    | bitwise_or GE bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op(">=", $1, $3)); }
    | bitwise_or LT bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("<", $1, $3)); }
    | bitwise_or LE bitwise_or { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("<=", $1, $3)); }
    | bitwise_or { $$ = $1; }
    ;

bitwise_or
    : bitwise_or BITWISE_OR bitwise_xor { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("|", $1, $3)); }
    | bitwise_xor { $$ = $1; }
    ;

bitwise_xor
    : bitwise_xor BITWISE_XOR bitwise_and { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("^", $1, $3)); }
    | bitwise_and { $$ = $1; }
    ;

bitwise_and
    : bitwise_and BITWISE_AND shift_expr { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("&", $1, $3)); }
    | shift_expr { $$ = $1; }
    ;

shift_expr
    : shift_expr LEFT_SHIFT sum { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("<<", $1, $3)); }
    | shift_expr RIGHT_SHIFT sum { $$ = create_expression(EXPR_BINARY_OP, create_binary_op(">>", $1, $3)); }
    | sum { $$ = $1; }
    ;

sum
    : sum ADD term { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("+", $1, $3)); }
    | sum SUB term { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("-", $1, $3)); }
    | term { $$ = $1; }
    ;

term
    : term MUL factor { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("*", $1, $3)); }
    | term DIV factor { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("/", $1, $3)); }
    | term MOD factor { $$ = create_expression(EXPR_BINARY_OP, create_binary_op("%", $1, $3)); }
    | factor { $$ = $1; }
    ;

factor
    : ADD factor { $$ = create_expression(EXPR_UNARY_OP, create_unary_op("+", $2)); }
    | SUB factor { $$ = create_expression(EXPR_UNARY_OP, create_unary_op("-", $2)); }
    | NEG factor { $$ = create_expression(EXPR_UNARY_OP, create_unary_op("~", $2)); }
    | function_call { $$ = create_expression(EXPR_FUNCTION_CALL, $1); }
    | atom { $$ = $1; }
    ;

function_call
    : NAME L_PAREN arguments R_PAREN { $$ = create_expression(EXPR_FUNCTION_CALL, create_function_call($1, $3)); }
    ;

arguments
    : arguments COMMA expression { $$ = create_arguments($3, $1); }
    | expression { $$ = create_arguments($1, NULL); }
    | /* empty */ { $$ = NULL; }
    ;

atom
    : NAME { $$ = create_expression(EXPR_NAME, create_name($1)); }
    | DEC_INTEGER { $$ = create_expression(EXPR_INT, create_int($1)); }
    | HEX_INTEGER { $$ = create_expression(EXPR_INT, create_int($1)); }
    | L_PAREN expression R_PAREN { $$ = $2; }
    ;