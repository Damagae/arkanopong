#include <GL/gl.h>
#include <math.h>

#define SEGMENTS 32

/** Fonctions de dessin canonique. **/

void drawSquare()
{
    glBegin(GL_QUADS);

    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);

    glEnd();
}

void drawSquareBorder()
{
    glBegin(GL_LINE_LOOP);

    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);

    glEnd();
}

void drawRoundedSquare(float radius)
{
    float i;
    glBegin(GL_POLYGON);
    for (i = 0; i <= (SEGMENTS/4); i++)
    {
        glVertex2f(0.5-radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, 0.5-radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = (SEGMENTS/4); i <= 2*(SEGMENTS/4); i++)
    {
        glVertex2f(-0.5+radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, 0.5-radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = 2*(SEGMENTS/4); i <= 3*(SEGMENTS/4); i++)
    {
        glVertex2f(-0.5+radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, -0.5+radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    for (i = 3*(SEGMENTS/4); i <= SEGMENTS; i++)
    {
        glVertex2f(0.5-radius+cos(i*(M_PI/(SEGMENTS/2)))*radius, -0.5+radius+sin(i*(M_PI/(SEGMENTS/2)))*radius);
    }
    glEnd();
}

void drawCircle()
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i <= SEGMENTS; i++)
    {
        glVertex2f(cos(i*(M_PI/(SEGMENTS/2))), sin(i*(M_PI/(SEGMENTS/2))));
    }
    glEnd();
}