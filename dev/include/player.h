#ifndef __PLAYER__H
#define __PLAYER__H

#include "ball.h"
#include "bar.h"

/** STRUCTURES **/

typedef struct
{
  int num;
  char* name;
  int life;
  PtBar p_bar;
  PtBall p_ball;
} Player;

typedef Player* PtPlayer;

/** FUNCTIONS **/

Player createPlayer(int num, char* name, PtBar ptBar, PtBall ptBall);

#endif