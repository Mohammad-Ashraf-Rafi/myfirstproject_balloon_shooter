#include "iGraphics.h"
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 500
#define MAX_LEVELS 3

bool musicOn = true, effect = false;
int imgHeight = 100, imgWidth = 50;
char mymusic[7][40] = {"music\\dreaming_big.wav", "music\\driving_ambition.wav", "music\\Lights.wav", "music\\Feel-Good.wav",
                       "music\\keys-of-moon-white-petals.wav", "music\\relax-loop-174069.wav", "music\\roa-music-walk-around.wav"};
char balloonImages[7][40] = {"img\\balloons\\balloonsblue.bmp", "img\\balloons\\balloonsdarkblue.bmp", "img\\balloons\\balloonsgreen.bmp",
                             "img\\balloons\\balloonsred.bmp", "img\\balloons\\balloonsorange.bmp", "img\\balloons\\balloonspurple.bmp", "img\\balloons\\balloonsyellow.bmp"};
char bc[20][50] = {"img\\sky\\skyimg2.bmp", "img\\sky\\sunset-reflection.bmp", "img\\sky\\sky_pic4.bmp", "img\\sky\\sunsetsky1.bmp", "img\\sky\\sky_pic6.bmp", // "img\\sky\\sky_pic9.bmp",
                   "img\\sky\\sea_sunshine.bmp", "img\\sky\\nightsky8.bmp", "img\\sky\\anime-scenery.bmp", "img\\sky\\nightsky9.bmp", "img\\sky\\beautynight.bmp",
                   "img\\sky\\star.bmp", "img\\sky\\beautysky.bmp", "img\\sky\\mountainsky.bmp", "img\\sky\\nightsky.bmp", "img\\sky\\highwaysky.bmp", "img\\sky\\nightsky2.bmp"};
char bullets[2][40] = {"img\\bullets\\fireflash.bmp", "img\\bullets\\muzzlered1.bmp"};
char shooter[8][50] = {"img\\shooter\\fortniteflyingshooter.bmp", "img\\shooter\\navysealshooter.bmp", "img\\shooter\\shooter.bmp",
                       "img\\shooter\\shooterflyingonplate.bmp", "img\\shooter\\shooterimg.bmp", "img\\shooter\\shooterimg1.bmp",
                       "img\\shooter\\shooterimg2.bmp", "img\\shooter\\uncleshooter1.bmp"};
char obstacle[3][50] = {"img\\obstacle\\missile2.bmp", "img\\obstacle\\missileyellow3.bmp", "img\\obstacle\\pinmain1.bmp"};

int bcindex = 0, bullindex = 0, shooterindex = 0, musicindex = 0;
int balloonX[1000], balloonY[1000];
int hitStatus[1000] = {0};
int hittedballoons[1000] = {0};
int mouseX, mouseY;
bool isDragging = false;
int bulletposy, bulletposx;
int shooterY = 250, shooterX = 30;
bool bulletout = true, bulletinitial = false;
int speed = 5, hit = 0, bulletspeed = 25, obsspeed = 3, playermove = 15;
int countf = 0, missCount = 0, missbullet = 0;
int obstacleX, obstacleY, obstacleind, countobs = 1, obsinitial = 1, hitable;
double playerlives = 5;
bool initial = true;
int i = 0, color[1000];
int currentPage = 1;
bool pause = false, restart = false, checkhit, exitpopup = false;
int level = 0;

int highscore[MAX_LEVELS];

