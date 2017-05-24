#ifndef __BALL__H
#define __BALL__H

#include "geometry.h"
#include "bar.h"
#include "brick.h"
#include "player.h"

/** STRUCTURES **/

typedef struct ball
{
  Point2D position;
  Vector2D direction;
  float radius;
  float speed;
  GLuint* texture;
  PtPlayer ptPlayer;
  struct ball* next;
  Point2D ghost[5];
} Ball, *PtBall;

typedef enum Orientation
{
    HORIZONTAL, VERTICAL
} Orientation;

/** FUNCTIONS **/

Ball* createBall (Point2D position, Vector2D direction, PtPlayer ptPlayer, GLuint* ballTexture);
void addBall(PtBall* ballList, Ball* ball);
void moveBall (PtBall ptBall);
void drawBall (Ball ball);
void drawGhosts(Ball ball);
void drawAllBalls(PtBall ballList);

float ballBottomPosition (PtBall ptBall);
float ballTopPosition (PtBall ptBall);
float ballLeftPosition (PtBall ptBall);
float ballRightPosition (PtBall ptBall);

void deleteBalls(PtBall* ballList);

#endif