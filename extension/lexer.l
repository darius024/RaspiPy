/*
 * RaspiPy - A python like language for easier use with Raspberry Pis
 * Lexer Rules
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "y.tab.h"
%}

%option yylineno

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

[0-9]+        { yylval.num = atoi(yytext); return DEC_INTEGER; }
"0"[xX][0-9a-fA-F]+ { yylval.num = strtol(yytext, NULL, 16); return HEX_INTEGER; }
[a-zA-Z_][a-zA-Z_0-9]*  { yylval.str = strdup(yytext); return NAME; }

"("           { return L_PAREN; }
")"           { return R_PAREN; }
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
"-"           { return MINUS; }
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

[ \r\t\n]+    { /* ignore whitespace */; }

%%
