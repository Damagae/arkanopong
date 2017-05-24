#include "audio.h"

void initAudio()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }
    else
    {
        Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
        Mix_AllocateChannels(14);
    }
}

void closeAudio()
{
    Mix_CloseAudio();
}

Mix_Music * createMusic(const char* filename)
{
    Mix_Music *music;
    music = Mix_LoadMUS(filename);
    return music;
}

void playMusic(Mix_Music *music)
{
    Mix_PlayMusic(music, -1);
}

void freeMusic(Mix_Music *music)
{
    Mix_FreeMusic(music);
}

Mix_Chunk * createSound(const char* filename)
{
    Mix_Chunk * sound;
    sound = Mix_LoadWAV(filename);
    Mix_VolumeChunk(sound, MIX_MAX_VOLUME/10);
    return sound;
}

void playSound(unsigned int channel, Mix_Chunk * sound)
{
    while(Mix_Playing(channel))
        ++channel;
    Mix_PlayChannel(channel, sound, 0);
}

void freeSound(Mix_Chunk* sound)
{
    Mix_FreeChunk(sound);
}