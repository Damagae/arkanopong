#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>

#include "game.h"
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

float transition = 0;
static bool mute = false;

static int hit[2] = {0,0};

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

#define AI_HARD 10 //Speed of the bar if AI is hard
#define AI_EASY 6 //Speed of the bar if AI is easy

/* INITIALISATION */

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
    /** SDL Initialisation **/
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return;
    }
    setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_WM_SetCaption("Arkanopong", NULL);
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
    return fresult;
}

Game* createGame(int lvl, unsigned int AI)
{
    int numFiles;
    char ** levelFiles = levelList(&numFiles);

    Game* game = malloc(sizeof(Game));
    if (game == NULL)    return NULL;

    /* Game State */
    game->start = false;
    game->pause = false;
    game->end = false;

    game->AI = AI;

    /* Level reading */
    game->level = levelFiles[lvl];
    int * level;
    level = loadLevel(game->level);
    free(levelFiles);

    if (level == NULL)
    {
        fprintf(stderr, "Fermeture du programme\n");
        return NULL;
    }

    /* Textures Creation */

    game->brickTextureFile[0] = "data/img/brick/S_indes_brick.png";
    game->brickTextureFile[1] = "data/img/brick/W_brick_0.png";
    game->brickTextureFile[2] = "data/img/brick/W_brick_1.png";
    game->brickTextureFile[3] = "data/img/brick/W_brick_2.png";
    game->barTextureFile[0] = "data/img/bar/B_lego_6x1.png";
    game->barTextureFile[1] = "data/img/bar/R_lego_6x1.png";
    game->ballTextureFile[0] = "data/img/ball/B_lego_rond.png";
    game->ballTextureFile[1] = "data/img/ball/R_lego_rond.png";
    game->backgroundTextureFile[0] = "data/img/background/fond.jpg";
    game->backgroundTextureFile[1] = "data/img/menu/fond_menu.jpg";
    game->barTextureFile[0] = "data/img/bar/B_lego_2x1.png";
    game->barTextureFile[1] = "data/img/bar/R_lego_2x1.png";
    game->barTextureFile[2] = "data/img/bar/B_lego_4x1.png";
    game->barTextureFile[3] = "data/img/bar/R_lego_4x1.png";
    game->barTextureFile[4] = "data/img/bar/B_lego_6x1.png";
    game->barTextureFile[5] = "data/img/bar/R_lego_6x1.png";
    game->barTextureFile[6] = "data/img/bar/B_lego_8x1.png";
    game->barTextureFile[7] = "data/img/bar/R_lego_8x1.png";
    game->barTextureFile[8] = "data/img/bar/B_lego_10x1.png";
    game->barTextureFile[9] = "data/img/bar/R_lego_10x1.png";
    game->ballTextureFile[0] = "data/img/ball/B_lego_rond.png";
    game->ballTextureFile[1] = "data/img/ball/R_lego_rond.png";
    game->lifeTextureFile[0] = "data/img/gameUI/life.png";
    game->lifeTextureFile[1] = "data/img/gameUI/life_empty.png";
    game->brickTextureFile[0] = "data/img/brick/S_indes_brick.png";
    game->brickTextureFile[1] = "data/img/brick/W_brick_0.png";
    game->brickTextureFile[2] = "data/img/brick/W_brick_1.png";
    game->brickTextureFile[3] = "data/img/brick/W_brick_2.png";
    game->bonusTextureFile[0] = "data/img/bonus/bar_up_img.png";
    game->bonusTextureFile[1] = "data/img/bonus/bar_up_txt.png";
    game->bonusTextureFile[2] = "data/img/bonus/bar_down_img.png";
    game->bonusTextureFile[3] = "data/img/bonus/bar_down_txt.png";
    game->bonusTextureFile[4] = "data/img/bonus/bar_speed_up_img.png";
    game->bonusTextureFile[5] = "data/img/bonus/bar_speed_up_txt.png";
    game->bonusTextureFile[6] = "data/img/bonus/slow_time_img.png";
    game->bonusTextureFile[7] = "data/img/bonus/slow_time_txt.png";
    game->bonusTextureFile[8] = "data/img/bonus/accelerate_time_img.png";
    game->bonusTextureFile[9] = "data/img/bonus/accelerate_time_txt.png";
    game->bonusTextureFile[10] = "data/img/bonus/add_ball_img.png";
    game->bonusTextureFile[11] = "data/img/bonus/add_ball_txt.png";
    game->bonusTextureFile[12] = "data/img/bonus/add_life_img.png";
    game->bonusTextureFile[13] = "data/img/bonus/add_life_txt.png";
    game->uiTextureFile[0] = "data/img/gameUI/pause.png";
    game->uiTextureFile[1] = "data/img/gameUI/play_again_on.png";
    game->uiTextureFile[2] = "data/img/gameUI/play_again_off.png";
    game->uiTextureFile[3] = "data/img/gameUI/exit_on.png";
    game->uiTextureFile[4] = "data/img/gameUI/exit_off.png";
    game->uiTextureFile[5] = "data/img/gameUI/exit_off.png";
    game->uiTextureFile[6] = "data/img/gameUI/exit_off.png";
    game->uiTextureFile[7] = "data/img/gameUI/0.png";
    game->uiTextureFile[8] = "data/img/gameUI/1.png";
    game->uiTextureFile[9] = "data/img/gameUI/2.png";
    game->uiTextureFile[10] = "data/img/gameUI/3.png";
    game->uiTextureFile[11] = "data/img/gameUI/3.png";
    game->uiTextureFile[12] = "data/img/gameUI/youwin.png";
    game->uiTextureFile[13] = "data/img/gameUI/youlose.png";
    game->uiTextureFile[14] = "data/img/gameUI/player1wins.png";
    game->uiTextureFile[15] = "data/img/gameUI/player2wins.png";

    int i;
    for (i = 0; i < 2; ++i)
    {
        game->backgroundTexture[i] = generateTexture(&(game->backgroundTexture[i]), game->backgroundTextureFile[i]);
        game->ballTexture[i] = generateTexture(&(game->ballTexture[i]), game->ballTextureFile[i]);
        game->lifeTexture[i] = generateTexture(&(game->lifeTexture[i]), game->lifeTextureFile[i]);
    }
    for (i = 0; i < 4; ++i)
        game->brickTexture[i] = generateTexture(&(game->brickTexture[i]), game->brickTextureFile[i]);
    for (i = 0; i < 10; ++i)
        game->barTexture[i] = generateTexture(&(game->barTexture[i]), game->barTextureFile[i]);
    for (i = 0; i < 14; ++i)
        game->bonusTexture[i] = generateTexture(&(game->bonusTexture[i]), game->bonusTextureFile[i]);
    for (i = 0; i < 16; ++i)
        game->uiTexture[i] = generateTexture(&(game->uiTexture[i]), game->uiTextureFile[i]);

    /* Bars creation */
    Point2D posDWN = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2 - 50);
    Point2D posUP = PointXY(GAME_WIDTH/2 + (WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2 + 50);
    game->bar[0] = createBar(posDWN, game->barTexture[4]);
    game->bar[1] = createBar(posUP, game->barTexture[5]);
    
    /* Players creation */
    game->player[0] = createPlayer(0, "Player 1", &(game->bar[0]));
    game->player[1] = createPlayer(1, "Player 2", &(game->bar[1]));

    /** Balls creation **/ 
    game->ballList = NULL;
    Point2D posBDWN = PointXY(randomNumber(barLeftPosition(&(game->bar[0])), barRightPosition(&(game->bar[0]))), posDWN.y - game->bar[0].height);
    Point2D posBUP = PointXY(randomNumber(barLeftPosition(&(game->bar[1])), barRightPosition(&(game->bar[1]))), posUP.y + game->bar[1].height);
    addBall(&(game->ballList), createBall(posBDWN, VectorXY(0, -0.8), &(game->player[0]), game->ballTexture));
    addBall(&(game->ballList), createBall(posBUP, VectorXY(0, 0.8), &(game->player[1]), game->ballTexture));
    changeAngle (game->ballList, &(game->bar[0]));
    changeAngle (game->ballList->next, &(game->bar[1]));

    /* Bonus list creation */
    game->bonusList = NULL;

    /** Bricks creation **/
    game->brickList = NULL;
    createLevelBricks(level, WINDOW_WIDTH, WINDOW_HEIGHT, &(game->brickList), &(game->bonusList), game->brickTexture, game->bonusTexture);

    /* Direction to control bars and menu *
    * Knowing if a power is on */
    game->direction[0] = game->direction[1] = NONE;
    game->power[0] = game->power[1] = false;
    game->selection = NONE;

    /* Sounds creation */

    game->sound[0] = createSound("data/audio/hitBrick.wav");
    game->sound[1] = createSound("data/audio/hitBar.wav");
    game->sound[2] = createSound("data/audio/barUp.wav");
    game->sound[3] = createSound("data/audio/end.wav");
    game->sound[4] = createSound("data/audio/start.wav");
    game->sound[5] = createSound("data/audio/barDwn.wav");
    game->sound[6] = createSound("data/audio/public.wav");
    game->sound[7] = createSound("data/audio/fastTime.wav");
    game->sound[8] = createSound("data/audio/beep.wav");
    game->sound[9] = createSound("data/audio/bip.wav");
    game->sound[10] = createSound("data/audio/slowPower.wav");
    game->sound[11] = createSound("data/audio/barSpdUp.wav");
    game->sound[12] = createSound("data/audio/addBall.wav");
    game->sound[13] = createSound("data/audio/addLife.wav");
    game->sound[14] = createSound("data/audio/incassable.wav");

    return game;
}

