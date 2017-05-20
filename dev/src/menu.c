#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <stdio.h>

#include "menu.h"
#include "primitives.h"
#include "level.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

/* DRAWING FUNCTIONS */

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

void drawMenuButton(Button selection, int difficulty, int lvl, char** levelFiles)
{
    bool selected[4];
    int i;
    for (i = 0; i < 4; ++i)
    {
        if (selection != i)
            selected[i] = false;
        else
            selected[i] = true;
    }

    drawButton(250, 100, "PLAYER VS PLAYER", selected[0]);
    drawButton(250, 300, "PLAYER VS COMPUTER", selected[1]);
    if (difficulty == 1)
        drawButton(250, 400, "NORMAL", false);
    else
        drawButton(250, 400, "HARD", false);
    drawButton(250, 550, "LEVEL", selected[2]);
    drawButton(250, 650, levelFiles[lvl], false);
    drawButton(250, 800, "EXIT", selected[3]);
}

void drawMenuText()
{
    glColor3f(0.0, 0.0, 0.0);
    drawText(250,950,"PRESS ENTER TO CONTINUE", 6);
}

void renderMenu(TextureList menuTextures, State state, Button selection, int difficulty, int lvl, char** levelFiles)
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
            drawMenuButton(selection, difficulty, lvl, levelFiles);
            drawMenuText();
        }
    glPopMatrix();
        
    SDL_GL_SwapBuffers();
}

/* INTERACTION FUNCTIONS */

void selectButton(bool UP, Button* selection)
{
    if (UP)
    {
        if (*selection == PVP)
            return;
        else
            --(*selection);
    }
    else
    {
        if(*selection == EXIT)
            return;
        else
            ++(*selection);
    }
}

void selectDifficulty(bool RIGHT, Button* selection, int* difficulty)
{
    if (*selection == COMPUTER)
    {
        if(RIGHT)
            *difficulty = 2;
        else
            *difficulty = 1;
    }
}

void selectLevel(bool RIGHT, Button* selection, int* lvl, int numLvl)
{
    if (*selection == LVL)
    {
        if(RIGHT)
        {
            if (*lvl < numLvl)
                ++(*lvl);
        }
        else
        {
            if (*lvl > 0)
                --(*lvl);
        }
    }
}
 
State menuEvent(State state, Button* selection, int* difficulty, int* lvl, int numLvl)
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      switch(e.type)
      {
        case SDL_QUIT:
            state = QUIT;
            break;          

        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
                case SDLK_RETURN:
                    if (state == MENU && (*selection == PVP || *selection == COMPUTER))
                        state = PLAY;
                    else if (*selection == EXIT)
                        state = QUIT;
                    else
                        state = MENU;
                    break;
                case SDLK_UP:
                    selectButton(true, selection);
                    break;
                case SDLK_DOWN:
                    selectButton(false, selection);
                    break;
                case SDLK_LEFT:
                    selectDifficulty(false, selection, difficulty);
                    selectLevel(false, selection, lvl, numLvl);
                    break;
                case SDLK_RIGHT:
                    selectDifficulty(true, selection, difficulty);
                    selectLevel(true, selection, lvl, numLvl);
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

State menuManager(unsigned int* AI, int* level)
{
    State state = SPLASH;
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");
    addTexture(&menuTextures, "data/img/menu/menuBackground.jpg");

    int numLvl, i;
    char ** levelFiles = levelList(&numLvl);
    char* levels[numLvl];
    for (i = 0; i <= numLvl; ++i)
    {
        levels[i] = levelFiles[i];
    }
    free(levelFiles);
    
    printf("%s\n",levels[0]);
    printf("%s\n",levels[1]);
    printf("%s\n",levels[2]);
    printf("test\n");
    Button selection = PVP;
    int difficulty = 1;
    int lvl = 1;

    while(state == MENU || state == SPLASH)
    {
        Uint32 startTime = SDL_GetTicks();

        renderMenu(menuTextures, state, selection, difficulty, lvl, levelFiles);

        state = menuEvent(state, &selection, &difficulty, &lvl, numLvl);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // If AI Selected
    if(state == PLAY && selection == COMPUTER)
    {
        if (difficulty == 1)
            *AI = 1;    // Normal
        else
            *AI = 2;    // Hard
    }

    *level = lvl;

    freeTexture(&menuTextures);

    return state;
}