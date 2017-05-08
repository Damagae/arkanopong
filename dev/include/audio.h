#ifndef __AUDIO__H
#define __AUDIO__H

#include <SDL/SDL_mixer.h>

/* FUNCTIONS */

void initAudio();
void closeAudio();
Mix_Music * createMusic(const char* filename);
void playMusic(Mix_Music *music);
void freeMusic(Mix_Music *music);
Mix_Chunk * createSound(const char* filename);
void playSound(unsigned int channel, Mix_Chunk * sound);
void freeSound(Mix_Chunk* sound);

#endif