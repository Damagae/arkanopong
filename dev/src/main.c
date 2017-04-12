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
#include "manager.h"
#include "bool.h"

#define MAX_BALL 6

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 800;

/* Dimensions du jeu */
int GAME_WIDTH = 600;
int GAME_HEIGHT = 600;

int LIFE_MAX = 3;

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
  gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}


int main(int argc, char** argv)
{
  bool start = false;
  int alive;

  /** Creation des balles, barres et joueurs**/
  Ball ball[MAX_BALL];
  Bar bar[2];
  Player player[2];
  Direction direction[2] = {NONE, NONE};

  bar[0] = createBar(PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2 - 50));
  ball[0] = createBall (PointXY(450, 550), VectorXY(0, 0.8));
  player[0] = createPlayer(0, "Toto", &bar[0], &ball[0]);

  bar[1] = createBar(PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2 + 50));
  player[1] = createPlayer(1, "Tata", &bar[1], &ball[0]);

  /** Creation des briques **/
  Brick brick;
  BrickType type = NORMAL;
  brick = createBrick(PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT/2 + (WINDOW_HEIGHT-GAME_HEIGHT)/2), type);
  
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
    renderGame(player[0], player[1], brick);

    if (start)
    {
      alive = runGame(&ball[0], &bar[0], &bar[1], &brick, player);
      if (alive < LIFE_MAX)
      {
        // Commenter cette ligne pour continuer à jouer serainement
        //start = false;
        if (!alive)
        {
          if (player[0].life == 0)
          {
            printf("%s a perdu !\n",player[0].name);
          }
          else
          {
            printf("%s a perdu !\n",player[1].name);
          }
        }
      }
      
      moveBar(player[0].p_bar, direction[0]);
      moveBar(&bar[1], direction[1]);
      //AIcontroller (&bar[1], ball[0]);
    }

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
            case SDLK_SPACE:
              start = true;
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
