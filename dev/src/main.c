#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

/*** Pourquoi déclarer plusieurs textures lorsqu'une seule suffit ? ***/
#define MAX 10

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(unsigned int width, unsigned int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
}

void setVideoMode(unsigned int width, unsigned int height) {
  if(NULL == SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  
  reshape(width, height);
}

  /** Dessine un carré (plein ou non) de côté 1 et centré en (0, 0). **/
void draw_square(int full) {
  /*** Tu aurais pu ajouter tes glTexCoord ici, et appeler draw_square. ***/
  glBegin(full ? GL_QUADS : GL_LINE_LOOP);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);
  glEnd();
}

int main(int argc, char** argv) {
  unsigned int WINDOW_WIDTH = 400;
  unsigned int WINDOW_HEIGHT = 400;
  /** Variables pour la texture **/
  char* filename = "medias/textures/bob.jpg";
  /*** Pour déclarer les chaînes de caractères, utilises plutôt un tableau, ou un pointeur constant. Au choix:

  char const* filename = "bob.jpg";
  char filename[] = "bob.jpg";
  char const filename[] = "bob.jpg";

  En effet, ici, tu as un char*, ce qui laisse penser que tu peux modifier ta chaîne, et faire
  filename[0] = 'c';
  Cela va résulter en une segfault, car la mémoire allouée pour les littéraux (ici "bob.jpg") n'est accessible
  quand lecture.

  ***/

  SDL_Surface* surface;
  /*** Initialise toujours tes variables à la creation, pour éviter les segfaults.
  SDL_Surface* surface = NULL;
  ***/

  /** Pointeur vers zone mémoire pouvant contenir MAX entiers **/
  GLuint textures[MAX];
  /*** Un seul suffisait.
  GLuint texture = 0;
  ***/

  /** Variables pour gerer la rotation et le scale **/
  int leftClick = 0, rightClick = 0;
  float myRotation = 0;
  float myScale = 1;
  
  /** Initialisation de la SDL **/
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  /** Charge la texture et teste si ca fonctionne **/
  SDL_WM_SetCaption("Texture", NULL);
  surface = IMG_Load(filename);
  if (surface == NULL)
  {
      fprintf(stderr, "L'allocation de l'image ne marche pas :/\n");
  }

  /** GESTION ET ALLOCATION DE LA TEXTURE **/

  /** demande à OpenGl 1 espace mémoire (< MAX) sur la CG pour placer nos textures **/
  glGenTextures(1, textures);
  /*** Si on a qu'une seule texture, l'appel ici donne:
  glGenTextures(1, &texture);
  ***/
  /** on bind la texture sur la case 1 de "textures" **/
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  /*** Et là
  glBindTexture(GL_TEXTURE_2D, texture);
  ***/
  /** On modifie les paramètres de la texture (filtre de minification) **/
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /** Determine le format des pixels **/
  GLenum format;
  switch(surface->format->BytesPerPixel) {
    case 1:
      format = GL_RED;
    break;
    case 3:
      format = GL_RGB;
    break;
    case 4:
      format = GL_RGBA;
    break;
    default:
      /* On ne traite pas les autres cas */
      fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", filename);
    return EXIT_FAILURE;
  }
  /** Envoie les données de la texture à OpenGL **/
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
  /*** Debind ta texture ici aussi, pour éviter de la modifier par inadvertance si tu fais autre chose entre temps. ***/

  /** Boucle d'affichage **/
  int loop = 1;
  while(loop) {
    Uint32 startTime = SDL_GetTicks();

    /* Dessin */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /** Active le texturage 2D **/
    glEnable(GL_TEXTURE_2D);
    /** On binde notre texture **/
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glPushMatrix();

      glRotatef(myRotation, 0.0, 0.0, 1.0);
      glScalef(myScale, myScale, 1);

      /** Specifie les coordonnees de la texture **/
      glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(-0.5,0.5);

        glTexCoord2f(1, 0);
        glVertex2f(0.5,0.5);

        glTexCoord2f(1, 1);
        glVertex2f(0.5,-0.5);

        glTexCoord2f(0, 1);
        glVertex2f(-0.5,-0.5);
      glEnd();

    glPopMatrix();

    /** On dé-binde la texture **/
    glBindTexture(GL_TEXTURE_2D, 0);
    /** Desactive le texturage 2D **/
    glDisable(GL_TEXTURE_2D);

    SDL_GL_SwapBuffers();
    /* ****** */    

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      switch(e.type) {          
        case SDL_VIDEORESIZE:
          WINDOW_WIDTH = e.resize.w;
          WINDOW_HEIGHT = e.resize.h;
          setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
          break;

        case SDL_KEYDOWN:
          if (e.key.keysym.sym == 'q' || e.key.keysym.sym == SDLK_ESCAPE) {
            loop = 0;
          }
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (e.button.button == SDL_BUTTON_LEFT)
          {
            leftClick = 1;
          }
          if (e.button.button == SDL_BUTTON_RIGHT)
          {
            rightClick = 1;
          }
          break;

        case SDL_MOUSEBUTTONUP:
          if (e.button.button == SDL_BUTTON_LEFT)
          {
            leftClick = 0;
          }
          if (e.button.button == SDL_BUTTON_RIGHT)
          {
            rightClick = 0;
          }
        break;

        /*** Tu pouvais concaténer les deux cases plus haut.
        
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          if (e.button.button == SDL_BUTTON_LEFT)         leftClick = e.button.state;
          else if (e.button.button == SDL_BUTTON_RIGHT)   rightClick = e.button.state;
        break;

        ***/

        case SDL_MOUSEMOTION:
          if (leftClick == 1) {
            myRotation += (-1.)*e.motion.yrel;
            /*** myRotation -= e.motion.yrel; pour aller plus vite :B ***/
          }
          if (rightClick == 1) {
            myScale += (-1.)*(e.motion.yrel)/100;
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

  /** Libere la texture */
  SDL_FreeSurface(surface);
  /*** Tu pouvais libérer la surface juste après glTexImage2D (qui copie l'image dans le GPU), puisque tu ne l'as pas
  réutilisée depuis. ***/
  /** on dé-bind la texture **/
  glBindTexture(GL_TEXTURE_2D, 0);
  /*** Pas besoin de débinder, puisque tu n'as rien bindé par ici. ***/
  /** on libére la mémoire allouée pour la texture **/
  glDeleteTextures(1, textures);

  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
