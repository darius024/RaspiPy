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
        printf("Token(reg=%u, type=%s)", token->reg, token_type_to_string(token->type));
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
        printf("IRInstruction(type=%s, dest=", ir_type_to_string(instr->type));
        print_token(instr->dest);
        printf(", src1=");
        print_token(instr->src1);
        printf(", src2=");
        print_token(instr->src2);
        printf(", src3=");
        print_token(instr->src3);
        printf(", line=%d, count=%d)\n", instr->line, instr->count);
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
