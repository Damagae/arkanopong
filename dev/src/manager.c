#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>

#include "manager.h"
#include "primitives.h"
#include "menu.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

int left1;
int right1;
int left2;
int right2;

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
    levelList();

    Game* game = malloc(sizeof(Game));
    if (game == NULL)    return NULL;

    game->start = false;
    game->pause = false;
    game->end = false;

    /* Création des textures */
    game->brickTexture = NULL;
    game->barTexture = NULL;
    game->ballTexture = NULL;
    game->backgroundTexture = NULL;
    game->lifeTexture = NULL;
    game->bonusTexture = NULL;

    //char* path = "data/img/";
    game->backgroundTextureFile[0] = "data/img/background/fond.jpg";
    game->backgroundTextureFile[1] = "data/img/menu/menuBackground.jpg";
    game->brickTextureFile[0] = "data/img/brick/B_lego_4x2.png";
    game->brickTextureFile[1] = "data/img/brick/R_lego_4x2.png";
    game->brickTextureFile[2] = "data/img/brick/P_lego_4x2.png";
    game->barTextureFile[0] = "data/img/bar/B_lego_6x1.png";
    game->barTextureFile[1] = "data/img/bar/R_lego_4x1.png";
    game->lifeTextureFile[0] = "data/img/life.png";
    game->lifeTextureFile[1] = "data/img/life_empty.png";
    game->bonusTextureFile[0] = "data/img/bonus/barUP.png";
    game->bonusTextureFile[1] = "data/img/bonus/barDWN.png";
    
    game->backgroundTexture = addTexture(&(game->backgroundTexture), game->backgroundTextureFile[0]);
    addTexture(&(game->backgroundTexture), game->backgroundTextureFile[1]);
    game->lifeTexture = addTexture(&(game->lifeTexture), game->lifeTextureFile[0]);
    game->bonusTexture = addTexture(&(game->bonusTexture), game->bonusTextureFile[0]);
    addTexture(&(game->bonusTexture), game->bonusTextureFile[1]);

    /* Création des barres */
    Point2D posDWN = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2 - 50);
    Point2D posUP = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2 + 50);
    game->bar[0] = createBar(posDWN, &(game->barTexture), game->barTextureFile[0]);
    game->bar[1] = createBar(posUP, &(game->barTexture), game->barTextureFile[1]);
    
    /* Création des joueurs */
    game->player[0] = createPlayer(0, "Player 1", &(game->bar[0]));
    game->player[1] = createPlayer(1, "Player 2", &(game->bar[1]));

    /** Creation des balles **/ 
    game->ballList = NULL;
    Point2D posBDWN = PointXY(randomNumber(barLeftPosition(&(game->bar[0])), barRightPosition(&(game->bar[0]))), posDWN.y - game->bar[0].height);
    Point2D posBUP = PointXY(randomNumber(barLeftPosition(&(game->bar[1])), barRightPosition(&(game->bar[1]))), posUP.y + game->bar[1].height);
    addBall(&(game->ballList), createBall(posBDWN, VectorXY(0, -0.8), &(game->player[0]), &game->ballTexture));
    addBall(&(game->ballList), createBall(posBUP, VectorXY(0, 0.8), &(game->player[1]), &game->ballTexture));
    changeAngle (game->ballList, &(game->bar[0]));
    changeAngle (game->ballList->next, &(game->bar[1]));

    /* Creation de la liste de bonus */
    game->bonusList = NULL;

    /** Creation des briques **/
    game->brickList = NULL;
    game->level = "data/level.txt";
    int * level;
    level = loadLevel(game->level);
    createLevelBricks(level, WINDOW_WIDTH, WINDOW_HEIGHT, &(game->brickList), &(game->bonusList), &(game->brickTexture), game->brickTextureFile, &(game->bonusTexture), game->bonusTextureFile);
    
    /* Direction pour controler les barres && le menu */
    game->direction[0] = game->direction[1] = NONE;
    game->selection = NONE;

    game->sound[0] = createSound("data/audio/hit.wav");

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

    glBindTexture(GL_TEXTURE_2D, background.texture[0]);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void renderGame(Game* game, char timer, bool restart)
{    

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawWindowBackground(game->backgroundTexture->texture[1]);

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        drawGameBackground(*(game->backgroundTexture));
        drawGameBorder();

        drawAllBalls(game->ballList);

        drawBar(*(game->player[0].ptBar), game->player[0].num);
        drawBar(*(game->player[1].ptBar), game->player[1].num);

        drawAllBricks(game->brickList);

        drawAllBonus(game->bonusList);

        drawLife(game->player[0], *(game->lifeTexture));
        drawLife(game->player[1], *(game->lifeTexture));

        glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawNames(game->player[0].name, game->player[1].name);

    if (timer != '0')
    {
        glColor3f(1.0, 0.0, 0.0);
        drawText(490,485, &timer, 6);
    }

    if (game->pause)
    {
        drawText(500, 500, "Pause", 6);
    }

    if(game->end)
    {
        drawWinner(game->player[0], game->player[1]);
        drawRestart(restart);
    }

    glColor3f(1.0, 1.0, 1.0);
    SDL_GL_SwapBuffers();
}

