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
void renderGame(Player player1, Player player2, PtBall ballList, PtBrick brickList, BonusList bonusList);
int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick);
void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2);
Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player);
Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player);
Position runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, BonusList* bonusList);

#endif