#include <SDL/SDL.h>
#include <GL/glut.h>
#include "menu.h"
#include "manager.h"
#include "audio.h"
#include "editor.h"
#include "utilities.h"

/* Dimensions de la fenêtre */
unsigned int WINDOW_WIDTH = 1000;
unsigned int WINDOW_HEIGHT = 1000;

/* Dimensions du jeu */
int GAME_WIDTH = 804;
int GAME_HEIGHT = 804;

/* Nombre de vies */
int LIFE_MAX = 10;

int main(int argc, char** argv)
{
  /* Parametres de la partie */
  unsigned int AI = 0;
  bool play = false;
  int level = 0;
  State state = SPLASH;

  initSDL();
  initAudio();
  glutInit( &argc, argv );

  Mix_Music * backgroundMusic = createMusic("data/audio/lego.mp3");
  playMusic(backgroundMusic);
  while (state == SPLASH || state == MENU)
  {
    state = menuManager(state, &AI, &level);
    if (state != QUIT)
    {
      if (state == GAME)
      {
        do
        {
          Game* game = createGame(level);
          if (game != NULL)
            play = playGame(game, AI, &state);
          freeGame(game);
        } while (play);
      }
      else if (state == EDITOR)
      {
        if (editorManager(&state) == false)
          if (state != QUIT)
            state = MENU;
      }
    }
  }

  freeMusic(backgroundMusic);
  closeAudio();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
