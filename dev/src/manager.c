#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>

#include "manager.h"
#include "primitives.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

extern int LIFE_MAX;

int loseLife(PtPlayer player)
{
    (player->life)--;
    if (player->life == 0)
        return 0;
    else
        return player->life;
}

void drawGameBorder()
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glPushMatrix();
    glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
    glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
    drawSquareBorder();
    glPopMatrix();
}

void renderGame(Player player1, Player player2, PtBall ballList, PtBrick brickList, BonusList bonusList)
{    
    glClear(GL_COLOR_BUFFER_BIT);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    glColor3f(1.0, 0.0, 0.0);
    drawGameBorder();

    glColor3f(1.0, 1.0, 1.0);
    drawAllBalls(ballList);

    drawBar(*(player1.ptBar), player1.num);
    drawBar(*(player2.ptBar), player2.num);

    drawAllBricks(brickList);

    drawAllBonus(bonusList);

    drawLife(player1);
    drawLife(player2);


    SDL_GL_SwapBuffers();
}

void brickManager(PtBall ptBall, PtBrick* brickList, PtBrick* ptBrick)
{
    if (brickDamaged(*ptBrick) == 0)
    {
        (*ptBrick)->bonus->ptPlayer = ptBall->ptPlayer;
        bonusOrientation((*ptBrick)->bonus, *(ptBall->ptPlayer));
        deleteBrick(ptBrick, ptBrick);
    }
}

void bonusManager(BonusList* bonusList, BonusList* bonus, Position bonusPosition)
{
    if (bonusPosition == OUT_UP || bonusPosition == OUT_DOWN || bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
    {
        if (bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
        {
            getBonus(*(*bonus));
        }   
        deleteBonus(bonusList, bonus);
    }
}

int ballManager(PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player, BonusList* bonusList)
{
    int ballPosition;
    int bonusPosition = INSIDE;
    int alive = LIFE_MAX;
    Bonus* bonus = findBonus(*bonusList, *ptBrick);

    moveBall(ptBall);
    moveBonus(bonus);

    ballPosition = checkBallPosition(ptBall, bar1, bar2, ptBrick, player);
    if (bonus != NULL)
    {
        bonusPosition = checkBonusPosition(*bonus, bar1, bar2);
    }
        
    if (ballPosition == OUT_DOWN)
    {
        alive = loseLife(&player[0]);
    }
    else if (ballPosition == OUT_UP)
    {
        alive = loseLife(&player[1]);
    }

    // Change the ball owner
    else if (ballPosition == BAR_UP)
    {
        ptBall->ptPlayer = &player[1];
    }
    else if (ballPosition == BAR_DOWN)
    {
        ptBall->ptPlayer = &player[0];
    }

    else if (ballPosition == BRICK)
    {
        brickManager(ptBall, &ptBrick, &ptBrick);
    }

    bonusManager(bonusList, &bonus, bonusPosition);

    return alive;
}

// Parcours la liste chainee de balles
int runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player, BonusList* bonusList)
{
    int alive = LIFE_MAX;
    if (ballList == NULL)
        return -1;
    for(; ballList != NULL; ballList = ballList->next)
    {
        alive = ballManager(ballList, bar1, bar2, ptBrick, player, bonusList);
        if (!alive)
            return 0;
    }
    return alive;
}