/* RENDER FUNCTIONS */

void drawGameBorder()
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
        glBegin(GL_LINE_LOOP);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2f(-0.5, 0.5);
            glVertex2f(0.5, 0.5);

            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(0.5, -0.5);
            glVertex2f(-0.5, -0.5);
        glEnd();
    glPopMatrix();
}

void drawGameBackground(GLuint backgroundTexture)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef(GAME_WIDTH-1, GAME_HEIGHT-1, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawPause(GLuint uiTexture)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, uiTexture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/2, GAME_TOP_LEFT.y + GAME_HEIGHT/2, 1);
        glScalef((GAME_WIDTH-1), -(GAME_HEIGHT-1), 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_BLEND);
}

void drawWinner(Player player1, unsigned int AI, GLuint* uiTexture)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    if (player1.life != 0 && AI != 0)
        glBindTexture(GL_TEXTURE_2D, uiTexture[12]);
    else if (AI != 0)
        glBindTexture(GL_TEXTURE_2D, uiTexture[13]);
    else if (player1.life != 0)
        glBindTexture(GL_TEXTURE_2D, uiTexture[14]);
    else
        glBindTexture(GL_TEXTURE_2D, uiTexture[15]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - (GAME_HEIGHT/4), 1);
        glScalef(500, -100, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void drawTimer(char *timer, GLuint* texture)
{
    int t = atoi(timer);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    glBindTexture(GL_TEXTURE_2D, texture[6+t]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 1);
        glScalef(100, -120, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void drawRestart(bool restart, GLuint* texture)
{
    Point2D GAME_TOP_LEFT = PointXY((WINDOW_WIDTH-GAME_WIDTH)/2, (WINDOW_HEIGHT-GAME_HEIGHT)/2);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    glBindTexture(GL_TEXTURE_2D, texture[2-restart]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + GAME_WIDTH/3, GAME_TOP_LEFT.y + 2*(GAME_HEIGHT/4), 1);
        glScalef(200, -125, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindTexture(GL_TEXTURE_2D, texture[3+restart]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
        glTranslatef(GAME_TOP_LEFT.x + 2*(GAME_WIDTH/3), GAME_TOP_LEFT.y + 2*(GAME_HEIGHT/4), 1);
        glScalef(200, -125, 1);
        drawSquareTexture();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void drawHit()
{    
    glEnable(GL_BLEND);
    if (hit[0] != 0)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glTranslatef(WINDOW_WIDTH/2, 2*WINDOW_HEIGHT/3, 1);
            glScalef(WINDOW_WIDTH, -2*WINDOW_HEIGHT/2, 1);
            hit[0] -= 2;
            drawGradientSquare(hit[0]);
        glPopMatrix();
    }
    if (hit[1] != 0)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
            glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/3, 1);
            glScalef(WINDOW_WIDTH, 2*WINDOW_HEIGHT/2, 1);
            hit[1] -= 2;
            drawGradientSquare(hit[1]);
        glPopMatrix();
    }
    glDisable(GL_BLEND);
    glColor4f(1.0,1.0,1.0,1.0);
}

void renderGame(Game* game, char timer, bool restart)
{    
    do
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawWindowBackground(game->backgroundTexture[1]);

        glPushMatrix();
        
            glScalef(transition/100,transition/100,1.0);

            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
                drawHit();
                drawGameBackground(game->backgroundTexture[0]);
                drawGameBorder();

                drawAllBalls(game->ballList);

                drawBar(game->player[0].ptBar, game->player[0].num);
                drawBar(game->player[1].ptBar, game->player[1].num);
                if (game->player[0].power != OFF)
                    drawGauge(game->player[0].ptBar, game->player[0].gauge, game->player[0].num);
                if (game->player[1].power != OFF)
                    drawGauge(game->player[1].ptBar, game->player[1].gauge, game->player[1].num);

                drawAllBricks(game->brickList);

                drawAllBonus(game->bonusList);

                drawLife(game->player[0], game->lifeTexture[0]);
                drawLife(game->player[1], game->lifeTexture[0]);

                if (game->pause)
                {
                    drawPause(game->uiTexture[0]);
                }

                glColor3f(1.0, 1.0, 1.0);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        if (transition == 100)
        {
            drawNames(game->player[0].name, game->player[1].name);

            if (timer != '0')
            {
                drawTimer(&timer, game->uiTexture);
            }
        }

        if(game->end)
        {
            drawWinner(game->player[0], game->AI, game->uiTexture);
            drawRestart(restart, game->uiTexture);
        }

        glColor4f(1.0,1.0,1.0,1.0);
        SDL_GL_SwapBuffers();
        if (transition < 100)
            ++transition;
    } while (transition < 100);
}

void bonusManager(BonusList* bonusList, PtBar bar1, PtBar bar2, PtBall* ballList, Mix_Chunk ** sound, GLuint* barTexture)
{
    Bonus bonus;
    BonusList ptBonus = (*bonusList);
    unsigned int channel = 1;
    
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
                ptBonus->actif = false;
                if (bonusPosition == BAR_UP || bonusPosition == BAR_DOWN)
                {
                    getBonus(bonus, ballList, barTexture);
                    ptBonus->animateTxt = 200;

                    // Bonus Sounds
                    if (ptBonus->type == BARUP)
                        playSound(channel, sound[2]);
                    else if (ptBonus->type == BARDWN)
                        playSound(channel, sound[5]);
                    else if (ptBonus->type == FASTPOW)
                        playSound(channel, sound[7]);
                    else if (ptBonus->type == SLOWPOW)
                        playSound(channel, sound[10]);
                    else if (ptBonus->type == BARSPDUP)
                        playSound(channel, sound[11]);
                    else if (ptBonus->type == ADDBALL)
                        playSound(channel, sound[12]);
                    else if (ptBonus->type == ADDLIFE)
                        playSound(channel, sound[13]);
                }
            }
        }
    }
}

