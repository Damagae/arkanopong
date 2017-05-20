#ifndef __MENU__H
#define __MENU__H

#include "textures.h"
#include "bool.h"
#include "utilities.h"

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
void drawSplashScreen(GLuint texture);
void drawWindowBackground(GLuint texture);
void drawLogo(GLuint texture);
void drawHowToPlay(GLuint texture);
void drawMenuText();
void drawMenuButton(bool* selected, char* mode, char* levelTxt);
void renderMenu(TextureList menuTextures, State state, bool* selected, char* mode, char* levelTxt);

void selectMode(bool RIGHT, Button* selection, int* gameMode);
void selectButton(bool UP, Button* selection);
void selectLevel(bool RIGHT, Button* selection, int* lvl, int numLvl);
void textManager(int gameMode, int lvl, char* mode, char* levelTxt);
State menuEvent(State state, Button* selection, int* dificulty, int* lvl, int numLvl);
State menuManager(State state, unsigned int* AI, int* level);

#endif