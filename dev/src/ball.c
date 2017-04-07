#include <stdio.h>
#include <GL/gl.h>
#include <math.h>

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
    // Change ball position
    ptBall->position = PointPlusVector(ptBall->position, deplacement);
    return checkPosition(ptBall, bar1, bar2);
}

// Check if the ball hits something or if it's out of the game
int checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2)
{
    int colBallBar;
    int inside = 0;

    // 0 if no collision
    colBallBar = collisionBallBar(ptBall, bar1, bar2);

    // If ball is out of the game
    if (ballTopPosition(ptBall) <= 0 || ballBottomPosition(ptBall) >= GAME_HEIGHT)
    {
        changeDirection(&(ptBall->direction), VERTICAL);
        inside = 1;
       // return 0;
    }

    // If ball hits left or right border
    if (collisionBallWall(ptBall))
    {
        changeDirection(&(ptBall->direction), HORIZONTAL);
        inside = 1;
    }

    // If ball hits bar1 (down)
    else if (colBallBar == 1)
    {
        changeAngle(ptBall, bar1);
        inside = 1;
    }

    // If ball hits bar2 (up)
    else if (colBallBar == 2)
    {
        changeAngle(ptBall, bar2);
        inside = 1;
    }

    // If ball hits nothing & it's inside the game
    else
        inside = 1;

    return inside;
}

void changeDirection (Vector2D* direction, Orientation orientation)
{
    if (orientation == HORIZONTAL)
    {
        direction->x = inverse(direction->x);
    }
    else if (orientation == VERTICAL)
    {
        direction->y = inverse(direction->y);
    }
}

// Change ball's direction. The more you are close to the bar center, the more vertical it will be
void changeAngle (PtBall ptBall, PtBar ptBar)
{
    float dist;
    float angle;

    dist = ptBall->position.x - ptBar->position.x;
    // To avoid bugs if you move the bar during collision
    if (dist > ptBar->width/2)
        dist = ptBar->width/2;
    else if (dist < inverse(ptBar->width/2))
        dist = inverse(ptBar->width/2);

    // Value between 0 and M_PI/3
    angle = dist/(ptBar->width/2) * M_PI/3;

    ptBall->direction.x = sin(angle);
    if (ptBall->direction.y > 0)
        ptBall->direction.y = -cos(angle);
    else
        ptBall->direction.y = cos(angle);
}

int collisionBallWall(PtBall ptBall)
{
    if (ballLeftPosition(ptBall) <= 0 || ballRightPosition(ptBall) >= GAME_WIDTH)
        return 1;
    else
        return 0;
}

/* Return 1 if collision with bar1, 2 if collision with bar2, 0 if no collision */
int collisionBallBar(PtBall ptBall, PtBar bar1, PtBar bar2)
{
    // Bar 1 collision between the center and the top of the bar
    if (ballBottomPosition(ptBall) >= barTopPosition(bar1) && ballBottomPosition(ptBall) <= bar1->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar1) && ballLeftPosition(ptBall) <= barRightPosition(bar1))
            return 1;
        else return 0;
    }

    // Bar 2 is rotated 180°
    else if (ballTopPosition(ptBall) <= barBottomPosition(bar2) && ballTopPosition(ptBall) >= bar2->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar2) && ballLeftPosition(ptBall) <= barRightPosition(bar2))
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