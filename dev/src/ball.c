#include <stdio.h>
#include <GL/gl.h>
#include <math.h>

#include "ball.h"
#include "collision.h"
#include "primitives.h"

#define BALL_RADIUS 10
#define BALL_SPEED 10

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
int moveBall (PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick)
{
    Vector2D deplacement = MultVector(ptBall->direction, ptBall->speed);
    // Change ball position
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
    return checkPosition(ptBall, bar1, bar2, ptBrick);
}

// Check if the ball hits something or if it's out of the game
int checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick)
{
    int colBallBar, colBallBrick;
    int inside = 0;

    // 0 if no collision with the bar
    colBallBar = collisionBallBar(ptBall, bar1, bar2);
    colBallBrick = BrickCollision(*ptBrick, *ptBall);

    // If ball hits left or right border
    if (collisionBallWall(ptBall))
    {
        changeDirection(&(ptBall->direction), HORIZONTAL);
    }
    // If ball is out of the game
    if (ballTopPosition(ptBall) <= 0 || ballBottomPosition(ptBall) >= GAME_HEIGHT)
    {
        changeDirection(&(ptBall->direction), VERTICAL);
        inside = 0;
    }
    // If ball hits nothing & it's inside the game
    else
        inside = 1;

    // If ball hits bar1 (down)
    if (colBallBar == 1)
    {
        changeAngle(ptBall, bar1);
    }
    // If ball hits bar2 (up)
    else if (colBallBar == 2)
    {
        changeAngle(ptBall, bar2);
    }

    if (colBallBrick == 2 || colBallBrick == 4)
    {
        changeDirection(&(ptBall->direction), HORIZONTAL);
        inside = 2;
    }
    else if (colBallBrick == 1 || colBallBrick == 3)
    {
        changeDirection(&(ptBall->direction), VERTICAL);
        inside = 2;
    }

    return inside;
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