#include <GL/gl.h>

#include "bar.h"
#include "primitives.h"

#define BAR_SPEED 10.0
#define BAR_WIDTH 200.0
#define BAR_HEIGHT 30.0

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

Bar createBar (Point2D position)
{
    Bar bar;
    bar.position = position;
    bar.width = BAR_WIDTH;
    bar.height = BAR_HEIGHT;
    bar.speed = BAR_SPEED;
   
    return bar;
}

void moveBar(PtBar ptBar, Direction direction)
{
    if(direction == RIGHT && (ptBar->position.x + ptBar->width/2 < GAME_WIDTH))
    {
        ptBar->position.x += ptBar->speed;
    }
    else if (direction == LEFT && (ptBar->position.x - ptBar->width/2 > 0))
    {
        ptBar->position.x -= ptBar->speed;
    }
}

void drawBar(Bar bar, int numPlayer)
{
    glPushMatrix();
    glTranslatef(bar.position.x, bar.position.y, 1);
    glScalef(bar.width, bar.height, 1);
    if (numPlayer == 1)
        glRotatef(180, 0.0, 0.0, 1.0);
    drawSquare();
    glPopMatrix();
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