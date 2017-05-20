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
void drawBrickPreview(GLuint texture);
void renderEditor(TextureList editorTextures);

bool editorEvent(State* state);
bool editorManager(State* state);

#endif