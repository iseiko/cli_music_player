#ifndef PLAYER_H
#define PLAYER_H

#include "playlist/playlist.h"
#include "core/state.h"
#include "audio/audio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Playlist *playlist;
    State *state;
    Audio audio;
} Player;

Player *player_create(Playlist *pl, State *st, Audio audio);
void player_destroy(Player *p);

void player_play(Player *p);
void player_next(Player *p);
void player_prev(Player *p);
void player_stop(Player *p);

#ifdef __cplusplus
}
#endif

#endif