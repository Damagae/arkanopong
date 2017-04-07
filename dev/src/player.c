#include "player.h"

#define LIFE_MAX 3

Player createPlayer(int num, char* name, PtBar ptBar, PtBall ptBall)
{
    Player player;
    player.num = num;
    player.name = name;
    player.life = LIFE_MAX;
    player.p_bar = ptBar;
    player.p_ball = ptBall;

    return player;
}

void drawLife(Player player)
{
    if (player.num == 0)
    {

    }
    else
    {
        
    }
}