#include <stdio.h>
#include <GL/gl.h>
#include <math.h>

#include "ball.h"
#include "collision.h"
#include "primitives.h"

#define BALL_RADIUS 10
#define BALL_SPEED 10

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