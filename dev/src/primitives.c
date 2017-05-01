#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>

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

void drawSquareTexture()
{
    glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2f(-0.5,0.5);

        glTexCoord2f(1, 0);
        glVertex2f(0.5,0.5);

        glTexCoord2f(1, 1);
        glVertex2f(0.5,-0.5);

        glTexCoord2f(0, 1);
        glVertex2f(-0.5,-0.5);

    glEnd();
}

void drawSquareBorder()
{
    glBegin(GL_LINE_LOOP);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);

        glColor3f(1.0, 0.0, 0.0);
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

void drawCircleTexture()
{
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i <= SEGMENTS; i++)
    {
        //glTexCoord2f(0, 0);
        glVertex2f(cos(i*(M_PI/(SEGMENTS/2))), sin(i*(M_PI/(SEGMENTS/2))));
    }
    glEnd();
}

int textLength(char * text)
{
    int length = 0;
    while(text[length])
        length++;
    return length;
}

void drawText(int x, int y, char* txt)
{
    if(txt!=NULL)
    {   
        //glMatrixMode( GL_PROJECTION );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();

        glRasterPos2f(x, y);
    
        int len = textLength(txt);
        for (int i = 0; i < len; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt[i]);
        }
    
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
    }
}