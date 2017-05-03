#ifndef __PRIMITIVES__H
#define __PRIMITIVES__H

#include "bool.h"

void drawSquare();
void drawSquareTexture();
void drawSquareBorder();
void drawRoundedSquare(bool full, float radius);
void drawCircle();
void drawCircleTexture();
int textLength(char * text);
void drawText(int x, int y, char* txt);
void drawButton(int x, int y, char* txt, bool select);

#endif