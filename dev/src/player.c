#include <GL/gl.h>

#include "stdio.h"
#include "player.h"
#include "primitives.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

extern int LIFE_MAX;

Player createPlayer(int num, char* name, PtBar ptBar)
{
    Player player;
    player.num = num;
    player.name = name;
    player.life = LIFE_MAX;
    player.ptBar = ptBar;

    return player;
}

void drawLife(Player player, Texture life)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);
    Point2D GAME_BOTTOM_RIGHT = PointXY(GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2);
    int i;
    
    glEnable(GL_BLEND);
    for (i = 0; i < LIFE_MAX; i++)
    {
        glPushMatrix(); 
        
        if (player.life >= i+1)
        {
            glBindTexture(GL_TEXTURE_2D, life.texture[0]);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            if (player.num == 0)
            {
                glTranslatef(GAME_TOP_LEFT.x + 20 + 50*i, GAME_BOTTOM_RIGHT.y + 40, 1);
            }
            else
            {
                glTranslatef(GAME_TOP_LEFT.x + 20 + 50*i, GAME_TOP_LEFT.y - 40, 1);
            }
            glScalef(40, 40, 1);
            glRotatef(180, 0.0, 0.0, 1.0);
            //drawCircle();
            drawSquareTexture();
        }
        else
        {
            //glBindTexture(GL_TEXTURE_2D, life.texture[1]);
        }
        glPopMatrix();
    }
    glDisable(GL_BLEND);
}

int loseLife(PtPlayer player)
{
    if (player->life > 0)
        (player->life)--;
    if (player->life == 0)
        return 0;
    else
        return player->life;
}