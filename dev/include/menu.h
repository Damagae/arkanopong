#ifndef __MENU__H
#define __MENU__H

#include "textures.h"
#include "bool.h"

/** STRUCTURES **/

typedef enum State
{
    EXIT, SPLASH, MENU, PLAY
} State;


/** FONCTIONS **/

TextureList createMenuTextures();
void drawSplashScreen(GLuint texture);
void drawWindowBackground();
void drawMenuText();
void drawMenuButton(bool* selection, bool direction);
void selectButton(bool UP, bool* button);
void renderMenu(TextureList menuTextures, State state, bool* selection, bool direction);
State menuEvent(State state, bool* button, bool* direction);
State menuManager(unsigned int* AI);

#endif