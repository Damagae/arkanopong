#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include "bonus.h"
#include "primitives.h"
#include "geometry.h"

#define BONUS_RADIUS 10
#define BONUS_SPEED 2

#define GAME_WIDTH 200
#define GAME_HEIGHT 400
#define BAR_SIZE_CHANGE 50
#define BALL_SIZE_CHANGE 1

Bonus* createBonus(PtBrick ptBrick, PtPlayer ptPlayer)
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
    bonus->ptPlayer = NULL;
    bonus->next = NULL;

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

// Draw only if brick is destroyed
void drawBonus(Bonus bonus)
{
    if (bonus.ptBrick->life == 0)
    {
        glPushMatrix();
        glTranslatef(bonus.position.x, bonus.position.y, 1);
        glScalef(bonus.radius, bonus.radius, 1);
        drawCircle();
        glPopMatrix();
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
    }
}

void barSizeUp (PtBar ptBar)
{
    ptBar->width += BAR_SIZE_CHANGE;
}

void barSizeDown (PtBar ptBar)
{
    ptBar->width -= BAR_SIZE_CHANGE;
}

void barSpeedUp (PtBar ptBar)
{
    ptBar->speed += 1;
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
    Vector2D vector;
    if (player->num == 1)
        vector = VectorXY(0,1);
    else
        vector = VectorXY(0,-1);
    addBall(ballList, createBall(PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), vector, player));
}

float bonusBottomPosition (Bonus bonus)
{
    return bonus.position.y + bonus.radius;
}

float bonusTopPosition (Bonus bonus)
{
    return bonus.position.y + bonus.radius;
}

float bonusLeftPosition (Bonus bonus)
{
    return bonus.position.x - bonus.radius;
}

float bonusRightPosition (Bonus bonus)
{
    return bonus.position.x + bonus.radius;
}

void deleteBonus(BonusList* bonusList, BonusList* bonus)
{
    if (bonusList == NULL)
        return;
    if (*bonusList == *bonus)
    {
        *bonusList = NULL;
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

void getBonus(Bonus bonus)
{
    if (bonus.ptBrick->type == BARUP)
    {
        barSizeUp(bonus.ptPlayer->ptBar);
    }
}