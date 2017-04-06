#ifndef __BALL__H
#define __BALL__H

#include "geometry.h"

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
int moveBall (PtBall ptBall);
int checkPosition (PtBall ptBall);
void changeDirection (Vector2D* direction, Orientation orientation);
void drawCircle();
void drawBall(Ball ball);

#endif