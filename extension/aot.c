#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "ir.h"
#include "utils_ir.h"
#include "ast.h"
#include "ast_to_ir.h"
#include "../src/io.h"
#include "parser.h"
#include "lexer.h"


extern int yyparse(void);
extern int yylex_destroy (void);


extern Program *program;
extern int yyerrors;

const char r = 'r';

static FILE *loadInputFile(const char *filename, const char *extension, const char *readMode)
{
    if (extension != NULL) { // Check if a specific extension is required
        const char *fileType = strrchr(filename, '.');
        if (!fileType || strcmp(fileType + 1, extension) != 0) {
            fprintf(stderr, "This is not a valid .%s file: %s\n", extension, filename);
            exit(EXIT_FAILURE);
        }
    }

    FILE *file;
    file = fopen(filename, readMode);
    if (!file) {
        perror("Could not open input file.");
        exit(EXIT_FAILURE);
    }
    return file;
}

static FILE *openOutputFile(const char *filename, const char *extension, const char *writeMode)
{
    
    FILE *file;
    file = (!strcmp(filename, stdout)) ? stdout : fopen(filename, writeMode);
    if (!file) {
        perror("Could not open output file.\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Error Checking
// static void checkError(bool error)
// {
//     if (error) {
//         exit(EXIT_FAILURE);
//     }
// }

static void checkErrorOutput(FILE *file)
{
    if (ferror(file)) {
        perror("Error ocurred writing to the output.\n");
        exit(EXIT_FAILURE);
    }
}

// File Close
static void closeFiles(FILE *input, FILE *output)
{
    fclose(input);
    checkErrorOutput(output);
    fclose(output);
}



void generate_assembly(IRInstruction *ir_instr, FILE *outputFile)
{
    char assembly_line[MAX_ASSEMBLY_LINE] = "";
    putMnemonic(assembly_line, ir_instr->type);
    putRegOrImm(assembly_line, ir_instr->dest);
    putRegOrImm(assembly_line, ir_instr->src1);
    putRegOrImm(assembly_line, ir_instr->src2);
    putRegOrImm(assembly_line, ir_instr->src3);

    fprintf(outputFile,"%s\n",assembly_line);
}



uint32_t generateInstruction(State *state, IRInstruction *instruction) 
{
    // Check for hotspot - C to assembly to binary
    if (instruction->count >= THRESHOLD) {
        return returnHotSpot(state, instruction->line);
    }
    // No hotspot - C to binary
    uint32_t out = 0;
    // switch (instruction->type) {
    //     case IR_ADD:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){ADD}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_SUB:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){SUB}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_CMP:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){SUB_SETFLAGS}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_NEG:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){SUB}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){1}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_MUL:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){1}, DPR_M_OFFSET, DPR_M_LEN);
    //         putBits(&out, &(int){DPR_MUL}, DPR_OPR_OFFSET, DPR_OPR_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_AND:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){BITWISE_AND}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     // case IR_BIC:
    //     //     putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //     //     putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //     //     putBits(&out, &(int){BITWISE_AND}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //     //     putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //     //     putBits(&out, &(int){1}, DPR_N_OFFSET, DPR_N_LEN);
    //     //     putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //     //     putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //     //     putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //     //     break;
    //     case IR_ORR:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){BITWISE_OR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_EOR:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){BITWISE_XOR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_TST:
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){BITWISE_AND_SETFLAGS}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src2->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RN_OFFSET, DPR_RN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     // case IR_WIDEMOVE:
    //     //     putBits(&out, &(int){OP0_DPI}, OP0_OFFSET, OP0_LEN);
    //     //     putBits(&out, &(int){1}, DPI_SF_OFFSET, DPI_SF_LEN);
    //     //     putBits(&out, &(int){MOVE_WITH_ZERO}, DPI_OPC_OFFSET, DPI_OPC_LEN);
    //     //     putBits(&out, &(int){WIDEMOVE}, DPI_OPI_OFFSET, DPI_OPI_LEN);
    //     //     putBits(&out, &(int){instruction->src1->reg}, DPI_IMM16_OFFSET, DPI_IMM16_LEN);
    //     //     putBits(&out, &(int){instruction->dest->reg}, DPI_RD_OFFSET, DPI_RD_LEN);
    //     //     break;
    //     case IR_MOV: // same as IR_OR
    //         putBits(&out, &(int){OP0_DPR}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, DPR_SF_OFFSET, DPR_SF_LEN);
    //         putBits(&out, &(int){BITWISE_OR}, DPR_OPC_OFFSET, DPR_OPC_LEN);
    //         putBits(&out, &(int){0}, DPR_ARMORLOG_OFFSET, DPR_ARMORLOG_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, DPR_RM_OFFSET, DPR_RM_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, DPR_RD_OFFSET, DPR_RD_LEN);
    //         break;
    //     case IR_B:
    //         putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){BRANCH_UNCONDITIONAL}, B_TYPE_OFFSET, B_TYPE_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, B_SIMM26_OFFSET, B_SIMM26_LEN);
    //         break;
    //     // case IR_BCOND:
    //     //     putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
    //     //     putBits(&out, &(int){BRANCH_CONDITIONAL}, B_TYPE_OFFSET, B_TYPE_LEN);
    //     //     putBits(&out, &(int){instruction->dest->reg}, B_SIMM19_OFFSET, B_SIMM19_LEN);
    //     //     putBits(&out, &(int){instruction->src1->reg}, B_TAG_OFFSET, B_TAG_LEN);
    //     //     break;
    //     case IR_BR:
    //         putBits(&out, &(int){OP0_B}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){BRANCH_REGISTER}, B_TYPE_OFFSET, B_TYPE_LEN);
    //         putBits(&out, &(int){B_BIT}, B_BIT_OFFSET, B_BIT_LEN);
    //         putBits(&out, &(int){B_REG}, B_REG_OFFSET, B_REG_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, B_XN_OFFSET, B_XN_LEN);
    //         break;
    //     case IR_LDR:
    //         putBits(&out, &(int){OP0_SDT}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, SDT_MODE_OFFSET, SDT_MODE_LEN);
    //         putBits(&out, &(int){1}, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
    //         putBits(&out, &(int){1}, SDT_L_OFFSET, SDT_L_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, SDT_XN_OFFSET, SDT_XN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, SDT_RT_OFFSET, SDT_RT_LEN);
    //         break;
    //     case IR_STR:
    //         putBits(&out, &(int){OP0_SDT}, OP0_OFFSET, OP0_LEN);
    //         putBits(&out, &(int){1}, SDT_MODE_OFFSET, SDT_MODE_LEN);
    //         putBits(&out, &(int){1}, SDT_MODE_OFFSET2, SDT_MODE_LEN2);
    //         putBits(&out, &(int){0}, SDT_L_OFFSET, SDT_L_LEN);
    //         putBits(&out, &(int){instruction->src1->reg}, SDT_XN_OFFSET, SDT_XN_LEN);
    //         putBits(&out, &(int){instruction->dest->reg}, SDT_RT_OFFSET, SDT_RT_LEN);
    //         break;
    //     default:
    //         break;
    // }
    return out;
}


int main(int argc, char const *argv[])
{
    /* code */
    char *inputFile;
    char *outputFile;
    if (argc >= 2) {
        inputFile = argv[1];
        outputFile = (argc > 2) ? argv[2]: stdout ;
    } else {
        EXIT_PROGRAM("Provide at least an input file.");
    }
    const char r = "r";

    FILE *parsing_file = loadInputFile(inputFile, NULL, "r");

    yyin = parsing_file;

    if (yyparse() == 0) {
        // Parsing succeeded, process the AST (variable 'program') as needed
        printf("Parsing succeeded\n");
        // AST processing code here
    } else {
        fprintf(stderr, "Parsing failed\n");
    }

    //Progam = AST
    //AST to IR call in Progran 
    IRProgram *ir_program = AST_to_IR(program);

    //free stuff from the AST
    free_program(program);

    //open output file to
    // Write the assembl instructions
    FILE *output = openOutputFile(outputFile, NULL, "w");
    // writeBinaryInstr(output);

    //iterate through the program and use the aot
    IRInstruction *cur_instr = ir_program->head;

    while(cur_instr != NULL) {
        generate_assembly(cur_instr, output);
        cur_instr = cur_instr->next;      
    
    }
    //free IRProgram
    free_ir_program(ir_program);

    //close files
    closeFiles(parsing_file, output);
    return 0;

}

