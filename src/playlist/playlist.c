#include "playlist.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *xstrdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    if (!p) return NULL;
    memcpy(p, s, len);
    return p;
}

static Node *create_node(const char *path) {
    if (!path) return NULL;

    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) return NULL;

    n->path = xstrdup(path);
    if (!n->path) {
        free(n);
        return NULL;
    }

    n->next = n;
    n->prev = n;
    return n;
}

Playlist *playlist_create(void) {
    Playlist *pl = (Playlist *)malloc(sizeof(Playlist));
    if (!pl) return NULL;
    pl->current = NULL;
    return pl;
}

void playlist_add(Playlist *pl, const char *path) {
    if (!pl || !path) return;

    Node *n = create_node(path);
    if (!n) {
        fprintf(stderr, "Failed to add '%s' (out of memory)\n", path);
        return;
    }

    if (!pl->current) {
        pl->current = n;
        return;
    }

    Node *curr = pl->current;

    /* Insert after current */
    n->next = curr->next;
    n->prev = curr;

    curr->next->prev = n;
    curr->next = n;
}

void playlist_next(Playlist *pl) {
    if (pl && pl->current) pl->current = pl->current->next;
}

void playlist_prev(Playlist *pl) {
    if (pl && pl->current) pl->current = pl->current->prev;
}

const char *playlist_current(Playlist *pl) {
    if (!pl || !pl->current) return NULL;
    return pl->current->path;
}

void playlist_print(Playlist *pl) {
    if (!pl || !pl->current) {
        printf("Playlist empty\n");
        return;
    }

    Node *start = pl->current;
    Node *n = start;

    do {
        if (n == pl->current) printf("-> %s\n", n->path);
        else                 printf("   %s\n", n->path);

        n = n->next;
    } while (n != start);
}

void playlist_destroy(Playlist *pl) {
    if (!pl) return;

    if (pl->current) {
        Node *start = pl->current;
        Node *n = start->next;

        while (n != start) {
            Node *tmp = n;
            n = n->next;
            free(tmp->path);
            free(tmp);
        }

        free(start->path);
        free(start);
    }

    free(pl);
}