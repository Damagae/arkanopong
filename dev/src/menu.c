#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <stdio.h>

#include "menu.h"
#include "primitives.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

TextureList createMenuTextures()
{
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");

    return menuTextures;
}

void drawSplashScreen(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1);
        glScalef(WINDOW_WIDTH,WINDOW_HEIGHT,1);
        drawSquareTexture();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.0, 0.0, 0.0);
    drawText(500,500,"PRESS ANY KEY TO CONTINUE");
}

void renderMenu(TextureList menuTextures)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        drawSplashScreen(menuTextures->texture[0]);
    glPopMatrix();
        
    SDL_GL_SwapBuffers();
}

Selection menuEvent()
{
    Selection inMenu = MENU;

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      
      switch(e.type) {

        case SDL_QUIT:
            inMenu = EXIT;
            break;          

        case SDL_KEYDOWN:
            inMenu = PLAY;
            break;

        case SDL_KEYUP:
            break;
          
        default:
            break;
      }
    }
    return inMenu;
}

Selection menuManager()
{
    Selection inMenu = MENU;
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");
    printf("%d\n",menuTextures->texture[0]);
    while(inMenu == MENU)
    {
        Uint32 startTime = SDL_GetTicks();

        renderMenu(menuTextures);

        inMenu = menuEvent();

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    freeTexture(&menuTextures);

    return inMenu;
}