void levels()
{
    if (level == 1)
    {
        speed = 6;
        obsspeed = 4;
        bulletspeed = 30;
        playermove = 20;
    }
    if (level == 2)
    {
        speed = 7;
        obsspeed = 6;
        bulletspeed = 35;
        playermove = 30;
    }
}
void restartgame()
{
    playerlives = 5;
    hit = 0;
    countf = 0;
    missCount = 0;
    missbullet = 0;
    initial = true;
    i = 0;
    obsinitial = 1;
    for (int i = 0; i < 1000; i++)
    {
        hittedballoons[i] = 0;
        hitStatus[i] = 0;
    }
    bulletposx = 100;
    bulletposy = 2000;
    shooterY = 250;
    bulletout = true;
    bulletinitial = false;
    obstacleX = -1000;
    pause = false;
    restart = false;
}
void ballooninitialize(int index)
{
    balloonX[index] = 300 + rand() % 500;
    balloonY[index] = -imgHeight; // Appear randomly from the bottom
    color[index] = rand() % 7;
    hitStatus[index] = false;
}
void drawBalloons()
{
    if (initial)
    {
        ballooninitialize(i);
        initial = false;
    }

    for (int j = 0; j <= i; j++)
    {
        if (!hitStatus[j] && balloonY[j] < SCREEN_HEIGHT + 10)
        {
            iShowBMP2(balloonX[j], balloonY[j], balloonImages[color[j]], 0);
        }
        if (balloonY[j] >= SCREEN_HEIGHT && !hitStatus[j] && !hittedballoons[j])
        {
            ++missCount;
            playerlives -= 0.5;
            hittedballoons[j] = 1;
        }
    }
}
void drawAnother()
{

    if (balloonY[i] >= 200)
    {
        if (i == 1000)
            i = 0;
        ballooninitialize(++i);
    }
}
void moveBalloons()
{
    for (int j = 0; j <= i; j++)
    {
        if (balloonY[j] <= SCREEN_HEIGHT + 10 && !pause)
            balloonY[j] += speed;
    }
}
void checkBalloonHit()
{
    int tempi1 = i;
    int tempi2 = i - 1;

    if (!hitStatus[tempi1] && bulletposx >= balloonX[tempi1] && bulletposx <= (balloonX[tempi1] + imgWidth) &&
        bulletposy >= (balloonY[tempi1] + 35) && bulletposy <= (balloonY[tempi1] + imgHeight - 10))
    {
        hitStatus[tempi1] = true;
        hit++;
        checkhit = true;
        if (!musicOn && effect)
        {
            PlaySound(TEXT("music\\balloonspop.wav"), NULL, SND_ASYNC);
        }
    }
    if (!hitStatus[tempi2] && bulletposx >= balloonX[tempi2] && bulletposx <= (balloonX[tempi2] + imgWidth) &&
        bulletposy >= (balloonY[tempi2] + 35) && bulletposy <= (balloonY[tempi2] + imgHeight - 10))
    {
        hitStatus[tempi2] = true;
        hit++;
        checkhit = true;
        if (!musicOn && effect)
        {
            PlaySound(TEXT("music\\balloonspop.wav"), NULL, SND_ASYNC);
        }
    }
}
void bulletinitialize()
{
    bulletposx = 100;
    bulletposy = shooterY + 60;
}
void bulletdraw()
{
    iShowBMP2(bulletposx, bulletposy, bullets[bullindex], 0);
}
void bulletmove()
{
    if (!pause && bulletposy <= (1000 + bulletspeed))
    {
        bulletposx += bulletspeed;
    }
}
void obstacle_initialize()
{
    obstacleX = 300;
    obstacleY = 70 + rand() % 400;
    obstacleind = rand() % 3;
    hitable = 1;
}
void obstacle_draw()
{
    if (obsinitial)
    {
        obstacle_initialize();
        obsinitial = 0;
    }
    if (obstacleX >= 0)
    {
        iShowBMP2(obstacleX, obstacleY, obstacle[obstacleind], 0);
        countobs = 1;
    }
    if (obstacleX < -90 && countobs == 1)
    {
        obstacle_initialize();
        countobs = 0;
    }
}
void obstacle_move()
{
    if (obstacleX >= -100 and !pause)
    {
        obstacleX -= obsspeed;
    }
}
void collision_with_obstacle()
{
    if (obstacleY >= (shooterY - 30) && obstacleY <= (shooterY + 90) && obstacleX <= (shooterX + 45) && obstacleX >= 0 && hitable && !pause)
    {
        playerlives--;
        hitable = 0;
        if (!musicOn && effect)
            PlaySound(TEXT("music\\exclamation-of-pain-2204.wav"), NULL, SND_ASYNC);
    }
}

