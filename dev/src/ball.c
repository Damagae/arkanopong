#include "ball.h"

#define BALL_SPEED 1.0
#define SEGMENTS 32
#define GAME_WIDTH 200
#define GAME_HEIGHT 400

Ball createBall (Point2D position, Vector2D direction)
{
    Ball ball;
    ball.position = position;
    ball.direction = direction;
    ball.speed = BALL_SPEED;
   
    return ball;
}

// Return 1 if ball inside game
// Return 0 if ball outside game
int moveBall (PtBall ptBall)
{
    Vector2D deplacement = MultVector(ptBall->direction, ptBall->speed);
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
    return checkPosition(ptBall);
}

int checkPosition (PtBall ptBall)
{
    // If ball hit left or right border
    if (ptBall->position.x + ptBall->radius == GAME_WIDTH || ptBall->position.y - ptBall->radius == 0)
    {
        changeDirection(&(ptBall->direction), HORIZONTAL);
        return 1;
    }
    else if (ptBall->position.y- ptBall->radius == 0 || ptBall->position.y + ptBall->radius == GAME_HEIGHT)
    {
        changeDirection(&(ptBall->direction), VERTICAL);
        return 1;
       // return 0;
    }
    else
        return 1;
}

void changeDirection (Vector2D* direction, Orientation orientation)
{
    if (orientation == HORIZONTAL)
    {
        direction->x = direction->x*-1;
    }
    else
    {
        direction->y = direction->y*-1;
    }
}

void drawBall(Ball ball)
{
   return;
}