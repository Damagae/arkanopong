#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#include "geometry.h"
#include "collision.h"
#include "brick.h"
#include "ball.h"
#include "bar.h"
#include "player.h"
#include "ai.h"

#define MAX_BALL 6

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;

/* Dimensions du jeu */
int GAME_WIDTH = 800;
int GAME_HEIGHT = 800;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void setVideoMode(unsigned int width, unsigned int height) {
  if(NULL == SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, GAME_WIDTH, GAME_HEIGHT, 0);
}


int main(int argc, char** argv)
{
  /** Creation des balles et barres et joueurs**/
  Ball ball[MAX_BALL];
  Bar bar[2];
  Player player[2];
  Direction direction[2] = {NONE, NONE};

  bar[0] = createBar(PointXY(GAME_WIDTH/2, GAME_HEIGHT-50));
  ball[0] = createBall (PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), VectorXY(1, -0.5));
  player[0] = createPlayer(0, "Toto", &bar[0], &ball[0]);

  //bar[1] = createBar(PointXY(GAME_WIDTH/2, 50));

  /** Creation des briques **/
  Brick brick;
  BrickType type = NORMAL;
  brick = createBrick(PointXY(GAME_WIDTH/2, GAME_HEIGHT/2), type);
  
  /** Initialisation de la SDL **/
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  /** Boucle d'affichage **/
  int loop = 1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
    glClear(GL_COLOR_BUFFER_BIT);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    drawBall(ball[0]);
    drawBar(*(player[0].p_bar), player[0].num);

    //drawBar(bar[1], 1);

    drawBrick(brick);

    // Brick Collision
    if(BrickCollision(brick, ball[0]) == true) 
    {
      printf("**Collision Main\n");
      SDL_Delay(2000);
      moveBallBrick(&ball[0]);
    }
    else
    {
      moveBall(&ball[0], &bar[0], &bar[1]);
    }
    
    moveBar(player[0].p_bar, direction[0]);
    //moveBar(&bar[1], direction[1]);
    AIcontroller (&bar[1], ball[0]);

    


    SDL_GL_SwapBuffers();
    /* ****** */    

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {          

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym)
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
          switch(e.key.keysym.sym)
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
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }

  SDL_Quit();
  
  return EXIT_SUCCESS;
}
