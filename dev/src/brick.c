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

void addBrick(PtBrick* brickList, Brick* brick)
{
    if (*brickList == NULL)
        *brickList = brick;
    else
    {
        for (; (*brickList)->next != NULL; brickList = &((*brickList)->next));
        (*brickList)->next = brick;
    }
}

void drawBrick(Brick brick)
{
    if (brick.life != 0)
    {
        glPushMatrix();
        glTranslatef(brick.position.x, brick.position.y, 1);
        glScalef(brick.width, brick.height, 1);
        drawSquare();
        glColor3ub( 0, 0, 0 );
        drawSquareBorder();
        glColor3ub( 255, 255, 255 );
        glPopMatrix();
    }
}

void drawAllBricks(PtBrick brickList)
{
    for (; brickList != NULL; brickList = brickList->next)
    {
        drawBrick(*brickList);
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

// This function doesn't work well
void deleteBrick(PtBrick* brickList, PtBrick ptBrick)
{
    if (brickList == NULL)
        return;
    if (*brickList == ptBrick)
    {
        *brickList = NULL;
        //free(*ptBrick);
        ptBrick = NULL;
        return ;
    }
    PtBrick next;
    for (; (*brickList)->next != NULL && (*brickList)->next != ptBrick ; brickList = &next)
    {
        next = (*brickList)->next;
    }

    (*brickList)->next = ptBrick->next;
    //free(*ptBrick);
    ptBrick = NULL;
}

void deleteBrickList(PtBrick* brickList)
{
    if (brickList == NULL)
        return;
    PtBrick next;
    for (; *brickList != NULL; brickList = &next)
    {
        next = (*brickList)->next;
        free(*brickList);
        *brickList = NULL;
    }
}

BrickType getType(int t) {
    switch(t)
    {
        case 0:
            return INDES;
        case 1:
            return DISAP;
        case 2:
            return BARUP;
        case 3:
            return BARDWN;
        case 4:
            return BARSPDUP;
        case 5:
            return BALLSPDUP;
        case 6:
            return BALLSPDDWN;
        case 7:
            return BALLSIZEUP;
        case 8:
            return MOREBALL;
        case 9:
            return ADDLIFE;
        case 10:
            return NORMAL;
    }
    return NORMAL;
}

void createLevelBricks(int * lvl, int GAME_W, int GAME_H, PtBrick* brickList, BonusList* bonusList) {
    int largeur = lvl[0];
    int hauteur = lvl[1];
    int i = 0;
    int j = 0;
    float firstLine;
    float firstColumn;

    if (hauteur%2 == 0)
    {
        firstLine = GAME_H/2 - (hauteur/2) * HEIGHT_DEFAULT;
    } else
    {
        firstLine = GAME_H/2 - HEIGHT_DEFAULT/2 - (hauteur-1)/2 * HEIGHT_DEFAULT;
    }

    if (largeur%2 == 0)
    {
        firstColumn = GAME_W/2 - WIDTH_DEFAULT/2 - WIDTH_DEFAULT * (largeur/2 - 1);
    } else
    {
        firstColumn = GAME_W/2 - WIDTH_DEFAULT/2 - ((largeur-1)/2) * WIDTH_DEFAULT + 0.5 * WIDTH_DEFAULT;
    }
    printf("C %.0f\n", firstColumn);

    for(i = 0; i < hauteur; ++i)
    {
        for(j = 0; j < largeur; ++j)
        {
            addBrick(brickList, createBrick(PointXY(firstColumn + j * WIDTH_DEFAULT, firstLine + i * HEIGHT_DEFAULT), getType(lvl[3 + i * largeur + j]), bonusList));
        }   
    }
}