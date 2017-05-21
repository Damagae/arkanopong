#ifndef __BRICK__H
#define __BRICK__H

#include "GL/glu.h"

#include "geometry.h"
#include "primitives.h"
#include "ball.h"
#include "bool.h"
#define __BONUS__FORWARD__DECLARATION
#include "bonus.h"
#include "textures.h"

/** STRUCTURES **/

typedef enum BrickType
{
    EMPTY = 0,
    NORMAL = 1,
    BARUP = 2,
    BARDWN = 3,
    BARSPDUP = 4, 
    SLOWPOW = 5,
    FASTPOW = 6,
    ADDBALL = 7,
    ADDLIFE = 8,
    INDES = 9
} BrickType;

typedef struct brick
{
    Point2D position;
    float width;
    float height;
    int life;
    Color color;
    GLuint texture;
    BrickType type;
    Bonus* bonus;
    struct brick* next;
} Brick, *PtBrick;

/** FUNCTIONS **/

Brick* createBrick (Point2D position, BrickType type, BonusList* bonusList, GLuint brickTexture, GLuint* bonusTexture, int color);
void addBrick(PtBrick* brickList, Brick* brick);
void drawBrick(Brick brick);
void drawAllBricks(PtBrick brickList);
int brickDamaged(PtBrick ptBrick);
Point2D brickVerticeTopLeft(PtBrick ptBrick);
Point2D brickVerticeTopRight(PtBrick ptBrick);
Point2D brickVerticeBottomLeft(PtBrick ptBrick);
Point2D brickVerticeBottomRight(PtBrick ptBrick);

void createLevelBricks(int * lvl, int GAME_W, int GAME_H, PtBrick* brickList, BonusList* bonusList, GLuint* brickTexture, GLuint* bonusTexture);
BrickType getType(int t);

void deleteBrick(PtBrick* brickList, PtBrick ptBrick);
void deleteBrickList(PtBrick* brickList);

#endif