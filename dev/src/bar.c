#include "bar.h"

#define BAR_SPEED 1.0

Bar createBar (Point2D position, float width, float height)
{
    Bar bar;
    bar.position = position;
    bar.width = width;
    bar.height = height;
    bar.speed = BAR_SPEED;
   
    return bar;
}

void moveBar(PtBar ptBar, Direction direction)
{
    if(direction == RIGHT)
    {
        ptBar->position.x += ptBar->speed;
    }
    else if (direction == LEFT)
    {
        ptBar->position.x -= ptBar->speed;
    }
}