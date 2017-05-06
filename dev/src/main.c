#include <SDL/SDL.h>
#include <GL/glut.h>
#include "manager.h"

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH = 1000;
unsigned int WINDOW_HEIGHT = 1000;

/* Dimensions du jeu */
int GAME_WIDTH = 800;
int GAME_HEIGHT = 800;

/* Nombre de vies */
int LIFE_MAX = 6;

int main(int argc, char** argv)
{
  bool AI = true;
  bool play = true;

  initSDL();
  glutInit( &argc, argv );

  while (play)
  {
    Game* game = createGame();
    play = playGame(game, AI);
    freeGame(game);
  }

  SDL_Quit();
  
  return EXIT_SUCCESS;
}
