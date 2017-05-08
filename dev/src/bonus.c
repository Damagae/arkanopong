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

#define LIFE_MAX 3

/** CREATE FUNCTIONS **/

Bonus* createBonus(PtBrick ptBrick, TextureList* bonusTexture, char* textureFile)
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
    bonus->ptTexture = addTexture(bonusTexture, textureFile);
    // We don't need this, but if we delete it, it bugs, don't know why
    bonus->ptBrick = ptBrick;
    bonus->type = ptBrick->type;
    bonus->ptPlayer = NULL;
    bonus->actif = false;
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
        glBindTexture(GL_TEXTURE_2D, bonus.ptTexture->texture[bonus.ptTexture->num]);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glTranslatef(bonus.position.x, bonus.position.y, 1);
            glScalef(bonus.radius*2, bonus.radius*2, 1);
            if (bonus.direction.y <= 0)
                glRotatef(180, 0.0, 0.0, 1.0);
            drawSquareTexture();
        glPopMatrix();
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
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
    ptBar->speed += 5;
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
    addBall(ballList, createBall(PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), vector, player, &(*ballList)->ptTexture));
}

void addLife (Player* player)
{
    if (player->life < LIFE_MAX)
        ++(player->life);
}

void getBonus(Bonus bonus)
{
    if (bonus.type == BARUP)
    {
        barSizeUp(bonus.ptPlayer->ptBar);
    }
    else if (bonus.type == BARDWN)
    {
        barSizeDown(bonus.ptPlayer->ptBar);
    }
    else if (bonus.type == BARSPDUP)
    {
        barSpeedUp(bonus.ptPlayer->ptBar);
    }
    else if (bonus.type == ADDLIFE)
    {
        addLife(bonus.ptPlayer);
    }
}