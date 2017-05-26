#include "textures.h"

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define MAX_TEXTURE 30

Texture* createTexture(char* filename, int num)
{
  Texture* textures = malloc(sizeof (Texture));
  if (textures == NULL)
    return NULL;

  if(num == 0)
  {
    GLuint * ptTexture = malloc(MAX_TEXTURE * sizeof(GLuint));
    if (ptTexture == NULL)
      return NULL;
    textures->texture = ptTexture;
  }

  textures->num = num;
  textures->file = filename;
  textures->next = NULL;

  return textures;
}

GLuint generateTexture(GLuint * texture, char* filename)
{
  SDL_Surface* surface = NULL;

  /** Charge la texture et teste si ca fonctionne **/
  surface = IMG_Load(filename);
  if (surface == NULL)
  {
      fprintf(stderr, "L'allocation de l'image '%s' ne marche pas :/\n", filename);
  }

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  GLenum format;
  switch(surface->format->BytesPerPixel) {
    case 1:
      format = GL_RED;
    break;
    case 3:
      format = GL_RGB;
    break;
    case 4:
      format = GL_RGBA;
    break;
    default:
      /* On ne traite pas les autres cas */
      fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", filename);
      return EXIT_FAILURE;
  }
  /** Envoie les données de la texture à OpenGL **/
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  /** Libere la surface */
  SDL_FreeSurface(surface);
  //printf("HUM : %d\n", *texture);
  return *texture;
}

Texture* textureExist(TextureList textures, char* filename)
{
  Texture* tmp = textures;
  while(tmp != NULL)
  {
    if (tmp->file == filename)
      return tmp;
    tmp = tmp->next;
  }
  
  return NULL;
}

Texture* addTexture(TextureList* textures, char* filename)
{
  if(*textures == NULL)
    *textures = createTexture(filename, 0);
  else
  {
    Texture* exist = textureExist(*textures, filename);
    if (exist == NULL)
    {
      int num = (*textures)->num + 1;
      (*textures)->next = createTexture(filename, num);
      (*textures)->next->texture = (*textures)->texture;
      (*textures) = (*textures)->next;
    }
    else
      return exist;
  }
  ((*textures)->texture)[(*textures)->num] = generateTexture(&(((*textures)->texture)[(*textures)->num]), filename);

  return (*textures);
}

void freeTexture(TextureList* textureList)
{
    if (textureList == NULL)
        return;
    if (((*textureList)->texture) != NULL)
        glDeleteTextures(MAX_TEXTURE, (*textureList)->texture);
    TextureList tmp = *textureList;
    while (tmp != NULL)
    {
      tmp = (*textureList)->next;
      free(*textureList);
      *textureList = tmp;
    }
}