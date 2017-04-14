#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "brick.h"
#include "primitives.h"
#include "geometry.h"
#include "bonus.h"

#define HEIGHT_DEFAULT 40
#define WIDTH_DEFAULT 90
#define LIFE_DEFAULT 3

Brick* createBrick (Point2D position, BrickType type, BonusList* bonusList)
{
    PtBrick ptBrick = malloc(sizeof(Brick));
    if (ptBrick == NULL)
    {
        fprintf(stderr, "Echec de l'allocation de la brique.\n");
        exit(EXIT_FAILURE);
    }

    ptBrick->position = position;
    ptBrick->width = WIDTH_DEFAULT;
    ptBrick->height = HEIGHT_DEFAULT;
    ptBrick->type = type;
    if (ptBrick->type == DISAP)
    {
        ptBrick->life = 1;
        ptBrick->bonus = NULL;
    }
    else if (ptBrick->type == INDES) 
    {
        ptBrick->life = -1;
        ptBrick->bonus = NULL;
    }
    else 
    {
        ptBrick->life = LIFE_DEFAULT;
        if (ptBrick->type != NORMAL)
        {
            Bonus* bonus = createBonus(ptBrick);
            ptBrick->bonus = bonus;
            addBonus(bonusList, bonus);
        }
        else
        {
            ptBrick->bonus = NULL;
        }
    }
    ptBrick->next = NULL;
   
    return ptBrick;
}

void drawBrick(Brick brick)
{
    if (brick.life != 0)
    {
        glPushMatrix();
        glTranslatef(brick.position.x, brick.position.y, 1);
        glScalef(brick.width, brick.height, 1);
        drawSquare();
        glPopMatrix();
    }
}

int brickDamaged(PtBrick ptBrick)
{
    return --ptBrick->life;
}

/* Brick vertices */
/* A____________B */
/* |            | */
/* |            | */
/* D____________C */

/* A */
Point2D brickVerticeTopLeft(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x - ptBrick->width/2, ptBrick->position.y - ptBrick->height/2);
}

/* B */
Point2D brickVerticeTopRight(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x + ptBrick->width/2, ptBrick->position.y - ptBrick->height/2);
}

/* C */
Point2D brickVerticeBottomLeft(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x + ptBrick->width/2, ptBrick->position.y + ptBrick->height/2);
}

/* D */
Point2D brickVerticeBottomRight(PtBrick ptBrick)
{
    return PointXY(ptBrick->position.x - ptBrick->width/2, ptBrick->position.y + ptBrick->height/2);
}


