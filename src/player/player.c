#include "player.h"

#include <stdlib.h>
#include <stdio.h>

Player *player_create(Playlist *pl, State *st, Audio audio) {
    if (!pl || !st || !audio.play || !audio.stop) return NULL;

    Player *p = (Player *)malloc(sizeof(Player));
    if (!p) return NULL;

    p->playlist = pl;
    p->state = st;
    p->audio = audio;
    return p;
}

void player_destroy(Player *p) {
    free(p);
}

void player_play(Player *p) {
    if (!p) return;

    const char *song = playlist_current(p->playlist);
    if (!song) {
        printf("No song to play\n");
        return;
    }

    p->audio.play(song);
    p->state->state = PLAYING;
}

void player_next(Player *p) {
    if (!p) return;

    playlist_next(p->playlist);

    if (p->state->state == PLAYING) {
        const char *song = playlist_current(p->playlist);
        if (song) p->audio.play(song);
    }
}

void player_prev(Player *p) {
    if (!p) return;

    playlist_prev(p->playlist);

    if (p->state->state == PLAYING) {
        const char *song = playlist_current(p->playlist);
        if (song) p->audio.play(song);
    }
}

void player_stop(Player *p) {
    if (!p) return;

    p->audio.stop();
    p->state->state = STOPPED;
}