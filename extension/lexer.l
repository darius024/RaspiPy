/*
 * RaspiPy - A python like language for easier use with Raspberry Pis
 * Lexer Rules
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "ast.h"

// Stack to hold indentation levels
#define MAX_INDENT 100
int indent_stack[MAX_INDENT];
int indent_top = 0;

// Current indentation level
int current_indent = 0;

void push_indent(int indent) {
    if (indent_top < MAX_INDENT) {
        indent_stack[indent_top++] = indent;
    } else {
        fprintf(stderr, "Indentation stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

int pop_indent() {
    if (indent_top > 0) {
        return indent_stack[--indent_top];
    } else {
        fprintf(stderr, "Indentation stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

int get_top_indent() {
    if (indent_top > 0) {
        return indent_stack[indent_top - 1];
    } else {
        return 0;
    }
}
%}

%option noinput nounput yylineno


%%
"and"          { return AND; }
"break"        { return BREAK; }
"continue"     { return CONTINUE; }
"def"          { return DEF; }
"elif"         { return ELIF; }
"else"         { return ELSE; }
"for"          { return FOR; }
"global"       { return GLOBAL; }
"if"           { return IF; }
"in"           { return IN; }
"not"          { return NOT; }
"or"           { return OR; }
"return"       { return RETURN; }
"while"        { return WHILE; }

[0-9]+        { yylval.num = strtoll(yytext, NULL, 10); return DEC_INTEGER; }
"0"[xX][0-9a-fA-F]+ { yylval.num = strtoll(yytext, NULL, 16); return HEX_INTEGER; }
[a-zA-Z_][a-zA-Z_0-9]*  { yylval.str = strdup(yytext); return NAME; }

"("           { return L_PAREN; }
")"           { return R_PAREN; }
"{"           { return L_BRACE; }
"}"           { return R_BRACE; }
","           { return COMMA; }
":"           { return COLON; }
";"           { return SEMI_COLON; }
"="           { return ASSIGN; }
"|"           { return BITWISE_OR; }
"^"           { return BITWISE_XOR; }
"&"           { return BITWISE_AND; }
"<<"          { return LEFT_SHIFT; }
">>"          { return RIGHT_SHIFT; }
"+"           { return ADD; }
"-"           { return SUB; }
"*"           { return MUL; }
"/"           { return DIV; }
"%"           { return MOD; }
"~"           { return NEG; }
"<"           { return LT; }
">"           { return GT; }
"=="          { return EQ; }
">="          { return GE; }
"<="          { return LE; }
"!="          { return NE; }
"+="          { return ADD_ASSIGN; }
"-="          { return SUB_ASSIGN; }
"*="          { return MUL_ASSIGN; }
"/="          { return DIV_ASSIGN; }
"%="          { return MOD_ASSIGN; }
"&="          { return AND_ASSIGN; }
"|="          { return OR_ASSIGN; }
"^="          { return XOR_ASSIGN; }
"<<="         { return LEFT_SHIFT_ASSIGN; }
">>="         { return RIGHT_SHIFT_ASSIGN; }

"#"[^\r\n]*   { /* Skip comments */ }

[ \r\t\n]+      { /* ignore whitespace */ }

\n {
    int i = 0;
    int new_indent = 0;
    while (yytext[i] == ' ') {
        new_indent++;
        i++;
    }
    if (new_indent > get_top_indent()) {
        push_indent(new_indent);
        return INDENT;
    } else if (new_indent < get_top_indent()) {
        while (new_indent < get_top_indent()) {
            pop_indent();
            return DEDENT;
        }
    }
}

.             { return yytext[0]; } // Default rule to return single characters

%%

