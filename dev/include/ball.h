#ifndef __BALL__H
#define __BALL__H

#include "geometry.h"
#include "bar.h"
#include "brick.h"

/** STRUCTURES **/

typedef struct
{
  Point2D position;
  Vector2D direction;
  float radius;
  float speed;
  char* texture;
} Ball, *PtBall;

typedef enum Orientation
{
    HORIZONTAL, VERTICAL
} Orientation;

/** FUNCTIONS **/

Ball createBall (Point2D position, Vector2D direction);
void moveBall (PtBall ptBall);
void drawBall (Ball ball);

float ballBottomPosition (PtBall ptBall);
float ballTopPosition (PtBall ptBall);
float ballLeftPosition (PtBall ptBall);
float ballRightPosition (PtBall ptBall);

#endif