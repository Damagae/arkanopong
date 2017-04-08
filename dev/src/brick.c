#include <math.h>
#include <GL/gl.h>

#include "brick.h"
#include "primitives.h"
#include "geometry.h"

#define HEIGHT_DEFAULT 40
#define WIDTH_DEFAULT 90

Brick createBrick (Point2D position, BrickType type)
{
    Brick brick;
    brick.position = position;
    brick.width = WIDTH_DEFAULT;
    brick.height = HEIGHT_DEFAULT;
    brick.type = type;
    if (brick.type == DISAP)
    {
        brick.life = 1;
    }
    else if (brick.type == INDES) 
    {
        brick.life = -1;
    }
    else 
    {
        brick.life = 3;
    }
   
    return brick;
}

void drawBrick(Brick brick)
{
    glPushMatrix();
    glTranslatef(brick.position.x, brick.position.y, 1);
    glScalef(brick.width, brick.height, 1);
    drawSquare();
    glPopMatrix();
}

/* Brick vertices */
/* A____________B */
/* |            | */
/* |            | */
/* C____________D */

/* A */
Point2D brickVerticeTopLeft(PtBrick ptBrick)
{
    return ptBrick->position;
}

/* B */
Point2D brickVerticeTopRight(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x + ptBrick->width, ptBrick->position.y);
}

/* C */
Point2D brickVerticeBottomLeft(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x, ptBrick->position.y + ptBrick->height);
}

/* D */
Point2D brickVerticeBottomRight(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x + ptBrick->width, ptBrick->position.y + ptBrick->height);
}