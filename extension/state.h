#include "hotspot.h"
#include <stdint.h>
#include <stdlib.h>

#define MAX_NAMES 16
#define MAX_VARS 64
#define MAX_FUNCS 16
#define NOT_USED -1
#define MAX_MOVE_VALUE (1 << 16)
#define MEMORY_SIZE (1024 * 1024 * 2)
#define STACK_OFFSET 2048
#define MAX_STACK 1024

typedef struct {
    char name[MAX_NAMES];
    int64_t value;
    uint8_t reg;
} Entry;

typedef struct {
    char name[MAX_NAMES];
    int line;
} Func;

typedef struct {
    int map_size;
    Entry map[MAX_VARS];
    int funcs_size;
    Func funcs[MAX_FUNCS];
    int stack_size;
    int64_t stack[MAX_STACK];
    int hotspots_size;
    HotMap hotspots[MAX_HOTSPOTS];
} State;


extern State *create_state(void);
extern void free_state(State *state);
