#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include "bonus.h"
#include "primitives.h"
#include "geometry.h"

#define BONUS_RADIUS 10
#define BONUS_SPEED 2

#define BAR_SIZE_CHANGE 25
#define BALL_SIZE_CHANGE 1

extern int LIFE_MAX;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

/** CREATE FUNCTIONS **/

Bonus* createBonus(PtBrick ptBrick, GLuint* bonusTexture)
{
    Bonus* bonus = malloc(sizeof(Bonus));
    if (bonus == NULL)
    {
        fprintf(stderr, "Echec de l'allocation du bonus.\n");
        exit(EXIT_FAILURE);
    }

    bonus->position = ptBrick->position;
    bonus->direction = VectorXY(0,0);
    bonus->radius = BONUS_RADIUS;
    bonus->speed = BONUS_SPEED;
    bonus->ptBrick = ptBrick;
    bonus->type = ptBrick->type;

    int numBonus = 2*selectBonus(bonus->type);
    bonus->texture = bonusTexture[numBonus];
    bonus->textureTxt = bonusTexture[numBonus+1];
    // We don't need this, but if we delete it, it bugs, don't know why

    bonus->ptPlayer = NULL;
    bonus->actif = false;
    bonus->next = NULL;
    bonus->animateTxt = 0;

    return bonus;
}

void addBonus(BonusList* bonusList, Bonus* bonus)
{
    if (*bonusList == NULL)
        *bonusList = bonus;
    else
    {
        for (; (*bonusList)->next != NULL; bonusList = &((*bonusList)->next));
        (*bonusList)->next = bonus;
    }
}

int selectBonus(BrickType type)
{
    switch(type)
    {
        case BARUP:
            return 0;
        case BARDWN:
            return 1;
        case BARSPDUP:
            return 2;
        case SLOWPOW:
            return 3;
        case FASTPOW:
            return 4;
        case ADDBALL:
            return 5;
        case ADDLIFE:
            return 6;
        default :
            return 0;
    }
}

/** MOVING FUNCTIONS **/

void bonusOrientation(Bonus* bonus, Player player)
{
    if (player.num == 0)
    {
        bonus->direction = VectorXY(0,1);
    }
    else
    {
        bonus->direction = VectorXY(0,-1);
    }
}

// Change bonus position
void moveBonus (Bonus* bonus)
{
    if (bonus != NULL)
    {
        Vector2D deplacement = MultVector(bonus->direction, bonus->speed);
        bonus->position = PointPlusVector(bonus->position, deplacement);
    }
}

/** DRAWING FUNCTIONS **/

// Draw only if brick is destroyed
void drawBonus(Bonus bonus)
{
    if (bonus.actif)
    {
        glColor3f(1.0, 1.0, 1.0);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, bonus.texture);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glTranslatef(bonus.position.x, bonus.position.y, 1);
            if (bonus.type ==  BARSPDUP)
                glScalef(bonus.radius*6, -bonus.radius*2, 1);
            else if (bonus.type == BARUP || bonus.type == BARDWN)
                glScalef(bonus.radius*4, -bonus.radius*2, 1);
            else
                glScalef(bonus.radius*3, -bonus.radius*3, 1);
            drawSquareTexture();
        glPopMatrix();
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void drawBonusText(Bonus* bonus)
{
    if (bonus->animateTxt > 0)
    {
        float animate = (float)(bonus->animateTxt/200.0);
        glColor4f(1.0, 1.0, 1.0, animate);
        glEnable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, bonus->textureTxt);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            if (bonus->ptPlayer->num ==  0)
                glTranslatef(WINDOW_WIDTH/2, GAME_HEIGHT-200+bonus->animateTxt, 1);
            else
                glTranslatef(WINDOW_WIDTH/2, 200+200-bonus->animateTxt, 1);
            if (bonus->type != FASTPOW)
                glScalef(100, -30, 1);
            else
                glScalef(130, -30, 1);
            drawSquareTexture();
        glPopMatrix();
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        --(bonus->animateTxt);
    }
}

void drawAllBonus(BonusList bonusList)
{
    if (bonusList == NULL)
    {
        return;
    }

    for (; bonusList != NULL; bonusList = bonusList->next)
    {
        drawBonus(*bonusList);
        drawBonusText(bonusList);
    }
}

/** UTILITIES FUNCTIONS **/

float bonusBottomPosition (Bonus bonus)
{
    return bonus.position.y + bonus.radius;
}