int brickManager(PtBall ptBall, PtBrick* brickList, PtBrick ptBrick, GLuint* brickTexture)
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
        return 0;
    }
    else
    {
        if (ptBrick->life == 2)
                ptBrick->texture = brickTexture[2];
        else if (ptBrick->life == 1)
                ptBrick->texture = brickTexture[3];
        return ptBrick->life;
    }
}

Position positionDetection(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtBrick ptBrick, PtPlayer player, GLuint* brickTexture)
{
    Position ballPosition;

    ballPosition = checkBallPosition(ballList, bar1, bar2, ptBrick, player);
    
    if (ptBrick == NULL)
    {
        if (ballPosition == OUT_DOWN)
        {
            loseLife(&player[0]);
            hit[0] = 100;
        }
        else if (ballPosition == OUT_UP)
        {
            loseLife(&player[1]);
            hit[1] = 100;
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
            brickManager(ballList, brickList, ptBrick, brickTexture);
        }
    }

    return ballPosition;
}

Position ballManager(PtBall ballList, PtBar bar1, PtBar bar2, PtBrick* brickList, PtPlayer player, GLuint* brickTexture, Mix_Chunk ** sound)
{
    Position ballPosition = INSIDE;
    PtBrick ptBrick;
    unsigned int channel = 1;

    for(; ballList != NULL; ballList = ballList->next)
    {
        moveBall(ballList);
        
        ballPosition = positionDetection(ballList, bar1, bar2, brickList, NULL, player, brickTexture);
        // If the ball hit something (not a brick), then stop
        if(ballPosition == BAR_UP ||ballPosition == BAR_DOWN || ballPosition == WALL)
        {
            playSound(channel, sound[1]);
            return ballPosition;
        }
        else if(ballPosition == OUT_UP || ballPosition == OUT_DOWN)
        {
            playSound(channel, sound[9]);
            return ballPosition;
        }
            
        ptBrick = *brickList;
        // Check all brick / ball collision
        for(; ptBrick != NULL; ptBrick = ptBrick->next)
        {
            if (ptBrick->life != 0)
            {
                ballPosition = positionDetection(ballList, bar1, bar2, brickList, ptBrick, player, brickTexture);
                if(ballPosition == BRICK)
                {
                    if (ptBrick->type != INDES)
                        playSound(channel, sound[0]);
                    else
                        playSound(channel, sound[14]);
                    return ballPosition;
                }
            }
        } 
        ++channel;   
    }
    
    return ballPosition;
}

