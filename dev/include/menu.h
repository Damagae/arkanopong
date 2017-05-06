#ifndef __MENU__H
#define __MENU__H

#include "textures.h"
#include "bool.h"

/** STRUCTURES **/

typedef enum Selection
{
    EXIT, MENU, PLAY
} Selection;


/** FONCTIONS **/

TextureList createMenuTextures();
void drawSplashScreen(GLuint texture);
void renderMenu(TextureList menuTextures);
Selection menuEvent();
Selection menuManager();

#endif