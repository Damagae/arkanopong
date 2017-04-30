#ifndef __MANAGER__H
#define __MANAGER__H

#include "player.h"
#include "ball.h"
#include "bar.h"
#include "brick.h"
#include "collision.h"
#include "bonus.h"
#include "textures.h"
#include "level.h"
#include "ai.h"

#define MAX_TEXTURES 10

/** STRUCTURES **/

typedef struct
{
    bool start;
    
    TextureList brickTexture;
    TextureList barTexture;
    TextureList backgroundTexture;
    TextureList lifeTexture;
    TextureList bonusTexture;

    char* backgroundTextureFile[MAX_TEXTURES];
    char* brickTextureFile[MAX_TEXTURES];
    char* barTextureFile[2];
    char* lifeTextureFile[2];
    char* bonusTextureFile[MAX_TEXTURES];

    Bar bar[2];
    Player player[2];
    PtBall ballList;
    BonusList bonusList;
    PtBrick brickList;

    Direction direction[2];
    Position ballPosition;

    char* level;
} Game, *PtGame;

/** FONCTIONS **/

void setVideoMode(unsigned int width, unsigned int height);
void initSDL();
Game* createGame();
void drawGameBorder();
void drawGameBackground(Texture background);
void renderGame(Player player1, Player player2, PtBall ballList, PtBrick brickList, BonusList bonusList, Texture background, Texture life);
int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick);
void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2);
Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player);
Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player);
Position runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, BonusList* bonusList);
bool gameEvent(Game* game);
void playGame(Game* game, bool AI);
void freeGame(Game* game);

#endif