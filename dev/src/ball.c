#include <GL/gl.h>

#include "ball.h"
#include "bar.h"
#include "primitives.h"

#define BALL_RADIUS 10
#define BALL_SPEED 5

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

Ball createBall (Point2D position, Vector2D direction)
{
    Ball ball;
    ball.position = position;
    ball.direction = direction;
    ball.radius = BALL_RADIUS;
    ball.speed = BALL_SPEED;
   
    return ball;
}

void drawBall(Ball ball)
{
    glPushMatrix();
    glTranslatef(ball.position.x, ball.position.y, 1);
    glScalef(ball.radius, ball.radius, 1);
    drawCircle();
    glPopMatrix();
}

/* BALL MANAGMENT */

// Return 1 if ball inside game
// Return 0 if ball outside game
int moveBall (PtBall ptBall, PtBar bar1, PtBar bar2)
{
    Vector2D deplacement = MultVector(ptBall->direction, ptBall->speed);
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
    return checkPosition(ptBall, bar1, bar2);
}

int moveBallBrick (PtBall ptBall)
{
    Vector2D deplacement = MultVector(ptBall->direction, ptBall->speed);
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
    changeDirection(&(ptBall->direction), HORIZONTAL);
    return 1;
}

int checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2)
{
    // If ball hit left or right border
    if (collisionBallWall(ptBall))
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
    else if (collisionBallBar(ptBall, bar1, bar2))
    {
        changeDirection(&(ptBall->direction), VERTICAL);
        return 1;
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

void changeAngle (PtBall ptBall, PtBar ptBar)
{
    if (ptBall->position.x <= ptBar->position.x)
    {
       return ; 
    }
}

int collisionBallWall(PtBall ptBall)
{
    if (ballRightPosition(ptBall) == GAME_WIDTH || ballLeftPosition(ptBall) == 0)
        return 1;
    else
        return 0;
}

/* Return 1 if collision with bar1, 2 if collision with bar2, 0 if no collision */
int collisionBallBar(PtBall ptBall, PtBar bar1, PtBar bar2)
{
    if (ballBottomPosition(ptBall) == barTopPosition(bar1))
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar1) && ballLeftPosition(ptBall) <= barRightPosition(bar1))
            return 1;
        else return 0;
    }
    /* Bar 2 is rotated 180° */
    
    else if (ballTopPosition(ptBall) == barBottomPosition(bar2))
    {
        if (ballLeftPosition(ptBall) >= barLeftPosition(bar2) && ballRightPosition(ptBall) <= barRightPosition(bar2))
            return 2;
        else return 0;
    }
    else
        return 0;
}

/* Ball edges position */

float ballBottomPosition (PtBall ptBall)
{
    return ptBall->position.y + ptBall->radius;
}

float ballTopPosition (PtBall ptBall)
{
    return ptBall->position.y - ptBall->radius;
}

float ballLeftPosition (PtBall ptBall)
{
    return ptBall->position.x - ptBall->radius;
}

float ballRightPosition (PtBall ptBall)
{
    return ptBall->position.x + ptBall->radius;
}