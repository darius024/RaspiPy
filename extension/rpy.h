// Library for Raspberry Pi interface
// The state gets appended with directives / instructions
#ifndef RPY_H
#define RPY_H

#include <stdbool.h>
#include "ir.h"
#include "rpy.h"
#include "utils_ir.h"

// Base address for GPIO registers
#define GPIO_BASE 0x3f200000

// Function select registers
#define GPIO_FSEL0 (GPIO_BASE + 0x00)
#define GPIO_FSEL1 (GPIO_BASE + 0x04)
#define GPIO_FSEL2 (GPIO_BASE + 0x08)
#define GPIO_FSEL3 (GPIO_BASE + 0x0c)
#define GPIO_FSEL4 (GPIO_BASE + 0x10)
#define GPIO_FSEL5 (GPIO_BASE + 0x14)

// Set and clear registers
#define GPIO_SET0 (GPIO_BASE + 0x1c)
#define GPIO_SET1 (GPIO_BASE + 0x20)
#define GPIO_CLR0 (GPIO_BASE + 0x28)
#define GPIO_CLR1 (GPIO_BASE + 0x2c)

// Constants
#define SET_VALUE 0x1
#define WAIT_SECOND 0xfffff
#define BITS_FSEL 3
#define BITS_SET_CLR 1


// Function declarations
void fsel_value(IRProgram *program, State *state, int *line, uint8_t gpio);
void set_value(IRProgram *program, State *state, int *line, uint8_t gpio);
void clear_value(IRProgram *program, State *state, int *line, uint8_t gpio);
void wait(IRProgram *program, State *state, int *line, int wait_time);
void loop_end(IRProgram *program, State *state, int *line, int label_address);

bool check_gpio_assignment(IRProgram *program, AssignmentStmt *assign_stmt, State *state, int *line);
bool check_wait(IRProgram *program, AssignmentStmt *assign_stmt, State *state, int *line);
bool check_loop_return(IRProgram *program, FlowStmt *flow_stmt, State *state, int *line);
bool check_loop_call(IRProgram *program, FunctionCall *func_call, State *state, int *line);

#endif
