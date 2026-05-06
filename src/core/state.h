#ifndef STATE_H
#define STATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STOPPED = 0,
    PLAYING = 1
} PlayerState;

typedef struct {
    PlayerState state;
} State;

State *state_create(void);
void state_destroy(State *s);

#ifdef __cplusplus
}
#endif

#endif