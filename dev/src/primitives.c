#include <GL/gl.h>
#include <math.h>

#define SEGMENTS 32

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