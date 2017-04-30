#include "ai.h"
#include <stdio.h>

Ball closestBall (PtBar ptBar, PtBall ballList)
{
    Ball closest = *ballList;
    float coordinate = ballList->position.y;
    for (; ballList != NULL; ballList = (*ballList).next)
    {
        if ((*ballList).position.y <= coordinate)
        {
            coordinate = (*ballList).position.y;
            closest = *ballList;
        }
    }
            
    return closest;
}

void AIcontroller (PtBar ptBar, PtBall ballList)
{
    Ball ball = closestBall(ptBar, ballList);
    if (ball.position.x < ptBar->position.x)
        moveBar(ptBar, LEFT);
    else if (ball.position.x > ptBar->position.x)
        moveBar(ptBar, RIGHT);
}