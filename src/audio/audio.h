#ifndef AUDIO_H
#define AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*play)(const char *path);
    void (*stop)(void);
} Audio;

Audio create_dummy_audio(void);

#ifdef __cplusplus
}
#endif

#endif