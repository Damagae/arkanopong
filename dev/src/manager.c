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

void renderGame(Player player1, Player player2, Brick brick, BonusList bonusList)
{    
    glClear(GL_COLOR_BUFFER_BIT);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    glColor3f(1.0, 0.0, 0.0);
    drawGameBorder();

    glColor3f(1.0, 1.0, 1.0);
    drawBall(*(player1.p_ball));
    //drawBall(*(player2.p_ball));

    drawBar(*(player1.p_bar), player1.num);
    drawBar(*(player2.p_bar), player2.num);

    drawBrick(brick);
    drawAllBonus(bonusList);

    drawLife(player1);
    drawLife(player2);


    SDL_GL_SwapBuffers();
}

int runGame(PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player)
{
    int position;
    int alive = LIFE_MAX;
    moveBall(ptBall);
    position = checkPosition(ptBall, bar1, bar2, ptBrick, player);
    if (position == OUT_DOWN)
    {
        alive = loseLife(&player[0]);
    }
    else if (position == OUT_UP)
    {
        alive = loseLife(&player[1]);
    }
    else if (position == BRICK)
    {
        brickDamaged(ptBrick);
    }
    return alive;
}