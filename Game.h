#ifndef GAME_H_
#define GAME_H_

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "Board.h"
#include "Paddle.h"
#include "Ball.h"
#include "liver.h"

#define FPS_DELAY 500

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();
    void heart();
    int number = 0;
    char str[128];
    int highcore[8] = { 0 };
    char str1[128];
    int score;
    int menugame();
    int intro();
    void rank();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* texture;
    SDL_Texture* hearttexture;
    // Timing
    unsigned int lasttick, fpstick, fps, framecount;

    // Test
    float testx, testy;

    Board* board;
    Paddle* paddle;
    Ball* ball;
    liver* soluot;
 
    bool paddlestick;


    // SoVGA keep track to optimize update parameter
    int oldBrickCount = -1;

    void Clean();
    void Update(float delta);
    void Render(float delta);

    void NewGame();
    void ResetPaddle();
    void StickBall();

    void SetPaddleX(float x);
    void CheckBoardCollisions();
    float GetReflection(float hitx);
    void CheckPaddleCollisions();
    void CheckBrickCollisions();
    void CheckBrickCollisions2();
    void BallBrickResponse(int dirindex);
    int GetBrickCount();
    void amvacham();
    void amroi();
    void amstart();
    void diem(int x,int y);
    bool check(const int& x, const int& y, const SDL_Rect& rect);
    // void PlayOneShotSound(const char* name);
};

#endif
