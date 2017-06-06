#include <stdio.h>

#include "collision.h"
#include "math.h"
#include "brick.h"
#include "bonus.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

bool CollisionPointCercle (Point2D A, Ball ball)
{
    Point2D C = ball.position;
    int d2 = (A.x-C.x)*(A.x-C.x) + (A.y-C.y)*(A.y-C.y);
    if (d2>ball.radius*ball.radius)
    {
        return false;
    }
    else 
    {
        return true;
    }
}

bool CollisionDroite (Point2D A, Point2D B, Ball ball)
{
    Vector2D u = Vector(A, B);
    Point2D C = ball.position;
    Vector2D AC = Vector(A, C);
    float numerateur = u.x*AC.y - u.y*AC.x;   // AC vector norm
    if (numerateur <0)
        numerateur = -numerateur ;
    float denominateur = sqrt(u.x*u.x + u.y*u.y);  // AB vector norm
    float CI = numerateur / denominateur;
    
    if (CI<ball.radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionSegment (Point2D A,Point2D B, Ball ball)
{
    Point2D C = ball.position;
    if (CollisionDroite(A,B,ball) == false)
        return false;  // if there is collision with the straight line, there won't be with the segment

    Vector2D AB,AC, BC;
    AB = Vector(A, B);
    AC = Vector(A, C);
    BC = Vector(B, C);
    float pscal1 = DotProduct(AB, AC);
    float pscal2 = DotProduct(MultVector(AB, -1), BC);

    if (pscal1>=0 && pscal2>=0)
    {
        return true;   // I between A and B
    }
    // Last possibility, A or B in the circle
    if (CollisionPointCercle(A,ball))
    {
        return true;
    }
    if (CollisionPointCercle(B,ball))
    {
        return true;
    }
    return false;
}

int BrickCollision (Brick brick, PtBall ptBall)
{
    if (brick.life == 0)
    {
        return 0;
    }
    else
    {
        Point2D A = brickVerticeTopLeft(&brick);
        Point2D B = brickVerticeTopRight(&brick);
        Point2D C = brickVerticeBottomRight(&brick);
        Point2D D = brickVerticeBottomLeft(&brick);

        bool AB, BC, CD, DA;
        AB = CollisionSegment(A, B, *ptBall);
        BC = CollisionSegment(B, C, *ptBall);
        CD = CollisionSegment(C, D, *ptBall);
        DA = CollisionSegment(D, A, *ptBall);

        if (AB == true || BC == true || CD == true || DA == true)
        {
            if (AB == true ) {
                // We put ball outside the brick
                ptBall->position.y = brick.position.y - brick.height/2 - ptBall->radius;
                return 1;
            }
            else if (CD == true ) {
                ptBall->position.y = brick.position.y + brick.height/2 + ptBall->radius;
                return 3;
            }
            else if (BC == true ) {
                ptBall->position.x = brick.position.x + brick.width/2 + ptBall->radius;
                return 2;
            }
            else {
                ptBall->position.x = brick.position.x - brick.width/2 - ptBall->radius;
                return 4;
            }
        }    
        else
        {
            return -1;
        }
    }
}

int collisionBallWall(PtBall ptBall)
{
    float LEFT_BORDER = (WINDOW_WIDTH-GAME_WIDTH)/2;
    float RIGHT_BORDER = GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2;

    if (ballLeftPosition(ptBall) <= LEFT_BORDER)
    {
        // To keep ball inside the game
        ptBall->position.x = LEFT_BORDER + ptBall->radius;
        return 1;
    }
    else if (ballRightPosition(ptBall) >= RIGHT_BORDER)
    {
        ptBall->position.x = RIGHT_BORDER - ptBall->radius;
        return 1;
    }
        
    else
        return 0;
}

// Tell if the ball is inside or outside the game
Position ballOutOfGame(PtBall ptBall)
{
    float TOP_BORDER = (WINDOW_HEIGHT-GAME_HEIGHT)/2;
    float BOTTOM_BORDER = GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2;

    if (ballTopPosition(ptBall) <= TOP_BORDER)
        return OUT_UP;
    else if (ballBottomPosition(ptBall) >= BOTTOM_BORDER)
        return OUT_DOWN;
    else
        return INSIDE;
}

// Tell if the bonus is inside or outside the game
Position bonusOutOfGame(Bonus bonus)
{
    float TOP_BORDER = (WINDOW_HEIGHT-GAME_HEIGHT)/2;
    float BOTTOM_BORDER = GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2;

    if (bonusBottomPosition(bonus) <= TOP_BORDER)
        return OUT_UP;
    else if (bonusTopPosition(bonus) >= BOTTOM_BORDER)
        return OUT_DOWN;
    else
        return INSIDE;
}

// Return 1 if collision with bar1, 2 if collision with bar2, 0 if no collision
int collisionBallBar(PtBall ptBall, PtBar bar1, PtBar bar2)
{
    // Bar 1 collision between the center and the top of the bar
    if (ballBottomPosition(ptBall) >= barTopPosition(bar1) && ballBottomPosition(ptBall) <= bar1->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar1) && ballLeftPosition(ptBall) <= barRightPosition(bar1))
        {
            ptBall->position.y = bar1->position.y - bar1->height/2 - ptBall->radius;
            return 1;
        }
            
        else return 0;
    }

    // Bar 2 is rotated 180°
    else if (ballTopPosition(ptBall) <= barBottomPosition(bar2) && ballTopPosition(ptBall) >= bar2->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar2) && ballLeftPosition(ptBall) <= barRightPosition(bar2))
        {
            ptBall->position.y = bar2->position.y + bar2->height/2 + ptBall->radius;
            return 2;
        }
        else return 0;
    }
    else
        return 0;
}

