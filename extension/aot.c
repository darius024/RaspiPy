#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ast.h"
#include "ast_to_ir.h"
#include "ir.h"
#include "lexer.h"
#include "parser.h"
#include "print.h"
#include "utils_ir.h"
#include "../src/io.h"

#define MAX_ASSEMBLY_LINE 32

extern int yyparse(void);
extern int yylex_destroy (void);

extern Program *program;
extern int yyerrors;

int main(int argc, char **argv)
{
    char *inputFile;
    char *outputFile;
    if (argc >= 2) {
        inputFile = argv[1];
        outputFile = (argc > 2) ? argv[2] : "stdout" ;
    } else {
        perror("Provide at least an input file.");
        exit(EXIT_FAILURE);
    }

    FILE *input = loadInputFile(inputFile, NULL, "r");

    yyin = input;

    if (yyparse() == 0) {
        printf("Parsing succeeded\n");
        // AST processing code here
    } else {
        fprintf(stderr, "Parsing failed\n");
        exit(EXIT_FAILURE);
    }

    // Progam => AST
    // Found in the extern program variable
    printf("\nParsed program:\n");
    print_program(program);

    // AST => IR
    // Printed as assembly directly
    State *state = create_state();
    IRProgram *ir_program = ast_to_ir(program, state);

    printf("\n\nAssembled program:\n");
    FILE *output = openOutputFile(outputFile, NULL, "w");
    print_ir_program(ir_program, state, output);

    // Free programs
    free_program(program);
    free_ir_program(ir_program);
    free_state(state);

    // Close files
    closeFiles(input, output);

    return EXIT_SUCCESS;
}

