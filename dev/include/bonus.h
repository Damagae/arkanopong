#ifndef __BONUS__H
#define __BONUS__H

#include "player.h"

/** STRUCTURES **/

/** FUNCTIONS **/

void barSizeUp (PtBar ptBar);
void barSizeDown (PtBar ptBar);
void barSpeedUp (PtBar ptBar);
void ballSpeedUp (PtBall ptBall);
void ballSpeedDown (PtBall ptBall);
void ballSizeUp (PtBall ptBall);
Ball moreBall (Player* player);

#endif