#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>

#include "manager.h"
#include "primitives.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void setVideoMode(unsigned int width, unsigned int height)
{
  if(NULL == SDL_SetVideoMode(width, height, BIT_PER_PIXEL, SDL_OPENGL)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}

void initSDL()
{
    /** Initialisation de la SDL **/
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return;
    }
    setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
}

float randomNumber(float min, float max)
{
    int result = 0, low_num = 0, hi_num = 0;
    int minimum = min*100 + 50;
    int maximum = max*100 - 50;
    float fresult = 0;

    if (minimum < maximum)
    {
        low_num = minimum;
        hi_num = maximum + 100;
    } else {
        low_num = maximum + 100;
        hi_num = minimum;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    fresult = result / 100;
    //SDL_Delay(50);
    return fresult;
}

Game* createGame()
{
    Game* game = malloc(sizeof(Game));
    if (game == NULL)    return NULL;

    game->start = false;

    /* Création des textures */
    game->brickTexture = NULL;
    game->barTexture = NULL;
    game->backgroundTexture = NULL;
    game->lifeTexture = NULL;
    game->bonusTexture = NULL;

    //char* path = "data/img/";
    game->backgroundTextureFile[0] = "data/img/background/greenBackground.jpg";
    game->backgroundTextureFile[1] = "data/img/background/blueBackground.png";
    game->brickTextureFile[0] = "data/img/brick/bob.jpg";
    game->brickTextureFile[1] = "data/img/brick/red.jpg";
    game->brickTextureFile[2] = "data/img/brick/border.jpg";
    game->barTextureFile[0] = "data/img/bar/blueBar.png";
    game->barTextureFile[1] = "data/img/bar/redBar.png";
    game->lifeTextureFile[0] = "data/img/life.png";
    game->lifeTextureFile[1] = "data/img/life_empty.png";
    game->bonusTextureFile[0] = "data/img/bonus/barUP.png";
    game->bonusTextureFile[1] = "data/img/bonus/barDWN.png";
    
    game->backgroundTexture = addTexture(&(game->backgroundTexture), game->backgroundTextureFile[0]);
    game->lifeTexture = addTexture(&(game->lifeTexture), game->lifeTextureFile[0]);
    game->bonusTexture = addTexture(&(game->bonusTexture), game->bonusTextureFile[0]);
    addTexture(&(game->bonusTexture), game->bonusTextureFile[1]);

    /* Création des barres */
    Point2D posDWN = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2 - 50);
    Point2D posUP = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2 + 50);
    game->bar[0] = createBar(posDWN, &(game->barTexture), game->barTextureFile[0]);
    game->bar[1] = createBar(posUP, &(game->barTexture), game->barTextureFile[1]);
    
    /* Création des joueurs */
    game->player[0] = createPlayer(0, "Toto", &(game->bar[0]));
    game->player[1] = createPlayer(1, "Tata", &(game->bar[1]));

    /** Creation des balles **/ 
    game->ballList = NULL;
    Point2D posBDWN = PointXY(randomNumber(barLeftPosition(&(game->bar[0])), barRightPosition(&(game->bar[0]))), posDWN.y - game->bar[0].height);
    Point2D posBUP = PointXY(randomNumber(barLeftPosition(&(game->bar[1])), barRightPosition(&(game->bar[1]))), posUP.y + game->bar[1].height);
    addBall(&(game->ballList), createBall(posBDWN, VectorXY(0, -0.8), &(game->player[0])));
    addBall(&(game->ballList), createBall(posBUP, VectorXY(0, 0.8), &(game->player[1])));
    changeAngle (game->ballList, &(game->bar[0]));
    changeAngle (game->ballList->next, &(game->bar[1]));

    /* Creation de la liste de bonus */
    game->bonusList = NULL;

    /** Creation des briques **/
    game->brickList = NULL;
    game->level = "data/level.txt";
    int * level;
    level = loadLevel(game->level);
    createLevelBricks(level, WINDOW_WIDTH, WINDOW_HEIGHT, &(game->brickList), &(game->bonusList), &(game->brickTexture), game->brickTextureFile[2], &(game->bonusTexture), game->bonusTextureFile);
    
    /* Direction pour controler les barres */
    game->direction[0] = game->direction[1] = NONE;

    return game;
}

void drawGameBorder()
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
        drawSquareBorder();
    glPopMatrix();
}

void drawGameBackground(Texture background)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glBindTexture(GL_TEXTURE_2D, background.texture[background.num]);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void renderGame(Player player1, Player player2, PtBall ballList, PtBrick brickList, BonusList bonusList, Texture background, Texture life)
{    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        drawGameBackground(background);
        
        drawGameBorder();

        glColor3f(1.0, 1.0, 1.0);
        drawAllBalls(ballList);

        drawBar(*(player1.ptBar), player1.num);
        drawBar(*(player2.ptBar), player2.num);

        drawAllBricks(brickList);

        drawAllBonus(bonusList);

        drawLife(player1, life);
        drawLife(player2, life);
        glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawText(470,950, player1.name);
    drawText(470,70, player2.name);

    SDL_GL_SwapBuffers();
}

