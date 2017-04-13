#ifndef __PLAYER__H
#define __PLAYER__H

#include "bar.h"

/** STRUCTURES **/

typedef struct
{
  int num;
  char* name;
  int life;
  PtBar ptBar;
} Player, *PtPlayer;

/** FUNCTIONS **/

Player createPlayer(int num, char* name, PtBar ptBar);
void drawLife(Player player);

#endif