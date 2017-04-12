#include <GL/gl.h>

#include "manager.h"
#include "primitives.h"
#include "player.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

int loseLife(PtPlayer player)
{
    (player->life)--;
    if (player->life == 0)
        return 0;
    else
        return player->life;
}

void drawGameBorder()
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
    glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
    drawSquareBorder();
    glPopMatrix();
}

void render(Player player1, Player player2)
{
    drawBall(*(player1.p_ball));
    drawBall(*(player2.p_ball));

    drawBar(*(player1.p_bar), player1.num);
    drawBar(*(player2.p_bar), player2.num);

    drawLife(player1);
    drawLife(player2);
}