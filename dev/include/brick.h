#ifndef __BRICK__H
#define __BRICK__H

#include "geometry.h"
#include "ball.h"
#include "bool.h"
#define __BONUS__FORWARD__DECLARATION
#include "bonus.h"

/** STRUCTURES **/

typedef enum BrickType
{
    INDES, DISAP, BARUP, BARDWN, BARSPDUP, BALLSPDUP, BALLSPDDWN, BALLSIZEUP, MOREBALL, NORMAL
} BrickType;

typedef struct brick
{
    Point2D position;
    float width;
    float height;
    char* texture;
    int life;
    BrickType type;
    Bonus* bonus;
    struct brick* next;
} Brick, *PtBrick;


/** FUNCTIONS **/

Brick* createBrick (Point2D position, BrickType type, BonusList* bonusList);
void drawBrick(Brick brick);
int brickDamaged(PtBrick ptBrick);
Point2D brickVerticeTopLeft(PtBrick ptBrick);
Point2D brickVerticeTopRight(PtBrick ptBrick);
Point2D brickVerticeBottomLeft(PtBrick ptBrick);
Point2D brickVerticeBottomRight(PtBrick ptBrick);

#endif