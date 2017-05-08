#ifndef __BRICK__H
#define __BRICK__H

#include "GL/glu.h"

#include "geometry.h"
#include "ball.h"
#include "bool.h"
#define __BONUS__FORWARD__DECLARATION
#include "bonus.h"
#include "textures.h"

/** STRUCTURES **/

typedef enum BrickType
{
    EMPTY, NORMAL, BARUP, BARDWN, BARSPDUP, BALLSPDUP, BALLSPDDWN, ADDBALL, ADDLIFE, DISAP, INDES
} BrickType;

typedef struct brick
{
    Point2D position;
    float width;
    float height;
    Texture* ptTexture;
    int life;
    BrickType type;
    Bonus* bonus;
    struct brick* next;
} Brick, *PtBrick;

/** FUNCTIONS **/

Brick* createBrick (Point2D position, BrickType type, BonusList* bonusList, TextureList* brickTexture, char* textureFile, TextureList* bonusTexture, char** bonusTextureFile);
void addBrick(PtBrick* brickList, Brick* brick);
void drawBrick(Brick brick);
void drawAllBricks(PtBrick brickList);
int brickDamaged(PtBrick ptBrick);
Point2D brickVerticeTopLeft(PtBrick ptBrick);
Point2D brickVerticeTopRight(PtBrick ptBrick);
Point2D brickVerticeBottomLeft(PtBrick ptBrick);
Point2D brickVerticeBottomRight(PtBrick ptBrick);

void createLevelBricks(int * lvl, int GAME_W, int GAME_H, PtBrick* brickList, BonusList* bonusList, TextureList* brickTexture, char** brickTextureFile, TextureList* bonusTexture, char** bonusTextureFile);
BrickType getType(int t);
int selectBonus(BrickType type);

void deleteBrick(PtBrick* brickList, PtBrick ptBrick);
void deleteBrickList(PtBrick* brickList);

#endif