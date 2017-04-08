#include <stdio.h>

#include "collision.h"
#include "math.h"
#include "brick.h"

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
    float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
    if (numerateur <0)
        numerateur = -numerateur ;   // valeur absolue ; si c'est négatif, on prend l'opposé.
    float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
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
        return false;  // si on ne touche pas la droite, on ne touchera jamais le segment

    Vector2D AB,AC, BC;
    AB = Vector(A, B);
    AC = Vector(A, C);
    BC = Vector(B, C);
    float pscal1 = DotProduct(AB, AC);  // produit scalaire
    float pscal2 = DotProduct(MultVector(AB, -1), BC);  // produit scalaire

    if (pscal1>=0 && pscal2>=0)
    {
        printf("Collision segment\n");
        return true;   // I entre A et B, ok.
    }
    // dernière possibilité, A ou B dans le cercle
    if (CollisionPointCercle(A,ball))
    {
        printf("Point A(%.1f,%.1f) dans cercle\n", A.x, A.y);
        return true;
    }
    if (CollisionPointCercle(B,ball))
    {
        printf("Point B(%.1f,%.1f) dans cercle\n", B.x, B.y);
        return true;
    }
    return false;
}

int BrickCollision (Brick brick, Ball ball)
{
    Point2D A = brickVerticeTopLeft(&brick);
    Point2D B = brickVerticeTopRight(&brick);
    Point2D C = brickVerticeBottomLeft(&brick);
    Point2D D = brickVerticeBottomRight(&brick);

    bool AB, BC, CD, DA;
    AB = CollisionSegment(A, B, ball);
    BC = CollisionSegment(B, C, ball);
    CD = CollisionSegment(C, D, ball);
    DA = CollisionSegment(D, A, ball);

    if (AB == true || BC == true || CD == true || DA == true)
    {
        if (AB == true ) {
            printf("AB\n");
            return 1;
        }
        if (BC == true ) {
            printf("BC\n");
            return 2;
        }
        if (CD == true ) {
            printf("CD\n");
            return 3;
        }
        if (DA == true ) {
            printf("DA\n");
            return 4;
        }
    }    
    else {
        return -1;
    }
    return -1;
}

