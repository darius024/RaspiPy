#include <stdio.h>

#include "ast.h"
#include "parser.h"
#include "lexer.h"
#include "../src/io.h"

// using non-reentrant lexer
extern int yyparse(void);
extern int yylex_destroy (void);


extern Program *program;
extern int yyerrors;

int main(void) {
    const char read = 'r';
    FILE *parsing_file = loadInputFile("testData/arithm1.py", NULL, &read);

    
    yyin = parsing_file;

    if (yyparse() == 0) {
        // Parsing succeeded, process the AST (variable 'program') as needed
        printf("Parsing succeeded\n");
        // AST processing code here
    } else {
        fprintf(stderr, "Parsing failed\n");
    }
    

    return 0;
}
