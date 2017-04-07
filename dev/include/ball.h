#ifndef __BALL__H
#define __BALL__H

#include "geometry.h"
#include "bar.h"

/** STRUCTURES **/

typedef struct
{
  Point2D position;
  Vector2D direction;
  float radius;
  float speed;
  char* texture;
} Ball;

typedef Ball* PtBall;

typedef enum Orientation
{
    HORIZONTAL, VERTICAL
} Orientation;

/** FUNCTIONS **/

Ball createBall (Point2D position, Vector2D direction);
int moveBall (PtBall ptBall, PtBar bar1, PtBar bar2);
int checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2);
void changeDirection (Vector2D* direction, Orientation orientation);
void drawBall (Ball ball);
int collisionBallWall (PtBall ptBall);
int collisionBallBar (PtBall ptBall, PtBar bar1, PtBar bar2);
void changeAngle (PtBall ptBall, PtBar ptBar);

float ballBottomPosition (PtBall ptBall);
float ballTopPosition (PtBall ptBall);
float ballLeftPosition (PtBall ptBall);
float ballRightPosition (PtBall ptBall);

#endif