#include "playlist/playlist.h"
#include "core/state.h"
#include "player/player.h"
#include "audio/audio.h"
#include "cli/cli.h"

#include <stdio.h>

int main(void) {
    Playlist *pl = playlist_create();
    if (!pl) {
        fprintf(stderr, "Failed to create playlist\n");
        return 1;
    }

    State *st = state_create();
    if (!st) {
        fprintf(stderr, "Failed to create state\n");
        playlist_destroy(pl);
        return 1;
    }

    Audio audio = create_dummy_audio();

    Player *player = player_create(pl, st, audio);
    if (!player) {
        fprintf(stderr, "Failed to create player\n");
        state_destroy(st);
        playlist_destroy(pl);
        return 1;
    }

    run_cli(player, pl);

    player_destroy(player);
    playlist_destroy(pl);
    state_destroy(st);

    return 0;
}