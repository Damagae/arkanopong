#ifndef __GAME__H
#define __GAME__H

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

/** STRUCTURES **/

typedef struct
{
    bool start;
    bool pause;
    bool end;

    unsigned int AI;
    
    GLuint backgroundTexture[2];
    GLuint barTexture[10];
    GLuint ballTexture[2];
    GLuint lifeTexture[2];
    GLuint brickTexture[4];
    GLuint bonusTexture[14];
    GLuint uiTexture[16];

    char* backgroundTextureFile[2];
    char* barTextureFile[10];
    char* ballTextureFile[2];
    char* lifeTextureFile[2];
    char* brickTextureFile[4];
    char* bonusTextureFile[14];
    char* uiTextureFile[16];

    Bar bar[2];
    Player player[2];
    PtBall ballList;
    BonusList bonusList;
    PtBrick brickList;

    Direction direction[2];
    bool power[2];
    Position ballPosition;

    Direction selection;
    
    Mix_Chunk * sound[15];

    char* level;
} Game, *PtGame;

/** FONCTIONS **/

void setVideoMode(unsigned int width, unsigned int height);
void initSDL();
float randomNumber(float min, float max);
Game* createGame(int lvl, unsigned int AI);

void drawGameBorder();
void drawGameBackground(GLuint backgroundTexture);
void drawWinner(Player player1, unsigned int AI, GLuint* uiTexture);
void drawRestart(bool restart, GLuint* texture);
void drawPause(GLuint uiTexture);
void drawHit();

void renderGame(Game* game, char timer, bool restart);
int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick, GLuint* brickTexture);
void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2, PtBall* ballList, Mix_Chunk ** sound, GLuint* barTexture);
Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player, GLuint* brickTexture);
Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, GLuint* brickTexture, Mix_Chunk ** sound);
Position runGame(Game* game);
bool gameEvent(Game* game, char timer, State* state);
bool playGame(Game* game, State* state);
char gameLaunch(Uint32 startTime);
void moveBarBall(PtBar bar, PtBall ball, Direction direction);
bool restartGame(Direction direction);

void slowGame(Game* game);
void accelerateGame(Game* game);
void powerManager(Game* game);

void freeGameTextures(Game* game);
void freeGame(Game* game);

#endif