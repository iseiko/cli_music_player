#define _POSIX_C_SOURCE 200809L

#include "audio.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t g_mpv_pid = -1;

static void reap_children_nonblocking(void) {
    int status = 0;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        /* reaped */
    }
}

static void sigchld_handler(int signo) {
    (void)signo;
    reap_children_nonblocking();

    /* If the mpv process exited, clear pid (best effort) */
    if (g_mpv_pid > 0) {
        if (kill(g_mpv_pid, 0) != 0) {
            g_mpv_pid = -1;
        }
    }
}

static void ensure_sigchld_handler_installed(void) {
    static int installed = 0;
    if (installed) return;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) != 0) {
        perror("[AUDIO] sigaction(SIGCHLD)");
    }

    installed = 1;
}

static void stop_impl(void) {
    if (g_mpv_pid <= 0) {
        printf("[AUDIO] Nothing playing\n");
        return;
    }

    /* Try to stop mpv gracefully */
    if (kill(g_mpv_pid, SIGTERM) != 0) {
        perror("[AUDIO] kill(SIGTERM)");
    } else {
        printf("[AUDIO] Stop sent to mpv (pid=%d)\n", (int)g_mpv_pid);
    }

    /* Don’t block; mpv may take time to exit. SIGCHLD will reap it. */
}

static void play_impl(const char *path) {
    ensure_sigchld_handler_installed();

    if (!path || path[0] == '\0') {
        fprintf(stderr, "[AUDIO] play: empty path\n");
        return;
    }

    /* If something is currently playing, stop it first */
    if (g_mpv_pid > 0) {
        (void)kill(g_mpv_pid, SIGTERM);
        g_mpv_pid = -1;
    }

    printf("[AUDIO] Starting mpv for path: '%s'\n", path);
    fflush(stdout);

    pid_t pid = fork();
    if (pid < 0) {
        perror("[AUDIO] fork");
        return;
    }

    if (pid == 0) {
        /* Child process */
        execlp("mpv", "mpv",
               "--no-video",
               "--quiet",
               "--force-window=no",
               "--audio-display=no",
               path,
               (char *)NULL);

        fprintf(stderr, "[AUDIO] execlp(mpv) failed: %s\n", strerror(errno));
        _exit(127);
    }

    /* Parent */
    g_mpv_pid = pid;
    printf("[AUDIO] mpv started (pid=%d)\n", (int)g_mpv_pid);
    fflush(stdout);
}

Audio create_dummy_audio(void) {
    Audio a;
    a.play = play_impl;
    a.stop = stop_impl;
    return a;
}