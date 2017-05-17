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
#include "audio.h"

#define MAX_TEXTURES 10

/** STRUCTURES **/

typedef struct
{
    bool start;
    bool pause;
    bool end;
    
    GLuint brickTexture[MAX_TEXTURES];
    GLuint barTexture[2];
    GLuint ballTexture[2];
    GLuint backgroundTexture[MAX_TEXTURES];
    GLuint lifeTexture[2];
    GLuint bonusTexture[MAX_TEXTURES];

    char* backgroundTextureFile[MAX_TEXTURES];
    char* brickTextureFile[MAX_TEXTURES];
    char* ballTextureFile[2];
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

    Direction selection;
    
    Mix_Chunk * sound[2];

    char* level;
} Game, *PtGame;

/** FONCTIONS **/

void setVideoMode(unsigned int width, unsigned int height);
void initSDL();
float randomNumber(float min, float max);
Game* createGame();

void drawGameBorder();
void drawGameBackground(GLuint backgroundTexture);
void drawRestart(bool restart);

void renderGame(Game* game, char timer, bool restart);
int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick);
void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2, PtBall* ballList);
Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player);
Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, Mix_Chunk ** sound);
Position runGame(Game* game);
bool gameEvent(Game* game, char timer);
bool playGame(Game* game, unsigned int AI);
char gameLaunch(Uint32 startTime);
void moveBarBall(PtBar bar, PtBall ball, Direction direction);
bool restartGame(Direction direction);

void freeGameTextures(Game* game);
void freeGame(Game* game);

#endif