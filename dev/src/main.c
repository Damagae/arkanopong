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
  /* Parametres de la partie */
  unsigned int AI = 0;
  bool play = false;
  int level = 0;

  initSDL();
  initAudio();
  glutInit( &argc, argv );

  Mix_Music * backgroundMusic = createMusic("data/audio/lego.mp3");
  playMusic(backgroundMusic);

  if (menuManager(&AI, &level) != QUIT)
  {
    do
    {
      Game* game = createGame(level);
      if (game != NULL)
        play = playGame(game, AI);
      freeGame(game);
    } while (play);
  }

  freeMusic(backgroundMusic);
  closeAudio();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
