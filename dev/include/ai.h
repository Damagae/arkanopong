#ifndef __AI__H
#define __AI__H

#include "bar.h"
#include "ball.h"

/** FUNCTIONS **/

Ball closestBall (PtBar ptBar, PtBall ballList);
void AIcontroller (PtBar bar, PtBall ballList);

#endif