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
#include "utilities.h"

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
    GLuint uiTexture[5];

    char* backgroundTextureFile[MAX_TEXTURES];
    char* brickTextureFile[MAX_TEXTURES];
    char* ballTextureFile[2];
    char* barTextureFile[2];
    char* lifeTextureFile[2];
    char* bonusTextureFile[MAX_TEXTURES];
    char* uiTextureFile[5];

    Bar bar[2];
    Player player[2];
    PtBall ballList;
    BonusList bonusList;
    PtBrick brickList;

    Direction direction[2];
    bool power[2];
    Position ballPosition;

    Direction selection;
    
    Mix_Chunk * sound[10];

    char* level;
} Game, *PtGame;

/** FONCTIONS **/

void setVideoMode(unsigned int width, unsigned int height);
void initSDL();
float randomNumber(float min, float max);
Game* createGame(int lvl);

void drawGameBorder();
void drawGameBackground(GLuint backgroundTexture);
void drawRestart(bool restart);
void drawPause(GLuint uiTexture);

void renderGame(Game* game, char timer, bool restart);
int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick, GLuint* brickTexture);
void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2, PtBall* ballList, Mix_Chunk ** sound);
Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player, GLuint* brickTexture);
Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, GLuint* brickTexture, Mix_Chunk ** sound);
Position runGame(Game* game);
bool gameEvent(Game* game, char timer, State* state);
bool playGame(Game* game, unsigned int AI, State* state);
char gameLaunch(Uint32 startTime);
void moveBarBall(PtBar bar, PtBall ball, Direction direction);
bool restartGame(Direction direction);

void slowGame(Game* game);
void accelerateGame(Game* game);
void powerManager(Game* game);

void freeGameTextures(Game* game);
void freeGame(Game* game);

#endif