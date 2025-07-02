lexer grammar RaspiPyLexer;

tokens {
    INDENT,
    DEDENT
}

options {
    superClass = RaspiPyLexerBase;
}

/*
 * Lexer Rules
 */

NUMBER: INTEGER;

INTEGER: DECIMAL_INTEGER | HEX_INTEGER;

AND        : 'and';
BREAK      : 'break';
// CASE       : 'case';
CONTINUE   : 'continue';
DEF        : 'def';
ELIF       : 'elif';
ELSE       : 'else';
FALSE      : 'False';
FOR        : 'for';
GLOBAL     : 'global';
IF         : 'if';
IN         : 'in';
// MATCH      : 'match';
NOT        : 'not';
OR         : 'or';
RETURN     : 'return';
TRUE       : 'True';
WHILE      : 'while';

NEWLINE: ({this.atStartOfInput()}? SPACES | ( '\r'? '\n' | '\r' | '\f') SPACES?) {this.onNewLine();};

NAME: ID_START ID_CONTINUE*;

DECIMAL_INTEGER: NON_ZERO_DIGIT DIGIT* | '0'+;

HEX_INTEGER: '0' [xX] HEX_DIGIT+;


STAR               : '*';
OPEN_PAREN         : '(' {this.openBrace();};
CLOSE_PAREN        : ')' {this.closeBrace();};
COMMA              : ',';
COLON              : ':';
SEMI_COLON         : ';';
// POWER              : '**';
ASSIGN             : '=';
// OPEN_BRACK         : '[' {this.openBrace();};
// CLOSE_BRACK        : ']' {this.closeBrace();};
OR_OP              : '|';
XOR                : '^';
AND_OP             : '&';
LEFT_SHIFT         : '<<';
RIGHT_SHIFT        : '>>';
ADD                : '+';
MINUS              : '-';
DIV                : '/';
MOD                : '%';
NOT_OP             : '~';
// OPEN_BRACE         : '{' {this.openBrace();};
// CLOSE_BRACE        : '}' {this.closeBrace();};
LESS_THAN          : '<';
GREATER_THAN       : '>';
EQUALS             : '==';
GT_EQ              : '>=';
LT_EQ              : '<=';
NOT_EQ             : '!=';
ADD_ASSIGN         : '+=';
SUB_ASSIGN         : '-=';
MULT_ASSIGN        : '*=';
DIV_ASSIGN         : '/=';
MOD_ASSIGN         : '%=';
AND_ASSIGN         : '&=';
OR_ASSIGN          : '|=';
XOR_ASSIGN         : '^=';
LEFT_SHIFT_ASSIGN  : '<<=';
RIGHT_SHIFT_ASSIGN : '>>=';
// POWER_ASSIGN       : '**=';

SKIP_: ( SPACES | COMMENT | LINE_JOINING ) -> skip;

UNKNOWN_CHAR: .;

/*
 * Fragments
 */

fragment NON_ZERO_DIGIT: [1-9];

fragment DIGIT: [0-9];

fragment HEX_DIGIT: [0-9a-fA-F];

fragment INT_PART: DIGIT+;

fragment SPACES: [ \t]+;

fragment COMMENT: '#' ~[\r\n\f]*;

fragment LINE_JOINING: '\\' SPACES? ( '\r'? '\n' | '\r' | '\f');

fragment ID_START:
    '_'
    | [\p{L}]
    | [\p{Nl}]
;

fragment ID_CONTINUE:
    ID_START
    | [\p{Mn}]
    | [\p{Mc}]
    | [\p{Nd}]
    | [\p{Pc}]
;
