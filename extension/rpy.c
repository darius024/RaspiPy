#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "ir.h"
#include "rpy.h"
#include "utils_ir.h"


int64_t GPIO_FSEL[] = { GPIO_FSEL0, GPIO_FSEL1, GPIO_FSEL2, GPIO_FSEL3, GPIO_FSEL4, GPIO_FSEL5 };
int64_t GPIO_SET[] = { GPIO_SET0, GPIO_SET1 };
int64_t GPIO_CLR[] = { GPIO_CLR0, GPIO_CLR1 };


void fsel_value(IRProgram *program, State *state, int *line, uint8_t gpio)
{
    char str[MAX_NAMES];
    sprintf(str, "gpio_fsel%d", gpio / 10);
    // Add directive
    push_directive(state, GPIO_FSEL[gpio / 10], str);
    // Set immediate value
    int imm = SET_VALUE << (BITS_FSEL * gpio);

    // Load directive address
    uint8_t address_reg = get_free_register();
    sprintf(str, "GPIO_%d", gpio);
    add_name(state, address_reg, str);
    IRInstruction *load = create_ir_instruction(IR_LDR, address_reg, get_directive_address(state), NOT_USED, NOT_USED, line);
    load->dest->type = REGX;
    load->src1->type = DIR;
    insert_instruction(program, load, 1);
    update_state(state, address_reg, state->directives[get_directive_address(state)]->value);

    // Move value
    uint8_t move_reg = get_free_register();
    IRInstruction *movz = create_ir_instruction(IR_MOVZ, move_reg, imm, NOT_USED, NOT_USED, line);
    movz->dest->type = REGX;
    movz->src1->type = IMM;
    insert_instruction(program, movz, 1);
    update_state(state, move_reg, imm);

    // Store that value 
    IRInstruction *store = create_ir_instruction(IR_STR, move_reg, address_reg, NOT_USED, NOT_USED, line);
    store->dest->type = REGX;
    store->src1->type = REGX;
    insert_instruction(program, store, 1);

    free_register(state, move_reg);
}

void set_value(IRProgram *program, State *state, int *line, uint8_t gpio)
{
    char str[MAX_NAMES];
    sprintf(str, "gpio_set%d", gpio / 32);
    // Add directive
    push_directive(state, GPIO_SET[gpio / 32], str);
    // Set immediate value
    int imm = SET_VALUE << (BITS_SET_CLR * gpio);

    // Load directive address
    uint8_t address_reg = get_free_register();
    IRInstruction *load = create_ir_instruction(IR_LDR, address_reg, get_directive_address(state), NOT_USED, NOT_USED, line);
    load->dest->type = REGX;
    load->src1->type = DIR;
    insert_instruction(program, load, 1);
    update_state(state, address_reg, state->directives[get_directive_address(state)]->value);

    // Move value
    uint8_t move_reg = get_free_register();
    IRInstruction *movz = create_ir_instruction(IR_MOVZ, move_reg, imm, NOT_USED, NOT_USED, line);
    movz->dest->type = REGX;
    movz->src1->type = IMM;
    insert_instruction(program, movz, 1);
    update_state(state, move_reg, imm);

    // Store that value 
    IRInstruction *store = create_ir_instruction(IR_STR, move_reg, address_reg, NOT_USED, NOT_USED, line);
    store->dest->type = REGX;
    store->src1->type = REGX;
    insert_instruction(program, store, 1);

    free_register(state, move_reg);
}

void clear_value(IRProgram *program, State *state, int *line, uint8_t gpio)
{
    char str[MAX_NAMES];
    sprintf(str, "gpio_clear%d", gpio / 32);
    // Add directive
    push_directive(state, GPIO_CLR[gpio / 32], str);
    // Set immediate value
    int imm = SET_VALUE << (BITS_SET_CLR * gpio);

    // Load directive address
    uint8_t address_reg = get_free_register();
    IRInstruction *load = create_ir_instruction(IR_LDR, address_reg, get_directive_address(state), NOT_USED, NOT_USED, line);
    load->dest->type = REGX;
    load->src1->type = DIR;
    insert_instruction(program, load, 1);
    update_state(state, address_reg, state->directives[get_directive_address(state)]->value);

    // Move value
    uint8_t move_reg = get_free_register();
    IRInstruction *movz = create_ir_instruction(IR_MOVZ, move_reg, imm, NOT_USED, NOT_USED, line);
    movz->dest->type = REGX;
    movz->src1->type = IMM;
    insert_instruction(program, movz, 1);
    update_state(state, move_reg, imm);

    // Store that value 
    IRInstruction *store = create_ir_instruction(IR_STR, move_reg, address_reg, NOT_USED, NOT_USED, line);
    store->dest->type = REGX;
    store->src1->type = REGX;
    insert_instruction(program, store, 1);

    free_register(state, move_reg);
}

