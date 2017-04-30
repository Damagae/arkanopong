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
#include "bonus.h"
#include "textures.h"
#include "level.h"

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH = 1000;
unsigned int WINDOW_HEIGHT = 1000;

/* Dimensions du jeu */
int GAME_WIDTH = 800;
int GAME_HEIGHT = 800;

/* Niveau par default */
char levelpath[] = "../data/level.txt";

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
  Position ballPosition;

  /** Initialisation de la SDL **/
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  /* Création des textures */
  TextureList brickTexture = NULL;
  //TextureList ballTexture = NULL;
  TextureList barTexture = NULL;
  TextureList backgroundTexture = NULL;
  TextureList lifeTexture = NULL;
  TextureList bonusTexture = NULL;

  char* backgroundTextureFile[] = {"data/img/background.png"};
  char* brickTextureFile[] = {"data/img/bob.jpg", "data/img/red.jpg", "data/img/border.jpg"};
  //char* ballTextureFile[] = {"data/img/redGradient.bmp"};
  char* barTextureFile[] = {"data/img/bar.png"};
  char* lifeTextureFile[] = {"data/img/life.png", "data/img/life_empty.png"};
  char* bonusTextureFile[] = {"data/img/bonus/barUP.png", "data/img/bonus/barDWN.png"};

  backgroundTexture = addTexture(&backgroundTexture, backgroundTextureFile[0]);
  lifeTexture = addTexture(&lifeTexture, lifeTextureFile[0]);
  //addTexture(&lifeTexture, lifeTextureFile[1]);
  bonusTexture = addTexture(&bonusTexture, bonusTextureFile[0]);
  addTexture(&bonusTexture, bonusTextureFile[1]);

  /* Création des barres */
  Bar bar[2];
  Direction direction[2] = {NONE, NONE};
  bar[0] = createBar(PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2 - 50), &barTexture, barTextureFile[0]);
  bar[1] = createBar(PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2 + 50), &barTexture, barTextureFile[0]);

  /* Création des joueurs */
  Player player[2];
  player[0] = createPlayer(0, "Toto", &bar[0]);
  player[1] = createPlayer(1, "Tata", &bar[1]);

  /** Creation des balles **/ 
  PtBall ballList = NULL;
  addBall(&ballList, createBall(PointXY(450, 550), VectorXY(0, 0.8), &player[0]));
  //addBall(&ballList, createBall(PointXY(450, 350), VectorXY(0, -0.8), &player[1]));

  /* Creation de la liste de bonus */
  BonusList bonusList = NULL;

  /** Creation des briques **/
  PtBrick brickList = NULL;
  int * level;
  level = loadLevel(levelpath);
  createLevelBricks(level, WINDOW_WIDTH, WINDOW_HEIGHT, &brickList, &bonusList, &brickTexture, brickTextureFile[2], &bonusTexture, &bonusTextureFile);

  /** Boucle d'affichage **/
  bool inGame = true;
  while(inGame) {
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
    renderGame(player[0], player[1], ballList, brickList, bonusList, *backgroundTexture, *lifeTexture);

    if (start)
    {
      ballPosition = runGame(ballList, &bar[0], &bar[1], &brickList, player, &bonusList);
      // If a player lose a life
      if (ballPosition == OUT_UP || ballPosition == OUT_DOWN)
      {
        // Commenter cette ligne pour continuer à jouer serainement
        //start = false;
        
        if (player[0].life == 0)
        {
          //printf("%s a perdu !\n",player[0].name);
        }
        else if (player[1].life == 0)
        {
          //printf("%s a perdu !\n",player[1].name);
        }
        
      }
      
      moveBar(player[0].ptBar, direction[0]);
      //moveBar(&bar[1], direction[1]);
      AIcontroller (&bar[1], ballList[0]);
    }

    /* ****** */    

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        inGame = false;
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

  freeTexture(&backgroundTexture);
  freeTexture(&brickTexture);
  //freeTexture(&ballTexture);
  freeTexture(&barTexture);
  freeTexture(&lifeTexture);
  freeTexture(&bonusTexture);

  deleteBalls(&ballList);
  deleteBrickList(&brickList);
  deleteBonusList(&bonusList);

  SDL_Quit();
  
  return EXIT_SUCCESS;
}
