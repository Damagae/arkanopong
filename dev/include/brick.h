#ifndef __BRICK__H
#define __BRICK__H

#include "geometry.h"
#include "ball.h"
#include "bool.h"

/** STRUCTURES **/

typedef enum BrickType
{
    INDES, DISAP, BARUP, BARDWN, BARSPDUP, BALLSPDUP, BALLSPDDWN, BALLSIZEUP, MOREBALL, NORMAL
} BrickType;

typedef struct
{
    Point2D position;
    float width;
    float height;
    char* texture;
    int life;
    BrickType type;
} Brick;

typedef Brick* PtBrick;


/** FUNCTIONS **/

Brick createBrick (Point2D position, BrickType type);
void drawBrick(Brick brick);
Point2D brickVerticeTopLeft(PtBrick ptBrick);
Point2D brickVerticeTopRight(PtBrick ptBrick);
Point2D brickVerticeBottomLeft(PtBrick ptBrick);
Point2D brickVerticeBottomRight(PtBrick ptBrick);

#endif