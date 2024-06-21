#include "testIR.h"
#include "ast.h"
#include "ir.h"
#include "ast_to_ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case IMM: return "IMM";
        case REG: return "REG";
        case LABEL: return "LABEL";
        // add other cases as necessary
        default: return "UNKNOWN";
    }
}

void print_token(Token *token) {
    if (token) {
        printf("Tok(%u, %s)", token->reg, token_type_to_string(token->type));
    } else {
        printf("NULL");
    }
}

const char* ir_type_to_string(IRType type) {
    switch (type) {
        case IR_ADD: return "ADD";
        case IR_SUB: return "SUB";
        case IR_CMP: return "CMP";
        case IR_NEG: return "NEG";
        case IR_AND: return "AND";
        case IR_EOR: return "EOR";
        case IR_ORR: return "ORR";
        case IR_TST: return "TST";
        case IR_MOV: return "MOV";
        case IR_MOVZ: return "MOVZ";
        case IR_MVN: return "MVN";
        case IR_MADD: return "MADD";
        case IR_MUL: return "MUL";
        case IR_B: return "B";
        case IR_BR: return "BR";
        case IR_BCOND: return "BCOND";
        case IR_LDR: return "LDR";
        case IR_STR: return "STR";
        case IR_DIR: return "DIR";
        default: return "UNKNOWN";
    }
}

void print_ir_instruction(IRInstruction *instr) {
    if (instr) {
        char assembly_line[MAX_ASSEMBLY_LINE] = "";
        putMnemonic(assembly_line, instr->type);
        if (instr->dest != NULL) putRegOrImm(assembly_line, instr->dest);
        if (instr->src1 != NULL) putRegOrImm(assembly_line, instr->src1);
        if (instr->src2 != NULL) putRegOrImm(assembly_line, instr->src2);
        if (instr->src3 != NULL) putRegOrImm(assembly_line, instr->src3);
        printf("%s\n", assembly_line);
    } else {
        printf("NULL\n");
    }
}

void print_ir_program(IRProgram *program) {
    if (program) {
        IRInstruction *current = program->head;
        while (current) {
            print_ir_instruction(current);
            current = current->next;
        }
    } else {
        printf("NULL\n");
    }
}
