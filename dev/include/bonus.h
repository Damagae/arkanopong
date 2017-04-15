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
  BrickType type;
  PtPlayer ptPlayer;
  bool actif;
  struct bonus* next;
} Bonus, *BonusList;

/** FUNCTIONS **/

Bonus* createBonus(PtBrick ptBrick);
void addBonus(BonusList* bonusList, Bonus* bonus);
void bonusOrientation(Bonus* bonus, Player player);
void moveBonus (Bonus* bonus);
void drawBonus(Bonus bonus);
void drawAllBonus(BonusList bonusList);

void barSizeUp (PtBar ptBar);
void barSizeDown (PtBar ptBar);
void barSpeedUp (PtBar ptBar);
void ballSpeedUp (PtBall ptBall);
void ballSpeedDown (PtBall ptBall);
void ballSizeUp (PtBall ptBall);
void moreBall (PtBall* ballList, Player* player);
void addLife (Player* player);

void getBonus(Bonus bonus);

float bonusBottomPosition (Bonus bonus);
float bonusTopPosition (Bonus bonus);
float bonusLeftPosition (Bonus bonus);
float bonusRightPosition (Bonus bonus);

void deleteBonus(BonusList* bonusList, BonusList* bonus);
void deleteBonusList(BonusList* bonusList);

#endif
#endif