void drawRestart(bool restart)
{
    drawButton(350, 550, "RESTART", restart);
    if (restart)
    {
        drawButton(650, 550, "EXIT", false);
    }
    else
    {
        drawButton(650, 550, "EXIT", true);
    }
}

void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2, PtBall* ballList)
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
                    getBonus(bonus, ballList);
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
        if (ptBrick->bonus != NULL)
        {
            ptBrick->bonus->ptPlayer = ptBall->ptPlayer;
            ptBrick->bonus->actif = true;
            bonusOrientation(ptBrick->bonus, *(ptBall->ptPlayer));
        }
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

Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, Mix_Chunk * hitSound)
{
    Position ballPosition = INSIDE;
    PtBrick ptBrick;
    unsigned int channel = 1;

    for(; ballList != NULL; ballList = ballList->next)
    {
        moveBall(ballList);
        
        ballPosition = positionDetection(ballList, bar1, bar2, brickList, NULL, player);
        // If the ball hit something, then stop
        if(ballPosition == OUT_UP || ballPosition == OUT_DOWN || ballPosition == BAR_UP ||ballPosition == BAR_DOWN || ballPosition == WALL)
        {
            return ballPosition;
        }
            
        
        ptBrick = *brickList;
        for(; ptBrick != NULL; ptBrick = ptBrick->next)
        {
            ballPosition = positionDetection(ballList, bar1, bar2, brickList, ptBrick, player);
            if(ballPosition == BRICK)
            {
                playSound(channel, hitSound);
                return ballPosition;
            }
        } 
        ++channel;   
    }
    
    return ballPosition;
}

// Parcours la liste chainee de balles
Position runGame(Game* game)
{
    Position ballPosition;

    // If no ball then exit function
    if (game->ballList == NULL)
        return -1;
    
    ballPosition = ballManager(game->ballList, &game->bar[0], &game->bar[1], &game->brickList, game->player, game->sound[0]);
    bonusManager(&game->bonusList, &game->bar[0], &game->bar[1], &game->ballList);

    return ballPosition;
}

bool gameEvent(Game* game, char timer)
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
              if(right1 == 1) {
                left1 = 2;
              } else {
                left1 = 1;
              }
              break;
            case SDLK_RIGHT:
              if(left1 == 1) {
                right1 = 2;
              }
              else {
                right1 = 1;
              }
              break;
            case SDLK_q:
              if(right2 == 1) {
                left2 = 2;
              } else {
                left2 = 1;
              }
              break;
            case SDLK_d:
              if(left2 == 1) {
                right2 = 2;
              }
              else {
                right2 = 1;
              }
              break;
            case SDLK_UP:
              game->selection = UP;
              break;
            case SDLK_DOWN:
              game->selection = DOWN;
              break;
            case SDLK_RETURN:
                if(game->end)
                    inGame = false;
                break;
            case SDLK_SPACE:
                if(timer == '0' && game->end == false)
                {
                    if(game->pause)
                    {
                        game->pause = false;
                        Mix_ResumeMusic();
                        Mix_Resume(-1);
                    }
                    else
                    {
                        game->pause = true;
                        Mix_PauseMusic();
                        Mix_Pause(-1);
                    }
                }
            default:
              break;
          }
          break;

        case SDL_KEYUP:
          switch(e.key.keysym.sym)
          {
            case SDLK_LEFT:
              if (right1 == 2) {
                right1 = 1;
              }
              left1 = 0;
              break;
            case SDLK_RIGHT:
              if (left1 == 2) {
                left1 = 1;
              }
              right1 = 0;
              break;
            case SDLK_q:
              if (right2 == 2) {
                right2 = 1;
              }
              left2 = 0;
              break;
            case SDLK_d:
              if (left2 == 2) {
                left2 = 1;
              }
              right2 = 0;
              break;
            case SDLK_UP:
              break;
            case SDLK_DOWN:
              break;
            case SDLK_RETURN:
                break;
            case SDLK_SPACE:
              break;
            default:
              break;
          }
          break;
          
        default:
          break;
      }

      if ( (left1 == 1 && right1 == 0) || (left1 == 2 && right1 == 1) )
      {
          game->direction[0] = LEFT;
          game->selection = LEFT;
      }
      else if ( (left1 == 0 && right1 == 1) || (left1 == 1 && right1 == 2) )
      {
          game->direction[0] = RIGHT;
          game->selection = RIGHT;
      }
      else {
          game->direction[0] = NONE;
      }
      if ( (left2 == 1 && right2 == 0) || (left2 == 2 && right2 == 1) )
      {
          game->direction[1] = LEFT;
      }
      else if ( (left2 == 0 && right2 == 1) || (left2 == 1 && right2 == 2) )
      {
          game->direction[1] = RIGHT;
      }
      else {
          game->direction[1] = NONE;
      }

    }
    return inGame;
}

