#ifndef PLAYLIST_H
#define PLAYLIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Node {
    char *path;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *current;
} Playlist;

Playlist *playlist_create(void);
void playlist_destroy(Playlist *pl);

void playlist_add(Playlist *pl, const char *path);
void playlist_next(Playlist *pl);
void playlist_prev(Playlist *pl);

const char *playlist_current(Playlist *pl);
void playlist_print(Playlist *pl);

#ifdef __cplusplus
}
#endif

#endif