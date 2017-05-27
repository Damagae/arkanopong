#include <SDL/SDL.h>
#include <GL/glut.h>
#include "menu.h"
#include "game.h"
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

  /* Initialisation des bibliothéques */
  initSDL();
  initAudio();
  glutInit( &argc, argv );

  /* Lancement de la musique principale */
  Mix_Music * backgroundMusic = createMusic("data/audio/lego.mp3");
  playMusic(backgroundMusic);

  /* Boucle qui gère les différents états du jeu */
  while (state == SPLASH || state == MENU)
  {
  /* MENU */
    state = menuManager(state, &AI, &level);
    if (state != QUIT)
    {
  /* JEU */
      if (state == GAME)
      {
        do
        {
          Game* game = createGame(level, AI);
          if (game != NULL)
            play = playGame(game, &state);
          freeGame(game);
        } while (play);
      }
  /* EDITEUR */
      else if (state == EDITOR)
      {
        if (editorManager(&state, -1) == false)
          if (state != QUIT)
            state = MENU;
      }
  /* MODIFICATION DE NIVEAU */
      else if (state == EDITLVL)
      {
        if (editorManager(&state, level) == false)
          if (state != QUIT)
            state = MENU;
      }
    }
  /* FERMETURE DE L'APPLICATION */
  }

  /* Liberation et fermeture des bibliothéques */
  freeMusic(backgroundMusic);
  closeAudio();
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
