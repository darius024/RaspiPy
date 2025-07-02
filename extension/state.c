#include <assert.h>
#include <stdlib.h>
#include "hotspot.h"
#include "state.h"

State *create_state(void)
{
    State *state = malloc(sizeof(State));
    assert(state != NULL);

    state->map_size = 0;
    state->funcs_size = 0;
    state->stack_size = rand() % (MEMORY_SIZE - STACK_OFFSET) + STACK_OFFSET;
    state->hotspots_size = 0;

    return state;
}

void free_state(State *state)
{
    free(state);
}
