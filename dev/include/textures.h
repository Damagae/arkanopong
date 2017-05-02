#ifndef __TEXTURES__H
#define __TEXTURES__H

#include <GL/gl.h>
#include <GL/glu.h>
//#include "brick.h"
#include "bool.h"

/** STRUCTURES **/

typedef struct text
{
    GLuint * texture;
    int num;
    char * file;
    struct text* next;
} Texture, *TextureList;

/** FUNCTIONS **/

Texture* createTexture(char* filename, int num);
GLuint generateTexture(GLuint * texture, char* filename);
Texture* textureExist(TextureList textures, char* filename);
Texture* addTexture(TextureList* textures, char* filename);
void freeTexture(TextureList* textureList);

#endif