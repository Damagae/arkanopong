#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>

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

void drawTitle(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT-GAME_HEIGHT, 1);
        glScalef(-GAME_WIDTH*2/5, 100, 1);
        glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void drawGrid()
{
    int i, j;
    float x, y;
    glColor3f(1.0, 1.0, 1.0);
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

void drawBrickPreview(GLuint texture, int position, int color)
{
    float x, y;
    Color c = createColor(color);
    glColor3f(c.r, c.g, c.b);
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
    glColor3f(1.0, 1.0, 1.0);
}

void drawEditorBackground(GLuint backgroundTexture)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2 - 3, GAME_TOP_LEFT.y + GAME_HEIGHT/2 + 2, 1);
        glScalef(GAME_WIDTH, GAME_HEIGHT/2 - 72, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawTab(int* tab, int* tabColor, TextureList editorTextures)
{
    int i;
    float x, y;
    glEnable(GL_TEXTURE_2D);
    for (i = 0; i < 120; ++i)
    {
        if (tab[i] != 0)
        {
            if (tab[i] == 9) // INDES
                glBindTexture(GL_TEXTURE_2D, editorTextures->texture[3]);
            else
            {
                Color color = createColor(tabColor[i]);
                glColor3f(color.r, color.g, color.b);
                glBindTexture(GL_TEXTURE_2D, editorTextures->texture[4]);
            }
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
            glColor3f(1.0, 1.0, 1.0);
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void drawHowToEdit(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT - WINDOW_HEIGHT/5, 1);
        glScalef(GAME_WIDTH, -WINDOW_HEIGHT/6, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderEditor(TextureList editorTextures, int position, int* tab, int* tabColor, int selection, int color)
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
        drawEditorBackground(editorTextures->texture[1]);
        drawTitle(editorTextures->texture[6]);
        drawHowToEdit(editorTextures->texture[5]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawTab(tab, tabColor, editorTextures);
    if (selection == 2)
    {
        drawBrickPreview(editorTextures->texture[selection+2], position, color);
        glColor3f(0.8,0.15,0.2);
        drawText(WINDOW_WIDTH/2, GAME_HEIGHT/3+50, "RANDOM BRICK", 6);
    }
    else
    {
        drawBrickPreview(editorTextures->texture[selection+2], position, 0);
        glColor3f(0.8,0.15,0.2);
        if (selection == 1) drawText(WINDOW_WIDTH/2, GAME_HEIGHT/3+50, "UNBREAKABLE BRICK", 6);
        else drawText(WINDOW_WIDTH/2, GAME_HEIGHT/3+50, "EMPTY SLOT", 6);
    }

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

int switchColor(int color)
{
    switch (color)
    {
        case 1 :
            return 2;
            break;
        case 2 :
            return 3;
            break;
        case 3 :
            return 4;
            break;
        case 4 :
            return 1;
            break;
        default :
            return 1;
    }
}

int putBrick(int selection)
{
    if (selection == 0)
        return 0;
    else if (selection == 1)
        return 9;
    else
    {
        srand(time(NULL));   // should only be called once
        // Return a number between 2 and 8 (if it's 9 or 10, it will be 2)
        int r = 2 + (rand() % 9);
        // To make more normal brick than bonus brick
        if (r == 9 || r == 10)
            r = 2;
        return r;
    }
        
}

bool editorEvent(State* state, int* position, int *tab, int* selection, int* color, int* tabColor, Mix_Chunk* confirmSound, Mix_Chunk* saveSound)
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
                    tab[*position] = putBrick(*selection);
                    if (tab[*position]!=0)
                        tabColor[*position] = *color;
                    playSound(0, confirmSound);
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
                case SDLK_c:
                    *color = switchColor(*color);
                    break;
                case SDLK_s:
                    createLevel(tab, tabColor);
                    playSound(1, saveSound);
                    SDL_Delay(1500);
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
    editorTextures = addTexture(&editorTextures, "data/img/menu/fond_menu.jpg");
    addTexture(&editorTextures, "data/img/editor/background_editor.png");
    addTexture(&editorTextures, "data/img/editor/cross.png");
    addTexture(&editorTextures, "data/img/brick/S_indes_brick.png");
    addTexture(&editorTextures, "data/img/brick/W_brick_0.png");
    addTexture(&editorTextures, "data/img/editor/How_to_edit_level.png");
    addTexture(&editorTextures, "data/img/editor/editor_title.png");

    Mix_Chunk * confirmSound = createSound("data/audio/confirm.wav");
    Mix_Chunk * saveSound = createSound("data/audio/save.wav");

    int tab[120];
    int tabColor[120];
    int position = 0;
    int i;
    int color = 1;
    
    for (i=0; i < 120; i++)
    {
        tab[i] = 0;
        tabColor[i] = 0;
    }

    int selection = 1;

    bool inEditor = true;
    while(inEditor)
    {
        Uint32 startTime = SDL_GetTicks();

        renderEditor(editorTextures, position, tab, tabColor, selection, color);

        inEditor = editorEvent(state, &position, tab, &selection, &color, tabColor, confirmSound, saveSound);

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    freeTexture(&editorTextures);
    freeSound(confirmSound);
    freeSound(saveSound);

    return inEditor;
}