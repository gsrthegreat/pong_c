#include "sound_manager.h"

static SoundManager manager = {0};

int init_sound_manager() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        printf("SDL audio init failed: %s\n", SDL_GetError());
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer init failed: %s\n", Mix_GetError());
        return 0;
    }
    return 1;
}

void close_sound_manager() {
    for (int i = 0; i < manager.count; i++) {
        if (manager.sounds[i].chunk) {
            Mix_FreeChunk(manager.sounds[i].chunk);
        }
    }
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

int load_sound(const char *name, const char *file) {
    if (manager.count >= MAX_SOUNDS) return 0;

    Mix_Chunk *chunk = Mix_LoadWAV(file);
    if (!chunk) {
        printf("Failed to load %s: %s\n", file, Mix_GetError());
        return 0;
    }

    strcpy(manager.sounds[manager.count].name, name);
    manager.sounds[manager.count].chunk = chunk;
    manager.count++;
    return 1;
}

void play_sound(const char *name) {
    for (int i = 0; i < manager.count; i++) {
        if (strcmp(manager.sounds[i].name, name) == 0) {
            Mix_PlayChannel(-1, manager.sounds[i].chunk, 0);
            return;
        }
    }
    printf("Sound not found: %s\n", name);
}

void free_sound(const char *name) {
    for (int i = 0; i < manager.count; i++) {
        if (strcmp(manager.sounds[i].name, name) == 0) {
            Mix_FreeChunk(manager.sounds[i].chunk);
            manager.sounds[i] = manager.sounds[manager.count - 1];
            manager.count--;
            return;
        }
    }
}

