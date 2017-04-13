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

void renderGame(Player player1, Player player2, PtBall ballList, Brick brick, BonusList bonusList)
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

    drawBrick(brick);

    drawAllBonus(bonusList);

    drawLife(player1);
    drawLife(player2);


    SDL_GL_SwapBuffers();
}

int runGame(PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player, BonusList* bonusList)
{
    int ballPosition;
    int bonusPosition = INSIDE;
    int alive = LIFE_MAX;
    moveBall(ptBall);
    moveBonus(ptBrick->bonus);

    ballPosition = checkBallPosition(ptBall, bar1, bar2, ptBrick, player);

    if (ptBrick->bonus != NULL)
    {
        bonusPosition = checkBonusPosition(*(ptBrick->bonus), bar1, bar2);
    }
        


    if (ballPosition == OUT_DOWN)
    {
        alive = loseLife(&player[0]);
    }
    else if (ballPosition == OUT_UP)
    {
        alive = loseLife(&player[1]);
    }
    else if (ballPosition == BRICK)
    {
        if (brickDamaged(ptBrick) == 0)
        {
            bonusOrientation(ptBrick->bonus, *(ptBall->ptPlayer));
        }
    }

    if (bonusPosition == OUT_UP || bonusPosition == OUT_DOWN || bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
    {
        if (bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
            getBonus(*(ptBrick->bonus), *ptBall);
        deleteBonus(bonusList, &(ptBrick->bonus));
    }

    return alive;
}