#include "cli.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char *xstrdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    if (!p) return NULL;
    memcpy(p, s, len);
    return p;
}

static void print_help(void) {
    printf("Commands:\n"
           "  help                 Show this help\n"
           "  add <path>            Add a song path to playlist\n"
           "  add \"<path with spaces>\"\n"
           "  play                 Play current song\n"
           "  next                 Next song (plays if already playing)\n"
           "  prev                 Previous song (plays if already playing)\n"
           "  stop                 Stop playback\n"
           "  list                 Print playlist\n"
           "  quit                 Exit\n"
           "\n"
           "Notes:\n"
           "  - ~ is expanded to your $HOME when adding paths.\n");
}

static void rstrip(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1]))) {
        s[n - 1] = '\0';
        n--;
    }
}

static char *lstrip(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

static void unquote(char *s) {
    size_t len = strlen(s);
    if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"') || (s[0] == '\'' && s[len - 1] == '\''))) {
        s[len - 1] = '\0';
        memmove(s, s + 1, len - 1);
    }
}

/* Expands:
   - "~/..." -> "$HOME/..."
   - "~"     -> "$HOME"
   Returns: newly allocated string (caller must free), or NULL on error. */
static char *expand_tilde(const char *path) {
    if (!path) return NULL;

    if (path[0] != '~') {
        return xstrdup(path);
    }

    const char *home = getenv("HOME");
    if (!home || home[0] == '\0') return NULL;

    /* Only support "~" or "~/..." (not ~user) */
    if (path[1] != '\0' && path[1] != '/') {
        return xstrdup(path);
    }

    size_t home_len = strlen(home);
    size_t rest_len = strlen(path + 1); /* includes '/' or '\0' */
    size_t out_len = home_len + rest_len + 1;

    char *out = (char *)malloc(out_len);
    if (!out) return NULL;

    memcpy(out, home, home_len);
    memcpy(out + home_len, path + 1, rest_len + 1);
    return out;
}

void run_cli(Player *player, Playlist *pl) {
    char input[512];

    print_help();

    while (1) {
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            putchar('\n');
            break;
        }

        rstrip(input);
        char *cmd = lstrip(input);

        if (strcmp(cmd, "help") == 0) {
            print_help();
        } else if (strncmp(cmd, "add", 3) == 0 && (cmd[3] == '\0' || isspace((unsigned char)cmd[3]))) {
            char *arg = cmd + 3;
            arg = lstrip(arg);
            if (*arg == '\0') {
                printf("Usage: add <path>\n");
                continue;
            }

            unquote(arg);

            char *expanded = expand_tilde(arg);
            if (!expanded) {
                printf("Failed to expand path\n");
                continue;
            }

            playlist_add(pl, expanded);
            free(expanded);
        } else if (strcmp(cmd, "play") == 0) {
            player_play(player);
        } else if (strcmp(cmd, "next") == 0) {
            player_next(player);
        } else if (strcmp(cmd, "prev") == 0) {
            player_prev(player);
        } else if (strcmp(cmd, "stop") == 0) {
            player_stop(player);
        } else if (strcmp(cmd, "list") == 0) {
            playlist_print(pl);
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        } else if (*cmd == '\0') {
            continue;
        } else {
            printf("Unknown command: '%s' (type 'help')\n", cmd);
        }
    }
}