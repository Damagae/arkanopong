#include "ai.h"

void AIcontroller (PtBar ptBar, Ball ball)
{
    if (ball.position.x < ptBar->position.x)
        moveBar(ptBar, LEFT);
    else if (ball.position.x > ptBar->position.x)
        moveBar(ptBar, RIGHT);
}