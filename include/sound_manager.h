#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define MAX_SOUNDS 32

typedef struct {
    char name[64];
    Mix_Chunk *chunk;
} Sound;

typedef struct {
    Sound sounds[MAX_SOUNDS];
    int count;
} SoundManager;

int init_sound_manager();
void close_sound_manager();
int load_sound(const char *name, const char *file);
void play_sound(const char *name);
void free_sound(const char *name);

#endif

