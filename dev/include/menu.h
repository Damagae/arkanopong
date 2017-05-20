#ifndef __MENU__H
#define __MENU__H

#include "textures.h"
#include "bool.h"

/** STRUCTURES **/

typedef enum State
{
    QUIT, SPLASH, MENU, PLAY
} State;

typedef enum Button
{
    PVP = 0,
    COMPUTER = 1,
    LVL = 2,
    EXIT = 3
} Button;

/** FONCTIONS **/

TextureList createMenuTextures();
void drawSplashScreen(GLuint texture);
void drawWindowBackground();
void drawMenuText();
void drawMenuButton(Button selection, int difficulty, int lvl);
void renderMenu(TextureList menuTextures, State state, Button selection, int difficulty, int lvl);

void selectDifficulty(bool RIGHT, Button* selection, int* difficulty);
void selectButton(bool UP, Button* selection);
void selectLevel(bool RIGHT, Button* selection, int* lvl, int numLvl);
State menuEvent(State state, Button* selection, int* dificulty, int* lvl, int numLvl);
State menuManager(unsigned int* AI, int* level);

#endif