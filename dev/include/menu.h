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
void drawMenuButton(bool* button);
void selectButton(bool UP, bool* button);
void renderMenu(TextureList menuTextures, State state, bool* selection);
State menuEvent(State state, bool* button);
State menuManager(bool* AI);

#endif