bool playGame(Game* game, bool AI)
{
    bool inGame = true;
    bool restart = false;
    char timer = '0';
    Uint32 ticks_reset = SDL_GetTicks();

    if(AI)
        game->player[1].name = "Computer";
    
    /* FOR COLLISION TEST 
        Ball* ball = createBall(PointXY(545,600), VectorXY(0,-1), &game->player[0]);
        addBall(&game->ballList, ball);
    */

    /** Boucle d'affichage et de gestion du jeu **/
    while(inGame)
    {
        Uint32 startTime = SDL_GetTicks() - ticks_reset;

        if(startTime < 5100)
        {
            moveBarBall(game->player[0].ptBar, game->ballList, game->direction[0]);
            if(!AI) moveBarBall(game->player[1].ptBar, game->ballList->next, game->direction[1]);
            timer = gameLaunch(startTime);
        }
        else if(startTime >= 5100 && startTime <= 5500)
            game->start = true;

        /* Dessin */
        renderGame(game, timer, restart);

        if (game->start && !game->pause)
        {
            game->ballPosition = runGame(game);
            // If a player lose a life
            if (game->ballPosition == OUT_UP || game->ballPosition == OUT_DOWN)
            {                
                if (game->player[0].life == 0 || game->player[1].life == 0)
                {
                    game->start = false;
                    game->end = true;
                }
            }
            
            moveBar(game->player[0].ptBar, game->direction[0]);
            if (AI) AIcontroller (game->player[1].ptBar, game->ballList);
            else moveBar(game->player[1].ptBar, game->direction[1]); 
        }

        if (game->end)
        {
            restart = restartGame(game->selection);
        }

        inGame = gameEvent(game, timer);
        
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

        //printf("%d\n",startTime);
    }
    return restart;
}

char gameLaunch(Uint32 startTime)
{
    char timer = '5';
    timer = timer-(startTime/1000);
    //printf("%d\n",startTime);

    return timer;
}

void moveBarBall(PtBar bar, PtBall ball, Direction direction)
{
    float LEFT_BORDER = (WINDOW_WIDTH-GAME_WIDTH)/2;
    float RIGHT_BORDER = GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2;

    if(direction == RIGHT && barRightPosition(bar) <= RIGHT_BORDER)
    {
        if (barRightPosition(bar)+bar->speed <= RIGHT_BORDER)
        {
            bar->position.x += bar->speed;
            ball->position.x += bar->speed;
        }
    }
    else if (direction == LEFT && barLeftPosition(bar) >= LEFT_BORDER)
    {
        if (barLeftPosition(bar)-bar->speed >= LEFT_BORDER)
        {
            bar->position.x -= bar->speed;
            ball->position.x -= bar->speed;
        }
    }
}

bool restartGame(Direction direction)
{
    bool restart = true;
    if (direction == LEFT)
        restart = true;
    else if (direction == RIGHT)
        restart = false;
    return restart;
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

    freeSound(game->sound[0]);
}