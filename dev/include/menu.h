#ifndef __MENU__H
#define __MENU__H

#include "textures.h"
#include "bool.h"
#include "utilities.h"
#include "audio.h"

/** STRUCTURES **/

typedef enum Button
{
    PLAY_GAME = 0,
    LVL = 1,
    EDIT = 2,
    EXIT = 3
} Button;

/** FONCTIONS **/

TextureList createMenuTextures();
void drawSplashScreen(GLuint splashScreen, GLuint text);
void drawWindowBackground(GLuint texture);
void drawLogo(GLuint texture);
void drawHowToPlay(GLuint texture);
void drawMenuText(GLuint texture);
void drawMenuSelection(bool* selected, char* mode, char* levelTxt, TextureList menuTextures, int lvl, int numLvl, int numMode);
void drawMenuButton(GLuint texture, int x, int y, bool selected, char* txt);
void drawArrow(GLuint texture, int x, int y, bool selected, int left);
void renderMenu(TextureList menuTextures, State state, bool* selected, char* mode, char* levelTxt, int lvl, int numLvl, int numMode);

void selectMode(bool RIGHT, Button* selection, int* gameMode);
void selectButton(bool UP, Button* selection);
void selectLevel(bool RIGHT, Button* selection, int* lvl, int numLvl);
void textManager(int gameMode, int lvl, char* mode, char* levelTxt);
State menuEvent(State state, Button* selection, int* dificulty, int* lvl, int* numLvl, Mix_Chunk* sound);
State menuManager(State state, unsigned int* AI, int* level);

#endif