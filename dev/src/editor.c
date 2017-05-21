#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "editor.h"
#include "primitives.h"
#include "audio.h"
#include "level.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

static bool mute = false;

#define HEIGHT_DEFAULT 33
#define WIDTH_DEFAULT 67

/* Drawing Functions */

void drawGrid()
{
    int i, j;
    float x, y;
    glColor3f(1.0, 0.0, 0.0);
    for (i = 0; i <= 10; i++)
    {
        glPushMatrix();
            x = 98+GAME_WIDTH/2;
            y = 338+i*HEIGHT_DEFAULT;
            glTranslatef(x, y, 1);
            glScalef(GAME_WIDTH, 1, 1);
            drawLine(-0.5,0,0.5,0);
        glPopMatrix();
    }

    for (j = 0; j <= 11; j++)
    {
        glPushMatrix();
            x = WINDOW_WIDTH-GAME_WIDTH-WIDTH_DEFAULT/2+j*WIDTH_DEFAULT;
            y = 502;
            glTranslatef(x, y, 1);
            glScalef(1, 10*HEIGHT_DEFAULT, 1);
            drawLine(0.0,-0.5,0.0,0.5);
        glPopMatrix();
    }
}

void drawBrickPreview(GLuint texture, int position)
{
    float x, y;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        x = (96+WIDTH_DEFAULT/2)+WIDTH_DEFAULT*(position%12);
        y = 355+(HEIGHT_DEFAULT*(position/12));
        glTranslatef(x, y, 1);
        glScalef(WIDTH_DEFAULT, HEIGHT_DEFAULT, 1);
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void drawTab(int* tab, TextureList editorTextures)
{
    int i;
    float x, y;
    glEnable(GL_TEXTURE_2D);
    for (i = 0; i < 120; ++i)
    {
        if (tab[i] != 0)
        {
            glBindTexture(GL_TEXTURE_2D, editorTextures->texture[tab[i]+2]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();
                x = (96+WIDTH_DEFAULT/2)+WIDTH_DEFAULT*(i%12);
                y = 355+(HEIGHT_DEFAULT*(i/12));
                glTranslatef(x, y, 1);
                glScalef(WIDTH_DEFAULT, HEIGHT_DEFAULT, 1);
                glRotatef(180, 0.0, 0.0, 1.0);
                drawSquareTexture();
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void renderEditor(TextureList editorTextures, int position, int* tab, int selection)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        drawWindowBackground(editorTextures->texture[0]);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        drawGameBackground(editorTextures->texture[1]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawTab(tab, editorTextures);
    drawBrickPreview(editorTextures->texture[selection+2], position);

    drawGrid();

    SDL_GL_SwapBuffers();
}

/* Interaction functions */

// UP - 0 / DOWN = 1 / LEFT = 2 / RIGHT = 3
int changePosition(int direction, int position)
{
    if (direction == 0 && (position-12)>=0)
        return -12;
    else if (direction == 1 && (position+12)<=119)
        return 12;
    else if (direction == 2 && (position-1)>=0)
        return -1;
    else if (direction == 3 && (position+1)<=119)
        return +1;
    else return 0;
}

int switchSelection(int selection)
{
    switch (selection)
    {
        case 0 :
            return 1;
            break;
        case 1 :
            return 2;
            break;
        case 2 :
            return 0;
            break;
        default :
            return 1;
    }
}

bool editorEvent(State* state, int* position, int *tab, int* selection)
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      switch(e.type)
      {
        case SDL_QUIT:
            *state = QUIT;
            return false;
            break;          

        case SDL_KEYDOWN:
            switch(e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    break;
                case SDLK_RETURN:
                    tab[*position] = *selection;
                    break;
                case SDLK_SPACE:
                    *selection = switchSelection(*selection);
                    break;
                case SDLK_UP:
                    *position += changePosition(0, *position);
                    break;
                case SDLK_DOWN:
                    *position += changePosition(1, *position);
                    break;
                case SDLK_LEFT:
                    *position += changePosition(2, *position);
                    break;
                case SDLK_RIGHT:
                    *position += changePosition(3, *position);
                    break;
                case SDLK_s:
                    createLevel(tab);
                    return false;
                    break;
                default :
                    break;
            }
            break;

        case SDL_KEYUP:
            switch(e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    return false;
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
    return true;
}

bool editorManager(State* state)
{
    TextureList editorTextures = NULL;
    editorTextures = addTexture(&editorTextures, "data/img/background/greyBackground.jpg");
    addTexture(&editorTextures, "data/img/background/fond.jpg");
    addTexture(&editorTextures, "data/img/delete.png");
    addTexture(&editorTextures, "data/img/brick/P_lego_4x2.png");
    addTexture(&editorTextures, "data/img/brick/R_lego_4x2.png");

    int tab[120];
    int position = 0;
    int i;
    
    for (i=0; i < 120; i++)
    {
        tab[i] = 0;
    }

    int selection = 1;

    bool inEditor = true;
    while(inEditor)
    {
        Uint32 startTime = SDL_GetTicks();

        renderEditor(editorTextures, position, tab, selection);

        inEditor = editorEvent(state, &position, tab, &selection);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    freeTexture(&editorTextures);

    return inEditor;
}