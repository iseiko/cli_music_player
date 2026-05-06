#ifndef CLI_H
#define CLI_H

#include "player/player.h"
#include "playlist/playlist.h"

#ifdef __cplusplus
extern "C" {
#endif

void run_cli(Player *player, Playlist *pl);

#ifdef __cplusplus
}
#endif

#endif