void changeDirection (Vector2D* direction, Orientation orientation)
{
    if (orientation == HORIZONTAL)
    {
        direction->x = inverse(direction->x);
    }
    else if (orientation == VERTICAL)
    {
        direction->y = inverse(direction->y);
    }
}

// Change ball's direction. The closer you are to the bar center, the more vertical it will be
void changeAngle (PtBall ptBall, PtBar ptBar)
{
    float dist;
    float angle;

    dist = ptBall->position.x - ptBar->position.x;
    // To avoid bugs if you move the bar during collision
    if (dist > ptBar->width/2)
        dist = ptBar->width/2;
    else if (dist < inverse(ptBar->width/2))
        dist = inverse(ptBar->width/2);

    // Value between 0 and M_PI/3
    angle = dist/(ptBar->width/2) * M_PI/3;

    ptBall->direction.x = sin(angle);
    if (ptBall->direction.y > 0)
        ptBall->direction.y = -cos(angle);
    else
        ptBall->direction.y = cos(angle);
}

// Return 1 if collision with bar1, 2 if collision with bar2, 0 if no collision
int collisionBonusBar(Bonus bonus, PtBar bar1, PtBar bar2)
{
    // Bar 1 collision between the bottom and the top of the bar
    if (bonusBottomPosition(bonus) >= barTopPosition(bar1) && bonusBottomPosition(bonus) <= barBottomPosition(bar1))
    {
        if (bonusRightPosition(bonus) >= barLeftPosition(bar1) && bonusLeftPosition(bonus) <= barRightPosition(bar1))
            return 1;
        else
            return 0;
    }

    // Bar 2 is rotated 180°
    else if (bonusTopPosition(bonus) <= barBottomPosition(bar2) && bonusTopPosition(bonus) >= barTopPosition(bar2))
    {
        if (bonusRightPosition(bonus) >= barLeftPosition(bar2) && bonusLeftPosition(bonus) <= barRightPosition(bar2))
            return 2;
        else
            return 0;
    }
    else
        return 0;
}

// Return the position of the ball
Position checkBallPosition (PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick, PtPlayer ptPlayer)
{
    int colBallBar, colBallBrick;
    Position position;

    // 0 if no collision with the bar
    colBallBar = collisionBallBar(ptBall, bar1, bar2);
    if (ptBrick != NULL)
        colBallBrick = BrickCollision(*ptBrick, ptBall);
    else
        colBallBrick = 0;
        
    position = ballOutOfGame(ptBall);

    if (position == INSIDE)
    {
        // If ball hits left or right border
        if (collisionBallWall(ptBall))
        {
            changeDirection(&(ptBall->direction), HORIZONTAL);
            position = WALL;
        }

        // If ball hits bar1 (down)
        if (colBallBar == 1)
        {
            changeAngle(ptBall, bar1);
            position = BAR_DOWN;
        }
        // If ball hits bar2 (up)
        else if (colBallBar == 2)
        {
            changeAngle(ptBall, bar2);
            position = BAR_UP;
        }

        if (colBallBrick == 2 || colBallBrick == 4)
        {
            changeDirection(&(ptBall->direction), HORIZONTAL);
            position = BRICK;
        }
        else if (colBallBrick == 1 || colBallBrick == 3)
        {
            changeDirection(&(ptBall->direction), VERTICAL);
            position = BRICK;
        }
    }
    else // If Ball Oustide
    {
        changeDirection(&(ptBall->direction), VERTICAL);
    }

    return position;
}

Position checkBonusPosition (Bonus bonus, PtBar bar1, PtBar bar2)
{
    int colBonusBar;
    Position position;

    colBonusBar = collisionBonusBar(bonus, bar1, bar2);
    position = bonusOutOfGame(bonus);

    if (colBonusBar == 1)
    {
        position = BAR_DOWN;
    }
    else if (colBonusBar == 2)
    {
        position = BAR_UP;
    }
    return position;
}
