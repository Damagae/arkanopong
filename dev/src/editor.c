#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "editor.h"
#include "primitives.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

/* Drawing Functions */

void drawGrid()
{
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1);
        glScalef(GAME_WIDTH, GAME_HEIGHT, 1);
        drawSquare();
        glColor3f(0.0, 0.0, 1.0);
        drawLine(-0.5, 0, 0.5, 0);
    glPopMatrix();
}

void renderEditor(TextureList editorTextures)
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
            
    drawGrid();

    SDL_GL_SwapBuffers();
}

/* Interaction functions */

bool editorEvent(State* state)
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
                    break;
                case SDLK_UP:
                    break;
                case SDLK_DOWN:
                    break;
                case SDLK_LEFT:
                    break;
                case SDLK_RIGHT:
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
    addTexture(&editorTextures, "data/img/brick/P_lego_4x2.png");
    addTexture(&editorTextures, "data/img/brick/R_lego_4x2.png");



    bool inEditor = true;

    while(inEditor)
    {
        Uint32 startTime = SDL_GetTicks();

        renderEditor(editorTextures);

        inEditor = editorEvent(state);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    freeTexture(&editorTextures);

    return inEditor;
}