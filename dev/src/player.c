#include <GL/gl.h>

#include "player.h"
#include "primitives.h"

#define LIFE_MAX 3

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

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
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);
    Point2D GAME_BOTTOM_RIGHT = PointXY(GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2);
    int i;

    for (i = 0; i < LIFE_MAX; i++)
    {
        glPushMatrix();
        if (player.life >= i+1)
        {
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glColor3f(1.0, 1.0, 1.0);
        }
        if (player.num == 0)
        {
            glTranslatef(GAME_TOP_LEFT.x + 20 + 40*i, GAME_BOTTOM_RIGHT.y + 20, 1);
        }
        else
        {
            glTranslatef(GAME_TOP_LEFT.x + 20 + 40*i, GAME_TOP_LEFT.y - 20, 1);
        }
        glScalef(10, 10, 1);
        drawCircle();
        glPopMatrix();
    }
}