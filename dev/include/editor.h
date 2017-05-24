#ifndef __EDITOR__H
#define __EDITOR__H

#include <GL/gl.h>
#include <GL/glu.h>

#include "manager.h"
#include "bool.h"
#include "menu.h"

/** STRUCTURES **/

/** FUNCTIONS **/

void drawTitle(GLuint texture);
void drawGrid();
void drawBrickPreview(GLuint texture, int position, int color);
void drawTab(int* tab, int* tabColor, TextureList editorTextures);
void drawHowToEdit(GLuint texture);
void renderEditor(TextureList editorTextures, int position, int* tab, int* tabColor, int selection, int color);

int changePosition(int direction, int position);
int switchSelection(int selection);
int switchColor(int color);
int putBrick(int selection);
bool editorEvent(State* state, int* position, int *tab, int* selection, int* color, int* tabColor, Mix_Chunk* sound);
bool editorManager(State* state);

#endif