// Browse the balls list
Position runGame(Game* game)
{
    Position ballPosition;

    // If no ball then exit function
    if (game->ballList == NULL)
        return -1;
    
    powerManager(game);
    ballPosition = ballManager(game->ballList, &game->bar[0], &game->bar[1], &game->brickList, game->player, game->brickTexture, game->sound);
    bonusManager(&game->bonusList, &game->bar[0], &game->bar[1], &game->ballList, game->sound, game->barTexture);

    return ballPosition;
}

bool gameEvent(Game* game, char timer, State* state)
{
    bool inGame = true;

    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      
      switch(e.type) {
        case SDL_QUIT:
            *state = QUIT;
            inGame = false;
            break;          

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym)
          {
            case SDLK_ESCAPE:
                break;
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
                game->power[0] = true;
                if (game->player[0].power == SLOW)
                {
                    slowGame(game);
                }
                if (game->player[0].power == FAST)
                {
                    accelerateGame(game);
                }
              break;
            case SDLK_z:
                game->power[1] = true;
                if (game->player[1].power == SLOW)
                {
                    slowGame(game);
                }
                if (game->player[1].power == FAST)
                {
                    accelerateGame(game);
                }
              break;
            case SDLK_DOWN:
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
            case SDLK_ESCAPE:
                *state = MENU;
                inGame = false;
                transition = 0.0;
                break;
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
                game->power[0] = false;
                if (game->player[0].power == SLOW)
                {
                    accelerateGame(game);
                }
                if (game->player[0].power == FAST)
                {
                    slowGame(game);
                }
              break;
            case SDLK_z:
                game->power[1] = false;
                if (game->player[1].power == SLOW)
                {
                    accelerateGame(game);
                }
                if (game->player[1].power == FAST)
                {
                    slowGame(game);
                }
              break;
            case SDLK_DOWN:
              break;
            case SDLK_RETURN:
                break;
            case SDLK_SPACE:
              break;
            case SDLK_m:
                if (!mute)
                {
                    Mix_PauseMusic();
                    mute = true;
                }
                else
                {
                    Mix_ResumeMusic();
                    mute = false;
                }
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

bool playGame(Game* game, State* state)
{
    bool inGame = true;
    bool restart = false;
    char timer = '0';
    unsigned int channel = 1;
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    Uint32 ticks_reset = SDL_GetTicks();
    game->end = false;

    if(game->AI)
    {
        if(game->AI == 1)
        {
            game->player[1].ptBar->speed = AI_EASY;
            game->player[1].name = "Computer EASY";
        }
        if(game->AI == 2)
        {
            game->player[1].ptBar->speed = AI_HARD;
            game->player[1].name = "Computer HARD";
        }
    }

    playSound(0, game->sound[6]);
 

    /** Display loop and game management **/
    while(inGame)
    {
        Uint32 startTime = SDL_GetTicks() - ticks_reset;
        channel = 1;

        // Starting time
        if (startTime >= 4500 && startTime <= 5000)
            playSound(channel, game->sound[4]);
        if(startTime < 5100)
        {
            moveBarBall(game->player[0].ptBar, game->ballList, game->direction[0]);
            if(!game->AI) moveBarBall(game->player[1].ptBar, game->ballList->next, game->direction[1]);
            timer = gameLaunch(startTime);
            Mix_Volume(0, MIX_MAX_VOLUME-startTime/50);
        }
        else if(startTime >= 5100 && startTime <= 5500)
        {
            Mix_Volume(0, MIX_MAX_VOLUME/2);
            Mix_HaltChannel(0);
            game->start = true;
        }

        /* Drawing */
        renderGame(game, timer, restart);

        if (game->start && !game->pause)
        {
            game->ballPosition = runGame(game);
            // If a player lose a life
            if (game->ballPosition == OUT_UP || game->ballPosition == OUT_DOWN)
            {                
                // If game ends
                if (game->player[0].life == 0 || game->player[1].life == 0)
                {
                    game->start = false;
                    game->end = true;

                    // SOUND
                    Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
                    playSound(channel, game->sound[3]);
                }
            }
            
            moveBar(game->player[0].ptBar, game->direction[0]);
            if (game->AI) AIcontroller (game->player[1].ptBar, game->ballList);
            else moveBar(game->player[1].ptBar, game->direction[1]);
        }

        if (game->end)
        {
            restart = restartGame(game->selection);
        }

        inGame = gameEvent(game, timer, state);
        
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    return restart;
}

char gameLaunch(Uint32 startTime)
{
    char timer = '5';
    timer = timer-(startTime/1000);

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

    /* Ghost take the previous positions */
    ball->ghost[4] = ball->position;
    ball->ghost[3] = ball->position;
    ball->ghost[2] = ball->position;
    ball->ghost[1] = ball->position;
    ball->ghost[0] = ball->position;

    int i;
    for (i = 9; i > 0; --i)
        bar->ghost[i] = bar->ghost[i-1];
    bar->ghost[0] = bar->position;
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

void slowGame(Game* game)
{
    Ball* tmp = game->ballList;
    while (tmp != NULL)
    {
        tmp->speed = tmp->speed - 4;
        tmp = tmp->next;
    }
}

void accelerateGame(Game* game)
{
    Ball* tmp = game->ballList;
    while (tmp != NULL)
    {
        tmp->speed = tmp->speed + 4;
        tmp = tmp->next;
    }
}

void powerManager(Game* game)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        if (game->player[i].power != OFF && game->power[i] == true)
        {
            if ( --(game->player[i].gauge) == 0)
            {
                if (game->player[i].power == SLOW)
                    accelerateGame(game);
                else if (game->player[i].power == FAST)
                    slowGame(game);
                game->player[i].power = OFF;
            }
        }
    }
}

void freeGameTextures(Game* game)
{
    glDeleteTextures(4, game->brickTexture);
    glDeleteTextures(10, game->barTexture);
    glDeleteTextures(2, game->ballTexture);
    glDeleteTextures(2, game->lifeTexture);
    glDeleteTextures(2, game->backgroundTexture);
    glDeleteTextures(14, game->bonusTexture);
    glDeleteTextures(16, game->uiTexture);
}

void freeGame(Game* game)
{
    freeGameTextures(game);

    deleteBalls(&(game->ballList));
    deleteBrickList(&(game->brickList));
    deleteBonusList(&(game->bonusList));

    int i;
    for (i = 0; i < 15; ++i)
        freeSound(game->sound[i]);
}
