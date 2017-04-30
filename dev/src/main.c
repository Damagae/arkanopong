#include <SDL/SDL.h>
#include "manager.h"

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH = 1000;
unsigned int WINDOW_HEIGHT = 1000;

/* Dimensions du jeu */
int GAME_WIDTH = 800;
int GAME_HEIGHT = 800;

/* Nombre de vies */
int LIFE_MAX = 3;

int main(int argc, char** argv)
{
  bool AI = true;

  initSDL();

  Game* game = createGame();
  
  playGame(game, AI);

  freeGame(game);

  SDL_Quit();
  
  return EXIT_SUCCESS;
}
