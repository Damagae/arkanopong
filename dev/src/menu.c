#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "primitives.h"
#include "level.h"
#include "audio.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

int animate = 0;
static bool mute = false;

/* DRAWING FUNCTIONS */

TextureList createMenuTextures()
{
    TextureList menuTextures = NULL;
    menuTextures = addTexture(&menuTextures, "data/img/menu/splashscreen.jpg");
    addTexture(&menuTextures, "data/img/menu/fond_menu.jpg");
    addTexture(&menuTextures, "data/img/menu/title.png");
    addTexture(&menuTextures, "data/img/menu/how_to.png");
    addTexture(&menuTextures, "data/img/menu/left.png");
    addTexture(&menuTextures, "data/img/menu/play_c_2p_on.png");
    addTexture(&menuTextures, "data/img/menu/play_c_easy_on.png");
    addTexture(&menuTextures, "data/img/menu/play_c_hard_on.png");
    addTexture(&menuTextures, "data/img/menu/map_editor_on.png");
    addTexture(&menuTextures, "data/img/menu/exit_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_1_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_2_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_3_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_4_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_5_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_6_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_7_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_8_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_9_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_10_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_11_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_12_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_13_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_14_on.png");
    addTexture(&menuTextures, "data/img/menu/map_select_15_on.png");

    return menuTextures;
}

void drawSplashScreen(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1);
        glScalef(WINDOW_WIDTH,WINDOW_HEIGHT,1);
        glRotatef(180, 0.0, 0.0, 1.0);
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
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_TEXTURE_2D);
}

void drawMenuSelection(bool* selected, char* mode, char* levelTxt, TextureList menuTextures, int lvl, int numLvl, int numMode)
{
    /* PLAY */
    drawMenuButton(menuTextures->texture[5+numMode], 300, 300, selected[0], mode);
    //if (strcmp(mode,"PLAY : PLAYER VS PLAYER"))
    if (numMode != 0)
        drawArrow(menuTextures->texture[4], 60, 300, selected[0], 1);
    //if (strcmp(mode,"PLAY : COMPUTER HARD"))
    if (numMode != 2)
        drawArrow(menuTextures->texture[4], 540, 300, selected[0], 0);
    /* LEVEL */
    drawMenuButton(menuTextures->texture[10+lvl], 300, 450, selected[1], levelTxt);
    if (lvl != 0)
        drawArrow(menuTextures->texture[4], 60, 450, selected[1], 1);
    if (lvl != numLvl-1)
        drawArrow(menuTextures->texture[4], 540, 450, selected[1], 0);
    /* EDITOR */
    drawMenuButton(menuTextures->texture[8], 300, 600, selected[2], "EDITOR");
    /* EXIT */
    drawMenuButton(menuTextures->texture[9], 300, 800, selected[3], "EXIT");
}

void drawMenuButton(GLuint texture, int x, int y, bool selected, char* txt)
{
    if (!selected)
        glColor4f(1.0,1.0,1.0,0.5);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(x-5*animate, y, 1);
        glScalef(-400,75,1);
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    
    //drawText(x-15-5*animate, y, txt, 6);
    glColor4f(1.0,1.0,1.0,1.0);
}

void drawArrow(GLuint texture, int x, int y, bool selected, int left)
{
    if (!selected)
        glColor4f(1.0,1.0,1.0,0.5);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(x-5*animate, y, 1);
        glScalef(-60*(-1+2*left),60,1);
        glRotatef(180,0,0,1.0);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,1.0);
}

void drawMenuText()
{
    glColor3f(0.0, 0.0, 0.0);
    drawText(250-5*animate,900,"PRESS ENTER TO CONTINUE", 6);
}

void drawLogo(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, 100-2*animate, 1);
        glScalef(-766,143,1);
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_TEXTURE_2D);
}

