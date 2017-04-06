#include "collision.h"

bool CollisionPointCercle(Point2D A, Ball ball)
{
    Point2D C = ball.position;
    int d2 = (A.x-C.x)*(A.x-C.x) + (A.y-C.y)*(A.y-C.y);
    if (d2>ball.radius*ball.radius)
        return false;
    else
        return true;
}

bool CollisionDroite(Point2D A, Point2D B, Ball ball)
{
    Vector2D u = Vector(A, B);
    Point2D C = ball.position;
    Vector2D AC = (A, C);
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

bool CollisionSegment(Point2D A,Point2D B, Ball ball)
{
    Point2D C = ball.position;
    if (CollisionDroite(A,B,C) == false)
        return false;  // si on ne touche pas la droite, on ne touchera jamais le segment

    Vector2D AB,AC,BC;
    AB = Vector(A, B);
    AC = Vector(A, C);
    BC = Vector(B, C);
    float pscal1 = DotProduct(AB, AC);  // produit scalaire
    float pscal2 = DotProduct(-AB, AC);  // produit scalaire

    if (pscal1>=0 && pscal2>=0)
        return true;   // I entre A et B, ok.
    // dernière possibilité, A ou B dans le cercle
    if (CollisionPointCercle(A,C))
        return true;
    if (CollisionPointCercle(B,C))
        return true;
    return false;
}

