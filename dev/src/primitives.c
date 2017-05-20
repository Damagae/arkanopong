#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>

#include "bool.h"

#define SEGMENTS 32

/** Fonctions de dessin canonique. **/

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);

    glEnd();
}

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
        //glColor3f(0.0, 0.0, 1.0);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);

        //glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.5, -0.5);
        glVertex2f(-0.5, -0.5);

    glEnd();
}

void drawRoundedSquare(bool full, float radius)
{
    float i;
    glBegin(full ? GL_POLYGON : GL_LINE_LOOP);
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

void drawText(int x, int y, char* txt, unsigned int size)
{
    if(txt!=NULL)
    {   
        //glMatrixMode( GL_PROJECTION );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();
    
        int len = textLength(txt);
        glRasterPos2f(8+x-5*len, y+5);

        void * font[7] = {
                    GLUT_BITMAP_8_BY_13,
                    GLUT_BITMAP_9_BY_15,
                    GLUT_BITMAP_TIMES_ROMAN_10,
                    GLUT_BITMAP_TIMES_ROMAN_24,
                    GLUT_BITMAP_HELVETICA_10,
                    GLUT_BITMAP_HELVETICA_12,
                    GLUT_BITMAP_HELVETICA_18
                };

        for (int i = 0; i < len; i++)
        {
            glutBitmapCharacter(font[size], txt[i]);
        }
    
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
    }
}

void drawButton(int x, int y, char* txt, bool select)
{
    glPushMatrix();
        glTranslatef(x, y, 1);
        glScalef(200, 100, 1);
        float color = select ? 1.0 : 0.0;
        glColor3f(color, color, color);
        drawRoundedSquare(1, 0.05);
        glColor3f(1.0, 0.0, 0.0);
        drawRoundedSquare(0, 0.05);
        drawText(x,y,txt, 1);
    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);
}