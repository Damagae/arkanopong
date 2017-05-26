#ifndef __PRIMITIVES__H
#define __PRIMITIVES__H

#include "bool.h"

/* STRUCTURES */

typedef struct
{
    float r;
    float g;
    float b;
} Color;

/* FUNCTIONS */

void drawLine(float x1, float y1, float x2, float y2);
void drawSquare();
void drawSquareTexture();
void drawSquareBorder();
void drawRoundedSquare(bool full, float radius);
void drawCircle();
void drawCircleTexture();
int textLength(char * text);
void drawText(int x, int y, char* txt, unsigned int size);
void drawButton(int x, int y, char* txt, bool select);
void drawGradientSquare(int timer);

Color createColor(int code);

#endif