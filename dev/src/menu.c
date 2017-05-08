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

void drawMenuButton(bool* selection)
{
    drawButton(450, 250, "PLAYER VS PLAYER", selection[0]);
    drawButton(450, 450, "PLAYER VS COMPUTER", selection[1]);
    drawButton(450, 650, "EXIT", selection[2]);
}

void drawMenuText()
{
    glColor3f(0.0, 0.0, 0.0);
    drawText(450,800,"PRESS ENTER TO CONTINUE", 6);
}

void renderMenu(TextureList menuTextures, State state, bool* selection)
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
            drawMenuButton(selection);
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
 
State menuEvent(State state, bool* selection)
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

State menuManager(bool* AI)
{
    State state = SPLASH;
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");
    addTexture(&menuTextures, "data/img/menu/menuBackground.jpg");
    bool selection[3];
    selection[0] = true;
    selection[1] = selection[2] = false;

    while(state == MENU || state == SPLASH)
    {
        Uint32 startTime = SDL_GetTicks();

        renderMenu(menuTextures, state, selection);

        state = menuEvent(state, selection);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    if(state == PLAY && selection[1] == true)
        *AI = true;

    freeTexture(&menuTextures);

    return state;
}