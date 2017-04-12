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

// Change ball position
void moveBall (PtBall ptBall)
{
    Vector2D deplacement = MultVector(ptBall->direction, ptBall->speed);
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
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