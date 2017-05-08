#include <SDL/SDL.h>
#include <GL/glut.h>
#include "menu.h"
#include "manager.h"
#include "audio.h"

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
  bool AI = false;
  bool play = true;

  initSDL();
  initAudio();
  glutInit( &argc, argv );

  Mix_Music * backgroundMusic = createMusic("data/audio/lego.mp3");
  playMusic(backgroundMusic);
  if (menuManager(&AI) != EXIT)
  {
    while (play)
    {
      Game* game = createGame();
      play = playGame(game, AI);
      freeGame(game);
    }
  }

  freeMusic(backgroundMusic);
  closeAudio();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
