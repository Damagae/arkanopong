#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>
#include "bar.h"
#include "primitives.h"

#define BAR_SPEED 6
#define BAR_WIDTH 100
#define BAR_HEIGHT 20

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

Bar createBar (Point2D position, GLuint texture)
{   
    Bar bar;
    bar.position = position;
    bar.width = BAR_WIDTH;
    bar.height = BAR_HEIGHT;
    bar.speed = BAR_SPEED;
    bar.texture = texture;

    return bar;
}

void moveBar(PtBar ptBar, Direction direction)
{
    float LEFT_BORDER = (WINDOW_WIDTH-GAME_WIDTH)/2;
    float RIGHT_BORDER = GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2;

    if(direction == RIGHT && barRightPosition(ptBar) <= RIGHT_BORDER)
    {
        ptBar->position.x += ptBar->speed;
    }
    else if (direction == LEFT && barLeftPosition(ptBar) >= LEFT_BORDER)
    {
        ptBar->position.x -= ptBar->speed;
    }
    // Block bar inside the game size
    if (barRightPosition(ptBar) >= RIGHT_BORDER)
    {
        ptBar->position.x = RIGHT_BORDER - ptBar->width/2;
    }
    else if (barLeftPosition(ptBar) <= LEFT_BORDER)
    {
        ptBar->position.x = LEFT_BORDER + ptBar->width/2;
    }
}

void drawBar(Bar bar, int numPlayer)
{
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, bar.texture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(bar.position.x, bar.position.y, 1);
        glScalef(bar.width, bar.height, 1);
        // if (numPlayer == 1)
            glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();
    glPopMatrix();
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/* For Power */
void drawGauge(Bar bar, int gauge, int numPlayer)
{
    int yPos = 20;
    if (numPlayer == 1)
        yPos = -20;

    glPushMatrix();
        glTranslatef(bar.position.x, bar.position.y+yPos, 1);
        glScalef(gauge, bar.height/2, 1);
        glColor3f(1.0, 0.5, 0.0);
        drawSquare();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(bar.position.x, bar.position.y+yPos, 1);
        glScalef(bar.width, bar.height/2, 1);
        glColor3f(1.0, 0.0, 0.0);
        drawSquareBorder();
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);  
}

/* Bar edges position */

float barTopPosition(PtBar ptBar)
{
    return ptBar->position.y - ptBar->height/2;
}

float barBottomPosition(PtBar ptBar)
{
    return ptBar->position.y + ptBar->height/2;
}

float barLeftPosition(PtBar ptBar)
{
    return ptBar->position.x - ptBar->width/2;
}

float barRightPosition(PtBar ptBar)
{
    return ptBar->position.x + ptBar->width/2;
}