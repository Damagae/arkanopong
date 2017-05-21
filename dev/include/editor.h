#ifndef __EDITOR__H
#define __EDITOR__H

#include <GL/gl.h>
#include <GL/glu.h>

#include "manager.h"
#include "bool.h"
#include "menu.h"

/** STRUCTURES **/

/** FUNCTIONS **/

void drawGrid();
void drawBrickPreview(GLuint texture, int position);
void drawTab(int* tab, TextureList editorTextures);
void renderEditor(TextureList editorTextures, int position, int* tab, int selection);

int changePosition(int direction, int position);
int switchSelection(int selection);
int switchColor(int color);
bool editorEvent(State* state, int* position, int *tab, int* selection, int* color);
bool editorManager(State* state);

#endif