void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2)
{
    Bonus bonus;
    BonusList ptBonus = (*bonusList);
    
    for(; ptBonus != NULL; ptBonus = ptBonus->next)
    {
        bonus  = *ptBonus;
        if (ptBonus->actif)
        {
            Position bonusPosition = INSIDE;
            moveBonus(ptBonus);
            bonusPosition = checkBonusPosition(bonus, bar1, bar2);

            if (bonusPosition == OUT_UP || bonusPosition == OUT_DOWN || bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
            {
                if (bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
                {
                    getBonus(bonus);
                    ptBonus->actif = false;
                }   
                //deleteBonus(bonusList, &ptBonus);
            }
        }
    }
}

int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick)
{
    if (ptBrick == NULL)
        return -1;
    if (brickDamaged(ptBrick) == 0)
    {
        ptBrick->bonus->ptPlayer = ptBall->ptPlayer;
        ptBrick->bonus->actif = true;
        bonusOrientation(ptBrick->bonus, *(ptBall->ptPlayer));
        //deleteBrick(brickList, ptBrick);
        return 0;
    }
    else
        return ptBrick->life;
}

Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player)
{
    Position ballPosition;

    ballPosition = checkBallPosition(ballList, bar1, bar2, ptBrick, player);
    
    if (ptBrick == NULL)
    {
        if (ballPosition == OUT_DOWN)
        {
            loseLife(&player[0]);
        }
        else if (ballPosition == OUT_UP)
        {
            loseLife(&player[1]);
        }

        // Change the ball owner
        else if (ballPosition == BAR_UP)
        {
            ballList->ptPlayer = &player[1];
        }
        else if (ballPosition == BAR_DOWN)
        {
            ballList->ptPlayer = &player[0];
        }
    }

    else if (ptBrick != NULL)
    {
        if (ballPosition == BRICK)
        {
            brickManager(ballList, brickList, ptBrick);
        }
    }

    return ballPosition;
}

Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player)
{
    Position ballPosition = INSIDE;
    PtBrick ptBrick;

    for(; ballList != NULL; ballList = ballList->next)
    {
        
        moveBall(ballList);
        
        ballPosition = positionDetection(ballList, bar1, bar2, brickList, NULL, player);
        // If the ball hit something, then stop
        if(ballPosition == OUT_UP || ballPosition == OUT_DOWN || ballPosition == BAR_UP ||ballPosition == BAR_DOWN || ballPosition == WALL)
            return ballPosition;
        
        ptBrick = *brickList;
        for(; ptBrick != NULL; ptBrick = ptBrick->next)
        {
            ballPosition = positionDetection(ballList, bar1, bar2, brickList, ptBrick, player);
            if(ballPosition == BRICK)
                return ballPosition;
        }     
    }
    
    return ballPosition;
}

// Parcours la liste chainee de balles
Position runGame(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, BonusList* bonusList)
{
    Position ballPosition;

    // If no ball then exit function
    if (ballList == NULL)
        return -1;
    
    ballPosition = ballManager(ballList, bar1, bar2, brickList, player);
    bonusManager(bonusList, bar1, bar2);

    return ballPosition;
}

bool gameEvent(Game* game)
{
    bool inGame = true;

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      
      switch(e.type) {

        case SDL_QUIT:
            inGame = false;
            break;          

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym)
          {
            case SDLK_LEFT:
              game->direction[0] = LEFT;
              break;
            case SDLK_RIGHT:
              game->direction[0] = RIGHT;
              break;
            case SDLK_q:
              game->direction[1] = LEFT;
              break;
            case SDLK_d:
              game->direction[1] = RIGHT;
              break;
            default:
              break;
          }
          break;

        case SDL_KEYUP:
          switch(e.key.keysym.sym)
          {
            case SDLK_LEFT:
              game->direction[0] = NONE;
              break;
            case SDLK_RIGHT:
              game->direction[0] = NONE;
              break;
            case SDLK_q:
              game->direction[1] = NONE;
              break;
            case SDLK_d:
              game->direction[1] = NONE;
              break;
            case SDLK_SPACE:
              game->start = true;
              break;
            default:
              break;
          }
          break;
          
        default:
          break;
      }
    }
    return inGame;
}

void playGame(Game* game, bool AI)
{
    /** Boucle d'affichage **/
    bool inGame = true;
    while(inGame) {
        Uint32 startTime = SDL_GetTicks();

        /* Dessin */
        renderGame(game->player[0], game->player[1], game->ballList, game->brickList, game->bonusList, *(game->backgroundTexture), *(game->lifeTexture));

        if (game->start)
        {
            game->ballPosition = runGame(game->ballList, &(game->bar[0]), &(game->bar[1]), &(game->brickList), game->player, &(game->bonusList));
            // If a player lose a life
            if (game->ballPosition == OUT_UP || game->ballPosition == OUT_DOWN)
            {
                // Commenter cette ligne pour continuer à jouer serainement
                //start = false;
                
                if (game->player[0].life == 0)
                {
                //printf("%s a perdu !\n",player[0].name);
                }
                else if (game->player[1].life == 0)
                {
                //printf("%s a perdu !\n",player[1].name);
                }
            }
            
            moveBar(game->player[0].ptBar, game->direction[0]);
            if (AI) AIcontroller (game->player[1].ptBar, game->ballList);
            else moveBar(game->player[1].ptBar, game->direction[1]); 
        }

        inGame = gameEvent(game);
        
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

        //printf("%d\n",startTime);
    }
}

void freeGame(Game* game)
{
    freeTexture(&(game->backgroundTexture));
    freeTexture(&(game->brickTexture));
    freeTexture(&(game->barTexture));
    freeTexture(&(game->lifeTexture));
    freeTexture(&(game->bonusTexture));

    deleteBalls(&(game->ballList));
    deleteBrickList(&(game->brickList));
    deleteBonusList(&(game->bonusList));
}