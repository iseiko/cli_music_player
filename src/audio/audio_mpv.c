#include "audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

static pid_t g_mpv_pid = -1;

static void play_impl(const char *path) {
    if (!path || path[0] == '\0') {
        fprintf(stderr, "[AUDIO] Invalid path\n");
        return;
    }

    /* Stop previous playback if any */
    if (g_mpv_pid > 0) {
        kill(g_mpv_pid, SIGTERM);
        g_mpv_pid = -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("[AUDIO] fork");
        return;
    }

    if (pid == 0) {
        /* child: replace with mpv */
        execlp("mpv", "mpv",
               "--no-video",
               "--quiet",
               "--force-window=no",
               path,
               (char *)NULL);

        /* if execlp fails */
        perror("[AUDIO] execlp mpv");
        _exit(127);
    }

    /* parent */
    g_mpv_pid = pid;
    printf("[AUDIO] mpv started (pid=%d)\n", (int)g_mpv_pid);
}

static void stop_impl(void) {
    if (g_mpv_pid > 0) {
        kill(g_mpv_pid, SIGTERM);
        g_mpv_pid = -1;
        printf("[AUDIO] Stopped\n");
    } else {
        printf("[AUDIO] Nothing playing\n");
    }
}

Audio create_dummy_audio(void) {
    /* Keep API name for now so you don’t have to change other code yet */
    Audio a;
    a.play = play_impl;
    a.stop = stop_impl;
    return a;
}