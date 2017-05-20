#ifndef __EDITOR__H
#define __EDITOR__H

#include <GL/gl.h>
#include <GL/glu.h>

#include "manager.h"
#include "bool.h"
#include "menu.h"

/** STRUCTURES **/



/** FUNCTIONS **/

void renderEditor(TextureList editorTextures);

bool editorEvent();
bool editorManager();

#endif