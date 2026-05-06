#include "state.h"
#include <stdlib.h>

State *state_create(void) {
    State *s = (State *)malloc(sizeof(State));
    if (!s) return NULL;
    s->state = STOPPED;
    return s;
}

void state_destroy(State *s) {
    free(s);
}