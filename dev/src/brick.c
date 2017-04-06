#include "brick.h"

#define HEIGHT_DEFAULT 5
#define WIDTH_DEFAULT 10

Brick createBrick (Point2D position, BrickType type)
{
    Brick brick;
    brick.position = position;
    brick.width = WIDTH_DEFAULT;
    brick.height = HEIGHT_DEFAULT;
    brick.type = type;
    if (brick.type == DISAP)
    {
        brick.life = 1;
    }
    else if (brick.type == INDES) 
    {
        brick.life = -1;
    }
    else 
    {
        brick.life = 3;
    }
   
    return brick;
}

bool CollisionDroite(Point2D A, Point2D B, Ball ball)
{

    Vecteur u = Vector(A, B);
    Point2D C = ball.position;
    Vecteur AC = (A, C);
    float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
    if (numerateur <0)
        numerateur = -numerateur ;   // valeur absolue ; si c'est négatif, on prend l'opposé.
    float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
    float CI = numerateur / denominateur;
    
    if (CI<ball.radius)
        return true;
    else
        return false;

}



int brickCollision (Brick brick, Ball ball)
{
    Point2D A;
    Point2D B;
    Point2D O = brick.position;
    Point2D C = ball.position;
    
    // UP SIDE
    if()

    // DOWN SIDE
    if()
    Vector2D u;
    A = PointXY(O.x, O.y + brick.height);
    B = PointXY(O.x + brick.width, O.y + brick.height);
    u = Vector(A, B);
    Vector2D AC;
    AC = Vector(A, C);
    float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v

    if (numerateur <0)
    numerateur = -numerateur ;   // valeur absolue ; si c'est négatif, on prend l'opposé.
    
    float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
    float CI = numerateur / denominateur;
    
    if (CI<ball.rayon)
        return true;
    else
        return false;
}

    // RIGHT SIDE

    // LEFT SIDE

    
   int d2 = (x-ball.x)*(x-ball.x) + (y-ball.y)*(y-ball.y);
   if (d2>C.rayon*C.rayon)
}

