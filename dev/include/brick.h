#ifndef __BRICK__H
#define __BRICK__H

#include "geometry.h"

/** STRUCTURES **/

typedef enum BrickType
{
    INDES, DISAP, BARUP, BARDWN, BARSPDUP, BALLSPDUP, BALLSPDDWN, BALLSIZEUP, MOREBALL
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

#endif