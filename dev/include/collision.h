#ifndef __COLLISION__H
#define __COLLISION__H

#include "geometry.h"
#include "ball.h"
#include "bar.h"
#include "bool.h"
#include "brick.h"

/** STRUCTURES **/


/** FUNCTIONS **/
bool CollisionPointCercle (Point2D A, Ball ball);
bool CollisionDroite (Point2D A, Point2D B, Ball ball);
bool CollisionSegment (Point2D A,Point2D B, Ball ball);
int BrickCollision (Brick brick, Ball ball);
int collisionBallWall (PtBall ptBall);
int ballOutOfGame(PtBall ptBall);
int collisionBallBar (PtBall ptBall, PtBar bar1, PtBar bar2);
void changeDirection (Vector2D* direction, Orientation orientation);
void changeAngle (PtBall ptBall, PtBar ptBar);


#endif