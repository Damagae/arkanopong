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
    drawText(500,500,"PRESS ANY KEY TO CONTINUE", 6);
}

void drawWindowBackground(GLuint texture)
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
}

void drawMenuButton(bool* selection, bool direction)
{
    drawButton(450, 200, "PLAYER VS PLAYER", selection[0]);
    drawButton(450, 400, "PLAYER VS COMPUTER", selection[1]);
    if (selection[1])
    {
        drawButton(350, 500, "NORMAL", direction);
        drawButton(550, 500, "HARD", (direction ? 0 : 1));
    }
    else
    {
        drawButton(350, 500, "NORMAL", false);
        drawButton(550, 500, "HARD", false);
    }
    drawButton(450, 700, "EXIT", selection[2]);
}

void drawMenuText()
{
    glColor3f(0.0, 0.0, 0.0);
    drawText(450,820,"PRESS ENTER TO CONTINUE", 6);
}

void renderMenu(TextureList menuTextures, State state, bool* selection, bool direction)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        if(state == SPLASH)
            drawSplashScreen(menuTextures->texture[0]);
        else if(state == MENU)
        {
            drawWindowBackground(menuTextures->texture[1]);
            drawMenuButton(selection, direction);
            drawMenuText();
        }
    glPopMatrix();
        
    SDL_GL_SwapBuffers();
}

void selectButton(bool UP, bool* selection)
{
    if (UP)
    {
        if(selection[0])
            return;
        else if(selection[1])
        {
            selection[0] = true;
            selection[1] = false;
        }
        else if(selection[2])
        {
            selection[1] = true;
            selection[2] = false;
        }
    }
    else
    {
        if(selection[2])
            return;
        else if(selection[1])
        {
            selection[2] = true;
            selection[1] = false;
        }
        else if(selection[0])
        {
            selection[1] = true;
            selection[0] = false;
        }
    }
}
 
State menuEvent(State state, bool* selection, bool* direction)
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      switch(e.type)
      {
        case SDL_QUIT:
            state = EXIT;
            break;          

        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
                case SDLK_RETURN:
                    if (state == MENU && selection[2] == false)
                        state = PLAY;
                    else if (selection[2] == true)
                        state = EXIT;
                    else
                        state = MENU;
                    break;
                case SDLK_UP:
                    state = MENU;
                    selectButton(true, selection);
                    break;
                case SDLK_DOWN:
                    state = MENU;
                    selectButton(false, selection);
                    break;
                case SDLK_LEFT:
                    if(selection[1])
                        *direction = true;
                    break;
                case SDLK_RIGHT:
                    if(selection[1])
                        *direction = false;
                    break;
                default :
                    state = MENU;
                    break;
            }
            break;

        case SDL_KEYUP:
            break;
          
        default:
            break;
      }
    }
    return state;
}

State menuManager(unsigned int* AI)
{
    State state = SPLASH;
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");
    addTexture(&menuTextures, "data/img/menu/menuBackground.jpg");
    bool selection[3];
    selection[0] = true;
    selection[1] = selection[2] = false;
    bool direction = true;

    while(state == MENU || state == SPLASH)
    {
        Uint32 startTime = SDL_GetTicks();

        renderMenu(menuTextures, state, selection, direction);

        state = menuEvent(state, selection, &direction);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    if(state == PLAY && selection[1] == true)
        {
            if (direction)
                *AI = 1;
            else
                *AI = 2;
        }

    freeTexture(&menuTextures);

    return state;
}