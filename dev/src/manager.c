#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>

#include "manager.h"
#include "primitives.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

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

void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2)
{
    Bonus bonus;
    BonusList ptBonus = (*bonusList);
    
    for(; ptBonus != NULL; ptBonus = ptBonus->next)
    {
        bonus  = *ptBonus;
        if (ptBonus->actif)
        {
            Position bonusPosition = INSIDE;
            moveBonus(ptBonus);
            bonusPosition = checkBonusPosition(bonus, bar1, bar2);

            if (bonusPosition == OUT_UP || bonusPosition == OUT_DOWN || bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
            {
                if (bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
                {
                    getBonus(bonus);
                    ptBonus->actif = false;
                }   
                //deleteBonus(bonusList, &ptBonus);
            }
        }
    }
}

int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick)
{
    if (ptBrick == NULL)
        return -1;
    if (brickDamaged(ptBrick) == 0)
    {
        ptBrick->bonus->ptPlayer = ptBall->ptPlayer;
        ptBrick->bonus->actif = true;
        bonusOrientation(ptBrick->bonus, *(ptBall->ptPlayer));
        //deleteBrick(brickList, ptBrick);
        return 0;
    }
    else
        return ptBrick->life;
}

Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player)
{
    Position ballPosition;

    ballPosition = checkBallPosition(ballList, bar1, bar2, ptBrick, player);
    
    if (ptBrick == NULL)
    {
        if (ballPosition == OUT_DOWN)
        {
            loseLife(&player[0]);
        }
        else if (ballPosition == OUT_UP)
        {
            loseLife(&player[1]);
        }

        // Change the ball owner
        else if (ballPosition == BAR_UP)
        {
            ballList->ptPlayer = &player[1];
        }
        else if (ballPosition == BAR_DOWN)
        {
            ballList->ptPlayer = &player[0];
        }
    }

    else if (ptBrick != NULL)
    {
        if (ballPosition == BRICK)
        {
            brickManager(ballList, brickList, ptBrick);
        }
    }

    return ballPosition;
}

Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player)
{
    Position ballPosition = INSIDE;
    PtBrick ptBrick;

    for(; ballList != NULL; ballList = ballList->next)
    {
        
        moveBall(ballList);
        
        ballPosition = positionDetection(ballList, bar1, bar2, brickList, NULL, player);
        // If the ball hit something, then stop
        if(ballPosition == OUT_UP || ballPosition == OUT_DOWN || ballPosition == BAR_UP ||ballPosition == BAR_DOWN || ballPosition == WALL)
            return ballPosition;
        
        ptBrick = *brickList;
        for(; ptBrick != NULL; ptBrick = ptBrick->next)
        {
            ballPosition = positionDetection(ballList, bar1, bar2, brickList, ptBrick, player);
            if(ballPosition == BRICK)
                return ballPosition;
        }     
    }
    
    return ballPosition;
}

// Parcours la liste chainee de balles
Position runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, BonusList* bonusList)
{
    Position ballPosition;

    // If no ball then exit function
    if (ballList == NULL)
        return -1;
    
    ballPosition = ballManager(ballList, bar1, bar2, brickList, player);
    bonusManager(bonusList, bar1, bar2);

    return ballPosition;
}