void drawHowToPlay(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH-200+5*animate, 550, 1);
        glScalef(-300,600,1);
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderMenu(TextureList menuTextures, State state, bool* selected, char* mode, char* levelTxt, int lvl, int numLvl, int numMode)
{
    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
            if(state == SPLASH)
                drawSplashScreen(menuTextures->texture[0]);
            else if(state == MENU || state == GAME || state == EDITOR)
            {
                drawWindowBackground(menuTextures->texture[1]);
                drawLogo(menuTextures->texture[2]);
                drawHowToPlay(menuTextures->texture[3]);
                drawMenuSelection(selected, mode, levelTxt, menuTextures, lvl, numLvl, numMode);
                drawMenuText();
            }
        glPopMatrix();
            
        SDL_GL_SwapBuffers();
        if (state == GAME)
            ++animate;
    } while (animate !=0 && animate < 100);
}

/* INTERACTION FUNCTIONS */

void selectButton(bool UP, Button* selection)
{
    if (UP)
    {
        if (*selection == PLAY_GAME)
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

void selectMode(bool RIGHT, Button* selection, int* gameMode)
{
    if (*selection == PLAY_GAME)
    {
        if(RIGHT)
        {
            if (*gameMode < 2)
                ++(*gameMode);
        }
        else
        {
            if (*gameMode > 0)
                --(*gameMode);
        }
    }
}

void selectLevel(bool RIGHT, Button* selection, int* lvl, int numLvl)
{
    if (*selection == LVL)
    {
        if(RIGHT)
        {
            if (*lvl < numLvl-1)
                ++(*lvl);
        }
        else
        {
            if (*lvl > 0)
                --(*lvl);
        }
    }
}
 
State menuEvent(State state, Button* selection, int* gameMode, int* lvl, int numLvl)
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
                    if (state == MENU && *selection == PLAY_GAME)
                        state = GAME;
                    else if (*selection == EDIT)
                        state = EDITOR;
                    else if (*selection == EXIT)
                        state = QUIT;
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
                    state = MENU;
                    selectMode(false, selection, gameMode);
                    selectLevel(false, selection, lvl, numLvl);
                    break;
                case SDLK_RIGHT:
                    state = MENU;
                    selectMode(true, selection, gameMode);
                    selectLevel(true, selection, lvl, numLvl);
                    break;
                default :
                    state = MENU;
                    break;
            }
            break;

        case SDL_KEYUP:
            switch(e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    state = QUIT;
                    break;
                case SDLK_m:
                    if (!mute)
                    {
                        Mix_PauseMusic();
                        mute = true;
                    }
                    else
                    {
                        Mix_ResumeMusic();
                        mute = false;
                    }
                    break;
                default :
                    break;
            }
            break;
          
        default:
            break;
      }
    }
    return state;
}

void textManager(int gameMode, int lvl, char* mode, char* levelTxt)
{
    strcpy(mode, "PLAY : ");
    switch (gameMode)
    {
        case 0 :
            strcat(mode, "PLAYER VS PLAYER");
            break;
        case 1 :
            strcat(mode, "COMPUTER NORMAL");
            break;
        case 2 :
            strcat(mode, "COMPUTER HARD");
            break;
        default :
            break;
    }
    strcpy(levelTxt, "MAP : LEVEL ");
    levelTxt[12] = '1'+lvl;
}

State menuManager(State state, unsigned int* AI, int* level)
{
    TextureList menuTextures = createMenuTextures();

    int numLvl;
    free(levelList(&numLvl));
    
    Button selection = PLAY_GAME;
    int gameMode = 0;
    int lvl = 0;

    char levelTxt[14];
    char mode[25];

    bool selected[5];
    int i;

    while(state == MENU || state == SPLASH)
    {
        Uint32 startTime = SDL_GetTicks();

        // Put true the selected button
        for (i = 0; i < 4; ++i)
        {
            if (selection != i)
                selected[i] = false;
            else
                selected[i] = true;
        }

        state = menuEvent(state, &selection, &gameMode, &lvl, numLvl);

        textManager(gameMode, lvl, mode, levelTxt);
        renderMenu(menuTextures, state, selected, mode, levelTxt, lvl, numLvl, gameMode);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // If AI Selected
    if(state == GAME && selection == PLAY_GAME)
    {
        *AI = gameMode;
    }
       
    *level = lvl;
    animate = 0;

    freeTexture(&menuTextures);

    return state;
}