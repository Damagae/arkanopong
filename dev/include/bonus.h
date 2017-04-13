#ifdef __BONUS__FORWARD__DECLARATION
#undef __BONUS__FORWARD__DECLARATION

struct bonus;
typedef struct bonus Bonus, *BonusList;

#else

#ifndef __BONUS__H
#define __BONUS__H

#include "geometry.h"
#include "player.h"
#include "brick.h"

/** STRUCTURES **/

typedef struct bonus
{
  Point2D position;
  Vector2D direction;
  float radius;
  float speed;
  char* texture;
  PtBrick ptBrick;
  struct bonus* next;
} Bonus, *BonusList;

/** FUNCTIONS **/

Bonus* createBonus(PtBrick ptBrick);
void addBonus(BonusList* bonusList, Bonus* bonus);
void bonusOrientation(Bonus* bonus, Player player);
void drawBonus(Bonus bonus);
void drawAllBonus(BonusList bonusList);

void barSizeUp (PtBar ptBar);
void barSizeDown (PtBar ptBar);
void barSpeedUp (PtBar ptBar);
void ballSpeedUp (PtBall ptBall);
void ballSpeedDown (PtBall ptBall);
void ballSizeUp (PtBall ptBall);
Ball moreBall (Player* player);

#endif
#endif