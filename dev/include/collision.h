#ifndef __COLLISION__H
#define __COLLISION__H

#include "geometry.h"
#include "player.h"
#include "bool.h"
#include "brick.h"

/** STRUCTURES **/
typedef enum Position
{
    INSIDE, OUT_UP, OUT_DOWN, WALL, BAR_UP, BAR_DOWN, BRICK
} Position;

/** FUNCTIONS **/
bool CollisionPointCercle (Point2D A, Ball ball);
bool CollisionDroite (Point2D A, Point2D B, Ball ball);
bool CollisionSegment (Point2D A,Point2D B, Ball ball);
int BrickCollision (Brick brick, PtBall ptBall);
int collisionBallWall (PtBall ptBall);
Position ballOutOfGame(PtBall ptBall);
int collisionBallBar (PtBall ptBall, PtBar bar1, PtBar bar2);
void changeDirection (Vector2D* direction, Orientation orientation);
void changeAngle (PtBall ptBall, PtBar ptBar);
Position checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer ptPlayer);


#endif