void updateHighScore(int level, int hit)
{
    if (level >= 0 && level < MAX_LEVELS)
    {
        if (hit > highscore[level])
        {
            highscore[level] = hit;

            FILE *file = fopen("leaderboard.txt", "w");
            if (file != NULL)
            {
                for (int i = 0; i < MAX_LEVELS; i++)
                {
                    fprintf(file, "%d\n", highscore[i]);
                }
                fclose(file);
            }
        }
    }
}

void loadHighScores()
{
    FILE *file = fopen("leaderboard.txt", "r");
    if (file != NULL)
    {
        for (int i = 0; i < MAX_LEVELS; i++)
        {
            fscanf(file, "%d", &highscore[i]);
        }
        fclose(file);
    }
}

void showLeaderboard()
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        char scoreText[30];
        iSetColor(255, 255, 255);
        sprintf(scoreText, "%d", highscore[i]);
        iText(530, 380 - i * 105, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void homepage()
{
    iShowBMP(0, 0, "img\\background\\backgroundhome1.bmp");
}
void levelpage()
{
    iShowBMP(0, 0, "img\\background\\levels.bmp");
    shooterindex = rand() % 8;
    bcindex = rand() % 16;
}
void gameoverpage()
{
    iShowBMP(0, 0, "img\\background\\gameover.bmp");
    iSetColor(0, 0, 0);
    char count[100];
    sprintf(count, "%d", hit);
    iText(450, 200, count, GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(118, 57, 40);
    sprintf(count, "high score: %d", highscore[level]);
    iText(400, 150, count, GLUT_BITMAP_TIMES_ROMAN_24);
    updateHighScore(level, hit);
}
void gamepage()
{
    iShowBMP(0, 0, bc[bcindex]);
    iShowBMP2(shooterX, shooterY, shooter[shooterindex], 0);
    if (shooterY > 500)
        shooterY = 0;
    if (shooterY < 0)
        shooterY = 500;
    obstacle_draw();
    if (bulletinitial)
    {
        bulletdraw();
        bulletout = false;
        if (bulletposx > 1000 && !bulletout)
        {
            bulletinitial = false;
            missbullet = countf - hit;
            if (!checkhit)
            {
                playerlives -= 0.5;
            }
            bulletout = true;
        }
    }
    drawBalloons();
    drawAnother();
    checkBalloonHit();
    collision_with_obstacle();
    iShowBMP2(0, 450, "img\\buttons\\pauseandplaybuttons.bmp", 0);
    iSetColor(255, 165, 0);
    iText(750, 450, "score :", GLUT_BITMAP_TIMES_ROMAN_24);
    char count[1000];
    sprintf(count, "%d", hit);
    iText(820, 450, count, GLUT_BITMAP_HELVETICA_18);
    char player[100];
    iSetColor(255, 0, 0);
    iText(750, 425, "lives: ", GLUT_BITMAP_HELVETICA_18);
    sprintf(player, "%.0lf", playerlives);
    iText(800, 425, player, GLUT_BITMAP_HELVETICA_18);
    if (playerlives <= 0)
    {
        PlaySound(TEXT("music\\game-over-arcade-6435.wav"), NULL, SND_ASYNC);
        currentPage = 4;
    }
}
void optionspage()
{
    iShowBMP(0, 0, "img\\background\\musicpage.bmp");
}
void aboutpage()
{
    iShowBMP(0, 0, "img\\background\\aboutmain.bmp");
}
void highscorepage()
{
    iShowBMP(0, 0, "img\\background\\highscore.bmp");
    showLeaderboard();
}
void iDraw()
{
    iClear();
    switch (currentPage)
    {
    case 1:
        homepage();
        break;
    case 2:
        levelpage();
        break;
    case 3:
        gamepage();
        break;
    case 4:
        gameoverpage();
        break;
    case 5:
        optionspage();
        break;
    case 6:
        aboutpage();
        break;
    case 7:
        highscorepage();
    }
    if (pause && currentPage == 3)
    {
        iShowBMP2(350, 175, "img\\background\\pause_popup.bmp", 0);
    }
    if (exitpopup)
    {
        iShowBMP2(340, 150, "img\\background\\exit_popup.bmp", 0);
    }
    if (restart && currentPage != 4)
    {
        restartgame();
    }
}

void iMouseMove(int mx, int my)
{
    printf("x = %d, y = %d\n", mx, my);
    if (isDragging)
    {
        shooterY = my - mouseY;
    }
}
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            if (currentPage == 1)
            {
                if (mx >= 580 && mx <= 720 && my >= 345 && my <= 395 && !exitpopup)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);

                    currentPage = 2;
                }
                if (mx >= 580 && mx <= 720 && my >= 270 && my <= 325 && !exitpopup)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);

                    currentPage = 5;
                }
                if (mx >= 580 && mx <= 720 && my >= 200 && my <= 250 && !exitpopup)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);

                    currentPage = 7;
                }
                if (mx >= 580 && mx <= 720 && my >= 130 && my <= 180 && !exitpopup)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);

                    currentPage = 6;
                }
                if (mx >= 580 && mx <= 720 && my >= 60 && my <= 110 && !exitpopup)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    exitpopup = true;
                }
                if (exitpopup)
                {
                    if (mx >= 380 && mx <= 450 && my >= 180 && my <= 210)
                    {
                        exitpopup = false;
                        exit(0);
                    }
                    if (mx >= 460 && mx <= 530 && my >= 180 && my <= 210)
                    {
                        exitpopup = false;
                    }
                }
            }
            if (currentPage == 2)
            {
                if (mx >= 350 && mx <= 560 && my >= 370 && my <= 450)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    currentPage = 3;
                }
                if (mx >= 350 && mx <= 560 && my >= 250 && my <= 324)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    level = 1;
                    levels();
                    currentPage = 3;
                }
                if (mx >= 350 && mx <= 560 && my >= 120 && my <= 200)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    level = 2;
                    levels();
                    currentPage = 3;
                }
            }
            if (currentPage == 3)
            {
                if (mx >= 0 && mx <= 50 && my >= 450 && my <= 500)
                {
                    pause = true;
                }
                if (pause)
                {
                    if (mx >= 420 && mx <= 520 && my >= 300 && my <= 330 && !exitpopup)
                    {
                        if (!musicOn)
                            PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                        pause = false;
                    }
                    if (mx >= 420 && mx <= 520 && my >= 255 && my <= 285 && !exitpopup)
                    {
                        if (!musicOn)
                            PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                        restart = true;
                        currentPage = 2;
                    }
                    if (mx >= 420 && mx <= 520 && my >= 210 && my <= 240 && !exitpopup)
                    {
                        if (!musicOn)
                            PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                        exitpopup = true;
                    }
                    if (exitpopup)
                    {
                        if (mx >= 380 && mx <= 450 && my >= 180 && my <= 210)
                        {
                            if (!musicOn)
                                PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                            exitpopup = false;
                            currentPage = 1;
                            restart = true;
                        }
                        if (mx >= 460 && mx <= 530 && my >= 180 && my <= 210)
                        {
                            if (!musicOn)
                                PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                            exitpopup = false;
                        }
                    }
                }

                if (mx >= shooterX && mx <= shooterX + 50 && my >= shooterY && my <= shooterY + 100 && !pause)
                {
                    isDragging = true;
                    mouseY = my - shooterY;
                }
                else if (state == GLUT_UP)
                {
                    isDragging = false;
                }
            }
            if (currentPage == 4)
            {
                if (mx >= 370 && mx <= 455 && my >= 90 && my <= 125)
                {
                    currentPage = 1;
                    restart = true;
                    if (musicOn)
                    {
                        PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC);
                    }
                }
                if (mx >= 465 && mx <= 550 && my >= 90 && my <= 125)
                {
                    currentPage = 2;
                    restart = true;
                    if (musicOn)
                    {
                        PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC);
                    }
                }
            }
            if (currentPage == 5)
            {
                if (mx >= 15 && mx <= 80 && my >= 425 && my <= 500)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    currentPage = 1;
                }
                if (mx >= 290 && mx <= 430 && my >= 350 && my <= 405)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    if (!musicOn)
                    {
                        musicOn = true;
                        PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC);
                    }
                }
                if (mx >= 470 && mx <= 615 && my >= 350 && my <= 405)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    if (musicOn)
                    {
                        musicOn = false;
                        PlaySound(0, 0, 0);
                    }
                }
                if (mx >= 655 && mx <= 800 && my >= 350 && my <= 405)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    if (musicOn)
                    {
                        musicindex++;
                        if (musicindex > 6)
                            musicindex = 0;

                        if (musicOn)
                        {
                            PlaySound(0, 0, 0);                                         // Stop the current music
                            PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC); // Start the new music
                        }
                    }
                }
                if (mx >= 385 && mx <= 525 && my >= 195 && my <= 250)
                {
                    if (!musicOn)
                    {
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                        effect = 1;
                    }
                }
                if (mx >= 590 && mx <= 730 && my >= 195 && my <= 250)
                {
                    if (!musicOn)
                    {
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                        effect = 0;
                    }
                }
            }
            if (currentPage == 6)
            {
                if (mx >= 15 && mx <= 90 && my >= 405 && my <= 500)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    currentPage = 1;
                }
            }
            if (currentPage == 7)
            {
                if (mx >= 15 && mx <= 90 && my >= 405 && my <= 500)
                {
                    if (!musicOn)
                        PlaySound("music\\tap-notification-180637.wav", NULL, SND_SYNC);
                    currentPage = 1;
                }
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (key == 'q')
    {
        exit(0);
    }
    if (key == 'n')
    {
        bcindex++;
        if (bcindex > 15)
        {
            bcindex = 0;
        }
    }
    if (key == 'b')
    {
        bcindex--;
        if (bcindex < 0)
        {
            bcindex = 15;
        }
    }
    if (key == 'e')
    {
        bullindex++;
        bullindex = bullindex % 2;
    }
    if (key == 'c')
    {
        shooterindex++;
        if (shooterindex > 7)
        {
            shooterindex = 0;
        }
    }
    if (key == 'v')
    {
        shooterindex--;
        if (shooterindex < 0)
        {
            shooterindex = 7;
        }
    }
    if (bulletout)
    {
        if (key == 'f')
        {
            countf++;
            bulletinitialize();
            // PlaySound(TEXT("music\\pistol-168180.wav"), NULL, SND_ASYNC);
            bulletinitial = true;
            checkhit = false;
        }
    }
    if (key == 'o')
    {
        if (musicOn)
        {
            musicOn = false;
            PlaySound(0, 0, 0);
        }
        else
        {
            musicOn = true;
            PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC);
        }
    }
    if (key == 'm')
    {
        if (musicOn)
        {
            musicindex++;
            if (musicindex > 6)
                musicindex = 0;

            if (musicOn)
            {
                PlaySound(0, 0, 0);                                         // Stop the current music
                PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC); // Start the new music
            }
        }
    }
    if (key == 'p')
    {
        if (!pause)
        {
            pause = true;
        }
        else
            pause = false;
    }
    if (key == 'r')
    {
        restart = true;
        if (currentPage == 3)
        {
            currentPage = 2;
        }
    }
    if (key == 't')
    {
        if (effect)
        {
            effect = false;
        }
        else
            effect = true;
    }
}

void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if (key == GLUT_KEY_DOWN && !pause)
    {
        shooterY -= playermove;
    }
    if (key == GLUT_KEY_UP && !pause)
    {
        shooterY += playermove;
    }
}

int main()
{
    srand(time(NULL));
    ballooninitialize(i);
    iSetTimer(1, moveBalloons);
    iSetTimer(1, bulletmove);
    iSetTimer(1, obstacle_move);
    if (musicOn)
        PlaySound(mymusic[musicindex], NULL, SND_LOOP | SND_ASYNC);
    loadHighScores();
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Balloon Shooting Game");
    return 0;
}
