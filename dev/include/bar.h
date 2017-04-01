#ifndef __BAR__H
#define __BAR__H

#include "geometry.h"

/** STRUCTURES **/

typedef struct
{
    Point2D position;
    float width;
    float height;
    float speed;
    char* texture;
} Bar;

typedef Bar* PtBar;

typedef enum Direction
{
    LEFT, RIGHT
} Direction;

/** FUNCTIONS **/

Bar createBar (Point2D position, float width, float height);
void moveBar(PtBar ptBar, Direction direction);

#endif