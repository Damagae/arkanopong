#ifndef __PLAYER__H
#define __PLAYER__H

#include "bar.h"
#include "textures.h"

/** STRUCTURES **/

typedef enum Power
{
    OFF, SLOW, FAST
} Power;

typedef struct
{
  int num;
  char* name;
  int life;
  PtBar ptBar;
  Power power;
  int gauge;
} Player, *PtPlayer;

/** FUNCTIONS **/

Player createPlayer(int num, char* name, PtBar ptBar);
void drawLife(Player player, GLuint lifeTexture);
int loseLife(PtPlayer player);
void drawNames(char* name1, char* name2);
void drawWinner(Player player1, Player player2);

#endif