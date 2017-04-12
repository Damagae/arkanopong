#ifndef __MANAGER__H
#define __MANAGER__H

#include "player.h"
#include "ball.h"
#include "bar.h"
#include "brick.h"
#include "collision.h"

/** STRUCTURES **/


/** FONCTIONS **/

int loseLife(PtPlayer player);
void drawGameBorder();
void renderGame(Player player1, Player player2, Brick brick);
int runGame(PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player);

#endif