void wait(IRProgram *program, State *state, int *line, int wait_time)
{
    char str[MAX_NAMES];
    
    // Store the wait time as directive
    sprintf(str, "wait_time%d", *line / 4);
    push_directive(state, wait_time * WAIT_SECOND, str);

    // Load the wait address in a free register
    uint8_t reg = get_free_register();
    IRInstruction *wait_load = create_ir_instruction(IR_LDR, reg, get_directive_address(state), NOT_USED, NOT_USED, line);
    wait_load->dest->type = REGX;
    wait_load->src1->type = DIR;
    insert_instruction(program, wait_load, 1);
    update_state(state, reg, wait_time * WAIT_SECOND);

    // Add label
    sprintf(str, "wait%d", *line / 4);
    add_label(state, *line, str);
    IRInstruction *subs = create_ir_instruction(IR_SUBS, reg, reg, 1, NOT_USED, line);
    subs->dest->type = get_register_type(reg);
    subs->src1->type = get_register_type(reg);
    insert_instruction(program, subs, wait_time * WAIT_SECOND);

    // Branch to the beginning if the Zero flag is not set
    IRInstruction *branch = create_ir_instruction(IR_BCOND, B_NE, get_label_address(state), NOT_USED, NOT_USED, line);
    branch->dest->type = BC;
    branch->src1->type = LABEL;
    insert_instruction(program, branch, wait_time * WAIT_SECOND);

    free_register(state, reg);
}

void loop_end(IRProgram *program, State *state, int *line, int label_address)
{
    IRInstruction *branch_loop = create_ir_instruction(IR_B, label_address, NOT_USED, NOT_USED, NOT_USED, line);
    branch_loop->dest->type = LABEL;
    insert_instruction(program, branch_loop, 1);
}


bool check_gpio_assignment(IRProgram *program, AssignmentStmt *assign_stmt, State *state, int *line)
{
    if (!strncmp(assign_stmt->name, "GPIO_", 5)) {
        uint8_t gpio = atoi(assign_stmt->name + 5);
        if (assign_stmt->expression->tag == EXPR_INT) {
            bool found = false;
            for (int i = 0; i < state->map_size; i++) {
                if (!strcmp(state->map[i]->name, assign_stmt->name)) {
                    found = true;
                }
            }
            if (!found) {
                fsel_value(program, state, line, gpio);
            }
            if (assign_stmt->expression->int_value->value == 1) {
                set_value(program, state, line, gpio);
            } else {
                clear_value(program, state, line, gpio);
            }
            return true;
        }
    }
    return false;
}

bool check_wait(IRProgram *program, AssignmentStmt *assign_stmt, State *state, int *line)
{
    if (assign_stmt->expression->tag == EXPR_FUNCTION_CALL) {
        if (!strcmp(assign_stmt->expression->function_call->name, "WAIT")) {
            wait(program, state, line, assign_stmt->expression->function_call->args->arg->int_value->value);
            return true;
        }
    }
    return false;
}

bool check_loop_return(IRProgram *program, FlowStmt *flow_stmt, State *state, int *line)
{
    if (flow_stmt->expression->tag == EXPR_FUNCTION_CALL) {
        if (!strcmp(flow_stmt->expression->function_call->name, "LOOP")) {
            loop_end(program, state, line, get_fun_address(state, "LOOP"));
            return true;
        }
    }
    return false;
}

bool check_loop_call(IRProgram *program, FunctionCall *func_call, State *state, int *line)
{
    if (!strcmp(func_call->name, "LOOP")) {
        int branch_line = get_fun_address(state, func_call->name);

        IRInstruction *call_instr = create_ir_instruction(IR_B, branch_line, NOT_USED, NOT_USED, NOT_USED, line);
        call_instr->dest->type = LABEL;
        insert_instruction(program, call_instr, 1);
        return true;
    }
    return false;
}
