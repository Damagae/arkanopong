#include "bonus.h"

#define GAME_WIDTH 200
#define GAME_HEIGHT 400
#define BAR_SIZE_CHANGE 6
#define BALL_SIZE_CHANGE 0.5

void barSizeUp (PtBar ptBar)
{
    ptBar->width += BAR_SIZE_CHANGE;
}

void barSizeDown (PtBar ptBar)
{
    ptBar->width -= BAR_SIZE_CHANGE;
}

void barSpeedUp (PtBar ptBar)
{
    ptBar->speed += 1;
}

void ballSpeedUp (PtBall ptBall)
{
    ptBall->speed += 1;
}

void ballSpeedDown (PtBall ptBall)
{
    ptBall->speed = ptBall->speed/2;
}

void ballSizeUp (PtBall ptBall)
{
    ptBall->radius += BALL_SIZE_CHANGE;
}

Ball moreBall (Player* player)
{
    if (player->num == 1)
        return createBall(PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), VectorXY(0, 1));
    else
        return createBall(PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), VectorXY(0,-1));
}