#ifndef __MANAGER__H
#define __MANAGER__H

#include "player.h"
#include "ball.h"
#include "bar.h"
#include "brick.h"
#include "collision.h"
#include "bonus.h"

/** STRUCTURES **/


/** FONCTIONS **/

int loseLife(PtPlayer player);
void drawGameBorder();
void renderGame(Player player1, Player player2, PtBall ballList, Brick brick, BonusList bonusList);
int runBall(PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player, BonusList* bonusList);
int runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer player, BonusList* bonusList);

#endif