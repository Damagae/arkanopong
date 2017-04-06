#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#include "geometry.h"
#include "ball.h"
#include "bar.h"
#include "primitives.h"

#define MAX_BALL 6

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Dimensions du jeu */
int GAME_WIDTH = 800;
int GAME_HEIGHT = 800;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void setVideoMode(unsigned int width, unsigned int height)
{
  if(NULL == SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL))
  {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);
  //gluOrtho2D(-1, -1, 1, 1);
}

int main(int argc, char** argv)
{
  /** Creation des balles **/
  Ball ball[MAX_BALL];
  Bar bar[2];
  Direction direction[2] = {NONE, NONE};
  bar[0] = createBar(PointXY(GAME_WIDTH/2, GAME_HEIGHT-50));
  ball[0] = createBall (PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), VectorXY(1, 0.5));

  /** Initialisation de la SDL **/
  if(-1 == SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  /** Boucle d'affichage **/
  int loop = 1;
  while(loop)
  {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBall(ball[0]);
    drawBar(bar[0]);

    drawSquare();

    moveBall(&ball[0]);
    moveBar(&bar[0], direction[0]);

    SDL_GL_SwapBuffers();
    /* ****** */    

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      if(e.type == SDL_QUIT)
      {
        loop = 0;
        break;
      }
      
      switch(e.type)
      {          
        case SDL_VIDEORESIZE:
          WINDOW_WIDTH = e.resize.w;
          WINDOW_HEIGHT = e.resize.h;
          setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
          break;

        case SDL_KEYDOWN:
          switch (e.key.keysym.sym)
          {
            case SDLK_LEFT:
              direction[0] = LEFT;
              break;
            case SDLK_RIGHT:
              direction[0] = RIGHT;
              break;
            case SDLK_q:
              direction[1] = LEFT;
              break;
            case SDLK_d:
              direction[1] = RIGHT;
              break;
            default:
              break;
          }
          break;

        case SDL_KEYUP:
          switch (e.key.keysym.sym)
          {
            case SDLK_LEFT:
              direction[0] = NONE;
              break;
            case SDLK_RIGHT:
              direction[0] = NONE;
              break;
            case SDLK_q:
              direction[1] = NONE;
              break;
            case SDLK_d:
              direction[1] = NONE;
              break;
            default:
              break;
          }
          break;
          
        default:
          break;
      }
    }
    
    /* Calcul du temps écoulé */
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    /* Si trop peu de temps s'est écoulé, on met en pause le programme */
    if(elapsedTime < FRAMERATE_MILLISECONDS)
    {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }

  SDL_Quit();
  
  return EXIT_SUCCESS;
}
