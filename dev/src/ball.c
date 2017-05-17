#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <math.h>

#include "ball.h"
#include "collision.h"
#include "primitives.h"

#define BALL_RADIUS 10
#define BALL_SPEED 10

Ball* createBall (Point2D position, Vector2D direction, PtPlayer ptPlayer, GLuint* ballTexture)
{
    Ball* ball = malloc(sizeof(Ball));
    if (ball == NULL)
    {
        fprintf(stderr, "Echec de l'allocation de la balle.\n");
        exit(EXIT_FAILURE);
    }

    ball->position = position;
    ball->direction = direction;
    ball->radius = BALL_RADIUS;
    ball->speed = BALL_SPEED;
    ball->ptPlayer = ptPlayer;
    ball->next = NULL;
    ball->texture = ballTexture;

    return ball;
}

void addBall(PtBall* ballList, Ball* ball)
{
    if (*ballList == NULL)
        *ballList = ball;
    else
    {
        for (; (*ballList)->next != NULL; ballList = &((*ballList)->next));
            (*ballList)->next = ball;
    }
}

void drawBall(Ball ball)
{
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, ball.texture[ball.ptPlayer->num]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(ball.position.x, ball.position.y, 1);
        glScalef(ball.radius*2, ball.radius*2, 1);
        if (ball.ptPlayer->num == 0)
            glRotatef(180, 0.0, 0.0, 1.0);
        drawSquareTexture();
    glPopMatrix();
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawAllBalls(PtBall ballList)
{
    for (; ballList != NULL; ballList = ballList->next)
    {
        drawBall(*ballList);
    }
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

void deleteBalls(PtBall* ballList)
{
    PtBall next;
    for (; *ballList != NULL; ballList = &next)
    {
        next = (*ballList)->next;
        free(*ballList);
        *ballList = NULL;
    }
}