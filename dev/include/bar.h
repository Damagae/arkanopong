#ifndef __BAR__H
#define __BAR__H

#include "geometry.h"
#include "textures.h"

/** STRUCTURES **/

typedef struct
{
    Point2D position;
    float width;
    float height;
    float speed;
    GLuint texture;
} Bar, *PtBar;

typedef enum Direction
{
    LEFT, RIGHT, UP, DOWN, NONE
} Direction;

/** FUNCTIONS **/

Bar createBar (Point2D position, GLuint texture);
void moveBar(PtBar ptBar, Direction direction);
void drawBar(Bar bar, int numPlayer);

float barTopPosition(PtBar ptBar);
float barBottomPosition(PtBar ptBar);
float barLeftPosition(PtBar ptBar);
float barRightPosition(PtBar ptBar);

#endif