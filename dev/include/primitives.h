#ifndef __PRIMITIVES__H
#define __PRIMITIVES__H

#include "bool.h"

void drawLine(int x1, int y1, int x2, int y2);
void drawSquare();
void drawSquareTexture();
void drawSquareBorder();
void drawRoundedSquare(bool full, float radius);
void drawCircle();
void drawCircleTexture();
int textLength(char * text);
void drawText(int x, int y, char* txt, unsigned int size);
void drawButton(int x, int y, char* txt, bool select);

#endif