float bonusTopPosition (Bonus bonus)
{
    return bonus.position.y - bonus.radius;
}

float bonusLeftPosition (Bonus bonus)
{
    return bonus.position.x - bonus.radius;
}

float bonusRightPosition (Bonus bonus)
{
    return bonus.position.x + bonus.radius;
}

/** DELETE FUNCTIONS **/

// This function doesn't work well
void deleteBonus(BonusList* bonusList, BonusList* bonus)
{
    if (bonusList == NULL)
        return;
    if (*bonusList == *bonus)
    {
        *bonusList = (*bonus)->next;
        free(*bonus);
        *bonus = NULL;
        return ;
    }
    Bonus* next;
    for (; (*bonusList)->next != NULL && (*bonusList)->next != *bonus ; bonusList = &next)
    {
        next = (*bonusList)->next;
    }

    (*bonusList)->next = (*bonus)->next;
    free(*bonus);
    *bonus = NULL;
}

void deleteBonusList(BonusList* bonusList)
{
    if (bonusList == NULL)
        return;
    Bonus* next;
    for (; *bonusList != NULL; bonusList = &next)
    {
        next = (*bonusList)->next;
        free(*bonusList);
        *bonusList = NULL;
    }
}

/** BONUS EFFECT FUNCTIONS **/

void barSizeUp (PtBar ptBar, GLuint* texture, int numPlayer)
{
    if (ptBar->width < 100+2*BAR_SIZE_CHANGE)
    {
        ptBar->width += BAR_SIZE_CHANGE;
        int i;
        for (i = -2; i <=2; i++)
        {
            if (ptBar->width == 100+(BAR_SIZE_CHANGE*i))
                ptBar->texture = texture[4+numPlayer+2*i];
        }
    }
}

void barSizeDown (PtBar ptBar, GLuint* texture, int numPlayer)
{
    if (ptBar->width > (100-2*BAR_SIZE_CHANGE))
    {
        ptBar->width -= BAR_SIZE_CHANGE;
        int i;
        for (i = -2; i <=2; i++)
        {
            if (ptBar->width == 100+BAR_SIZE_CHANGE*i)
                ptBar->texture = texture[4+numPlayer+2*i];
        }
    }
}

void barSpeedUp (PtBar ptBar)
{
    ptBar->speed += 5;
    if (ptBar->speed > 15)
        ptBar->speed = 15;
}

void ballSpeedUp (PtBall ptBall)
{
    ptBall->speed += 1;
}

void ballSpeedDown (PtBall ptBall)
{
    ptBall->speed = ptBall->speed/2;
}

void ballSizeUp (PtBall ptBall)
{
    ptBall->radius += BALL_SIZE_CHANGE;
}

void moreBall (PtBall* ballList, Player* player)
{
    Point2D position;
    if (player->num == 1)
    {
        position.x = player->ptBar->position.x;
        position.y = player->ptBar->position.y + player->ptBar->height/2;
    }
        
    else
    {
        position.x = player->ptBar->position.x;
        position.y = player->ptBar->position.y - player->ptBar->height/2;
    }
    addBall(ballList, createBall(position, VectorXY(0,0), player, (*ballList)->texture));
}

void addLife (Player* player)
{
    if (player->life < LIFE_MAX)
        ++(player->life);
}

void slowPower(Player* player)
{
    player->power = SLOW;
    player->gauge = 100;
}

void fastPower(Player* player)
{
    player->power = FAST;
    player->gauge = 100;
}

void getBonus(Bonus bonus, PtBall* ballList, GLuint* barTexture)
{
    if (bonus.type == BARUP)
    {
        barSizeUp(bonus.ptPlayer->ptBar, barTexture, bonus.ptPlayer->num);
    }
    else if (bonus.type == BARDWN)
    {
        barSizeDown(bonus.ptPlayer->ptBar, barTexture, bonus.ptPlayer->num);
    }
    else if (bonus.type == BARSPDUP)
    {
        barSpeedUp(bonus.ptPlayer->ptBar);
    }
    else if (bonus.type == ADDLIFE)
    {
        addLife(bonus.ptPlayer);
    }
    else if (bonus.type == ADDBALL)
    {
        moreBall(ballList, bonus.ptPlayer);
    }
    else if (bonus.type == SLOWPOW)
    {
        slowPower(bonus.ptPlayer);
    }
    else if (bonus.type == FASTPOW)
    {
        fastPower(bonus.ptPlayer);
    }
}