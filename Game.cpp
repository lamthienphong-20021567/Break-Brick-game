#include "Game.h"
#include<cstring>
#include<ctime>
#include<cstdlib>


Game::Game() {

    window = 0;
    renderer = 0;
}

Game::~Game() {

}


bool Game::Init() {
    SDL_Init(SDL_INIT_EVERYTHING);

    // Create window
    window = SDL_CreateWindow("Bricks Breaker 2.0",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!window) {
        std::cout << "Error creating window:" << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return false;
    }
    // Initialize resources
    SDL_Surface* surface = IMG_Load("white.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Initialize timing
    lasttick = SDL_GetTicks();
    fpstick = lasttick;
    fps = 0;
    framecount = 0;
    testx = 0;
    testy = 0;
    if (intro() != 1) return false;
   return true;
}

      
void Game::Clean() {
    // Clean resources
    SDL_DestroyTexture(texture);

    // Clean renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}

void Game::Run() {

        board = new Board(renderer);
        paddle = new Paddle(renderer);
        ball = new Ball(renderer);
        soluot = new liver(renderer);
        number = 0;
        bool pause = false;

        NewGame();
        //music
        Mix_Music* music = NULL;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        {
            printf("%s", Mix_GetError());
        }

        music = Mix_LoadMUS("music01.mp3");
        if (music == NULL)
        {
            printf("%s", Mix_GetError());
        }
        Mix_PlayMusic(music, -1);
        // Main loop
        while (1) {
            // Handler events
            SDL_Event e;
            if (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    break;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_RETURN)//Press "enter" to pause music.
                    {
                        Mix_PauseMusic();
                    }
                    if (e.key.keysym.sym == SDLK_SPACE)//"SPACE" to continue music
                        Mix_ResumeMusic();
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        pause = true; Mix_PauseMusic();
                    }
                    if (e.key.keysym.sym == SDLK_F1)
                    {
                        pause = false; Mix_ResumeMusic();
                    }
                }
            }
            // Calculate delta and fps
            unsigned int curtick = SDL_GetTicks();
            float delta = (curtick - lasttick) / 1000.0f;
            if (curtick - fpstick >= FPS_DELAY) {
                fps = framecount * (1000.0f / (curtick - fpstick));
                fpstick = curtick;
                framecount = 0;
                //std::cout << "FPS: " << fps << std::endl;
                char buf[100];
                snprintf(buf, 100, "Break Brick demo (fps: %u) (bricks: %1.02f)", fps, (float)oldBrickCount / (BOARD_WIDTH * BOARD_HEIGHT));
                SDL_SetWindowTitle(window, buf);
            }
            else {
                framecount++;
            }
            lasttick = curtick;

            // Update and render the game
            if (pause == false) {
                Update(delta);
                Render(delta);
            }

        }
        delete board;
        delete paddle;
        delete ball;
        delete soluot;

        Clean();
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
            
}


void Game::NewGame() {
  
    board->CreateLevel();
    ResetPaddle();
}

void Game::ResetPaddle() {
    paddlestick = true;
    StickBall();
}

void Game::StickBall() {
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

void Game::Update(float delta) {
    // Game logic

    // Input
    int mx, my;
    Uint8 mstate = SDL_GetMouseState(&mx, &my);
    SetPaddleX(mx - paddle->width/2.0f);

    if (mstate&SDL_BUTTON(1)) {
        if (paddlestick) {
            paddlestick = false;
            ball->SetDirection(1, -1);
        }
    }

    if (paddlestick) {
        StickBall();
    }

    CheckBoardCollisions();
    CheckPaddleCollisions();
    CheckBrickCollisions2();

    if (GetBrickCount() == 0) {
        Mix_PausedMusic();
        number += 200;
        score = number;
        bool quit = false;
        bool isRunning = true;
        SDL_Event event;

        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_JPG);
        TTF_Init();
        SDL_Surface* image = IMG_Load("winner.png");
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        Mix_Chunk* chunk = NULL;
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        {
            printf("%s", Mix_GetError());
        }
        chunk = Mix_LoadWAV("win01.wav");
        if (chunk == NULL)
        {
            printf("%s", Mix_GetError());
            // return -1;
        }
       // if (!Mix_Playing(-1))
            Mix_PlayChannel(-1, chunk, 0);

            TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
            SDL_Color color1 = { 255, 255, 255 };
            SDL_Surface* surface2 = TTF_RenderUTF8_Blended(font, "Press SPACE to rank and start new game, ESC to exit", color1);
            SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
            SDL_FreeSurface(surface2);
            int tex01 = 0;
            int tex1 = 0;
            SDL_QueryTexture(texture2, NULL, NULL, &tex01, &tex1);
            SDL_Rect rect1 = { 150,600 , tex01, tex1 };
            SDL_RenderCopy(renderer, texture2, NULL, &rect1);

            while (isRunning) {
                diem(50, 60);
                SDL_RenderPresent(renderer);
                while (!quit)
                {
                    SDL_WaitEvent(&event);
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_SPACE)
                            Run();
                        if (event.key.keysym.sym == SDLK_RETURN)
                            rank();
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            SDL_Quit();
                        break;
                    }


                }
            }
 
        SDL_DestroyTexture(texture);
        Mix_CloseAudio();
        SDL_FreeSurface(image);
        IMG_Quit();
    
    }

    board->Update(delta);
    paddle->Update(delta);

    if (!paddlestick) {
        ball->Update(delta);
    }
}

void Game::SetPaddleX(float x) {
    float newx;
    if (x < board->x) {
        // Upper bound
        newx = board->x;
    } else if (x + paddle->width > board->x + board->width) {
        // Lower bound
        newx = board->x + board->width - paddle->width;
    } else {
        newx = x;
    }
    paddle->x = newx;
}

void Game::CheckBoardCollisions() {
    // Top and bottom collisions
    if (ball->y < board->y) {
        // Top
        // Keep the ball within the board and reflect the y-direction
        ball->y = board->y;
        ball->diry *= -1;


    }
    else if (ball->y + ball->height > board->y + board->height) {
        // Bottom
        soluot->live--;
        if (soluot->live == 0)
        {
            Mix_PauseMusic();
            // picture & music game over
            bool quit = false;
            SDL_Event event;
            bool isRunning = true;

            score = number;
            SDL_Init(SDL_INIT_VIDEO);
            IMG_Init(IMG_INIT_JPG);
            TTF_Init();
            SDL_Surface* image = IMG_Load("gameover.jpg");

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            Mix_Chunk* chunk = NULL;
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
            {
                printf("%s", Mix_GetError());
            }
            chunk = Mix_LoadWAV("ohno.wav");
            if (chunk == NULL)
            {
                printf("%s", Mix_GetError());
                // return -1;
            }
            // if (!Mix_Playing(-1))
            Mix_PlayChannel(-1, chunk, 0);
            TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
            SDL_Color color1 = { 255, 255, 255 };
            SDL_Surface* surface2 = TTF_RenderUTF8_Blended(font, "Press SPACE to rank and start new game, ESC to exit", color1);
            SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
            SDL_FreeSurface(surface2);
            int tex01 = 0;
            int tex1 = 0;
            SDL_QueryTexture(texture2, NULL, NULL, &tex01, &tex1);
            SDL_Rect rect1 = {150,500 , tex01, tex1 };
            SDL_RenderCopy(renderer, texture2, NULL, &rect1);
            while (isRunning) {
                 diem(50, 60);
                SDL_RenderPresent(renderer);
                while (!quit)
                {
                    SDL_WaitEvent(&event);

                    switch (event.type)
                    {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_SPACE)
                            rank();
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            SDL_Quit();
                        break;
                    }
                }
                                  
            }
  
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(image);
            Mix_CloseAudio();
            IMG_Quit();

        }
        else
        {
            amroi();
            if (number >= 50) number = number - 50;
            else number = number;
            diem(16,18);
            ResetPaddle();
        }

    }

    // Left and right collisions
    if (ball->x <= board->x) {
        // Left
        // Keep the ball within the board and reflect the x-direction
        ball->x = board->x;
        ball->dirx *= -1;

    } else if (ball->x + ball->width >= board->x + board->width) {
        // Right
        // Keep the ball within the board and reflect the x-direction
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;
    }
}

float Game::GetReflection(float hitx) {
    // Make sure the hitx variable is within the width of the paddle
    if (hitx < 0) {
        hitx = 0;
    } else if (hitx > paddle->width) {
        hitx = paddle->width;
    }

    // Everything to the left of the center of the paddle is reflected to the left
    // while everything right of the center is reflected to the right
    hitx -= paddle->width / 2.0f;

    // Scale the reflection, making it fall in the range -2.0f to 2.0f
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions() {
    // Get the center x-coordinate of the ball
    float ballcenterx = ball->x + ball->width / 2.0f;

    // Check paddle collision
    if (ball->Collides(paddle)) {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
    }
}

void Game::CheckBrickCollisions() {
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];

            // Check if brick is present
            if (brick.state) {
                // Brick x and y coordinates
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // Check ball-brick collision
                // Determine the collision using the half-widths of the rectangles
                // http://stackoverflow.com/questions/16198437/minkowski-sum-for-rectangle-intersection-calculation
                // http://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
                // http://gamedev.stackexchange.com/questions/24078/which-side-was-hit/24091#24091
                float w = 0.5f * (ball->width + BOARD_BRWIDTH);
                float h = 0.5f * (ball->height + BOARD_BRHEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BOARD_BRWIDTH);
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BOARD_BRHEIGHT);

                if (fabs(dx) <= w && fabs(dy) <= h) {
                    // Collision detected
                    board->bricks[i][j].state = false;
                    float wy = w * dy;
                    float hx = h * dx;

                    if (wy > hx) {
                        if (wy > -hx) {
                            // Bottom (y is flipped)
                            BallBrickResponse(3);
                        } else {
                            // Left
                            BallBrickResponse(0);
                        }
                    } else {
                        if (wy > -hx) {
                            // Right
                            BallBrickResponse(2);
                        } else {
                            // Top (y is flipped)
                            BallBrickResponse(1);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::CheckBrickCollisions2() {
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];

            // Check if brick is present
            if (brick.state) {
                // Brick x and y coordinates
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // Center of the ball x and y coordinates
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                // Center of the brick x and y coordinates
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky) {
                    // Collision detected, remove the brick
                    board->bricks[i][j].state = false;
                    amvacham();
                    number += 100;
                    diem(16,18);


                    // Assume the ball goes slow enough to not skip through the bricks

                    // Calculate ysize
                    float ymin = 0;
                    if (bricky > ball->y) {
                        ymin = bricky;
                    } else {
                        ymin = ball->y;
                    }

                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height) {
                        ymax = bricky+BOARD_BRHEIGHT;
                    } else {
                        ymax = ball->y+ball->height;
                    }

                    float ysize = ymax - ymin;

                    // Calculate xsize
                    float xmin = 0;
                    if (brickx > ball->x) {
                        xmin = brickx;
                    } else {
                        xmin = ball->x;
                    }

                    float xmax = 0;
                    if (brickx+BOARD_BRWIDTH < ball->x+ball->width) {
                        xmax = brickx+BOARD_BRWIDTH;
                    } else {
                        xmax = ball->x+ball->width;
                    }

                    float xsize = xmax - xmin;

                    // The origin is at the top-left corner of the screen!
                    // Set collision response
                    if (xsize > ysize) {
                        if (ballcentery > brickcentery) {
                            // Bottom
                            ball->y += ysize + 0.01f; // Move out of collision
                            BallBrickResponse(3);
                        } else {
                            // Top
                            ball->y -= ysize + 0.01f; // Move out of collision
                            BallBrickResponse(1);
                        }
                    } else {
                        if (ballcenterx < brickcenterx) {
                            // Left
                            ball->x -= xsize + 0.01f; // Move out of collision
                            BallBrickResponse(0); 
                        } else {
                            // Right
                            ball->x += xsize + 0.01f; // Move out of collision
                            BallBrickResponse(2); 
                        }
                    }

                    return;
                }
            }
        }
    }
}

void Game::BallBrickResponse(int dirindex) {
    // dirindex 0: Left, 1: Top, 2: Right, 3: Bottom

    // Direction factors
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0) {
        // Ball is moving in the positive x direction
        if (ball->diry > 0) {
            // Ball is moving in the positive y direction
            // +1 +1
            if (dirindex == 0 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball is moving in the negative y direction
            // +1 -1
            if (dirindex == 0 || dirindex == 1) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    } else if (ball->dirx < 0) {
        // Ball is moving in the negative x direction
        if (ball->diry > 0) {
            // Ball is moving in the positive y direction
            // -1 +1
            if (dirindex == 2 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball is moving in the negative y direction
            // -1 -1
            if (dirindex == 1 || dirindex == 2) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    }

    // Set the new direction of the ball, by multiplying the old direction
    // with the determined direction factors
    ball->SetDirection(mulx*ball->dirx, muly*ball->diry);

}

int Game::GetBrickCount() {
    int brickcount = 0;
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];
            if (brick.state) {
                brickcount++;
            }
        }
    }

    if (brickcount != oldBrickCount)
    {
        oldBrickCount = brickcount;
    }
    return brickcount;
}

void Game::Render(float delta) {
    SDL_RenderClear(renderer);

    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);
    soluot->Render(delta);

    diem(16,18);
    SDL_RenderPresent(renderer);
}
void Game::amvacham()
{
    Mix_Chunk* chunk = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }
    chunk = Mix_LoadWAV("vacham.wav");
    if (chunk == NULL)
    {
        printf("%s", Mix_GetError());
        // return -1;
    }
        Mix_PlayChannel(-1, chunk, 0);
}
void Game::amroi()
{
    Mix_Chunk* chunk = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }
    chunk = Mix_LoadWAV("roixuong.wav");
    if (chunk == NULL)
    {
        printf("%s", Mix_GetError());
        // return -1;
    }
        Mix_PlayChannel(-1, chunk, 0);
}
void Game::diem(int x,int y)
{
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("arial.ttf", y);
    SDL_Color color = { 255, 255, 255 }; 
    sprintf_s(str,"SCORE: %d",number);
    SDL_Surface* surface1 = TTF_RenderUTF8_Blended(font, str, color); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface1);
    SDL_FreeSurface(surface1);
    surface1 = NULL;
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect rect_ = { x, 0, texW, texH }; 
    SDL_RenderCopy(renderer, texture, NULL, &rect_);
   // SDL_BlitSurface(surface1, NULL, screen, &rect_);
    SDL_DestroyTexture(texture);
    texture = NULL;
    //SDL_RenderPresent(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
  
}
int Game::menugame()
{
    bool quit1 = false, select = 0;
    SDL_Event event1;
    int xm, ym, k = 2;
    SDL_Surface* image = IMG_Load("bbreaker.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("arial.ttf", 25);
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface1[3];
    SDL_Rect rect_[3];
    SDL_Texture* texture_[3];
        surface1[0] = TTF_RenderUTF8_Blended(font, "Click here or press space to PLAY", color);
        texture_[0] = SDL_CreateTextureFromSurface(renderer, surface1[0]);
        SDL_FreeSurface(surface1[0]);

        surface1[1] = TTF_RenderUTF8_Blended(font, "Press esc to exit", color);
        texture_[1] = SDL_CreateTextureFromSurface(renderer, surface1[1]);
        SDL_FreeSurface(surface1[1]);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture_[0], NULL, NULL, &texW, &texH);
        SDL_QueryTexture(texture_[1], NULL, NULL, &texW, &texH);
   
        rect_[0] = { 16, 400, texW, texH };
        rect_[1] = { 16, 450, texW, texH };
        rect_[2] = { 16, 500, texW, texH };
        // SDL_BlitSurface(surface1, NULL, screen, &rect_);
        SDL_RenderCopy(renderer, texture_[0], NULL, &rect_[0]);
        SDL_RenderCopy(renderer, texture_[1], NULL, &rect_[1]);

        SDL_DestroyTexture(texture_[0]);
        SDL_DestroyTexture(texture_[1]);


    TTF_CloseFont(font);
    TTF_Quit();
    while (!quit1)
    {
        //SDL_WaitEvent(&event1);
        while (SDL_PollEvent(&event1))
        {
            switch (event1.type)
            {
            case SDL_QUIT:
                quit1 = true;
                break;
          
            case SDL_MOUSEBUTTONDOWN:
                xm = event1.button.x;
                ym = event1.button.y;
                    if (check(xm, ym, rect_[0])) {
                        amstart();
                        return 1;
                }
                break;
            case SDL_KEYDOWN:
                if (event1.key.keysym.sym == SDLK_ESCAPE)
                {
                    SDL_Quit();
                }
                if (event1.key.keysym.sym == SDLK_SPACE)
                {
                    amstart();
                    return 1;
                }

                break;
               default:
                break;
            }
        }
           SDL_RenderPresent(renderer);
        
    }
    return 1;
}
int Game::intro()
{
    bool quit1 = false;
    SDL_Event event1;
    int xm, ym;
    SDL_Surface* image = IMG_Load("white002.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);   
    while (!quit1)
    {
        SDL_WaitEvent(&event1);

        switch (event1.type)
        {
        case SDL_QUIT:
            quit1 = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            xm = event1.button.x;
            ym = event1.button.y;
            return 1;
            break;
        }

        SDL_RenderPresent(renderer);

    }
    return 1;
}
bool Game::check(const int& x, const int& y, const SDL_Rect& rect)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
    {
        return true;
    }
    return false;
}

void sapxep(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void Game::rank()
{
    bool quit1 = false;
    SDL_Event event1;
    SDL_Surface* image = IMG_Load("black.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
    TTF_Font* font1 = TTF_OpenFont("arial.ttf",80);
    SDL_Color color = { 255, 255, 255 };
    SDL_Color color1 = { 255, 0, 0 };

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font1, "RANK", color1);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;
    int tex0 = 0;
    int tex = 0;
    SDL_QueryTexture(texture1, NULL, NULL, &tex0, &tex);
    SDL_Rect rect = { 380, 0, tex, tex };
    SDL_RenderCopy(renderer, texture1, NULL, &rect);

    SDL_Surface* surface2 = TTF_RenderUTF8_Blended(font, "Press SPACE to start new game or ESC to exit", color1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface2);
    surface = NULL;
    int tex01 = 0;
    int tex1 = 0;
    SDL_QueryTexture(texture2, NULL, NULL, &tex01, &tex1);
    SDL_Rect rect1 = { 200,500 , tex01, tex1 };
    SDL_RenderCopy(renderer, texture2, NULL, &rect1);

    char s2[128][128];
 
    SDL_Surface* surface1[8];
    SDL_Rect rect_[8];
    SDL_Texture* texture_[8];
    if (score > highcore[7]) highcore[7] = score;
    for (int i = 0; i < 8; i++)
    {
        for (int j = i + 1; j < 8; j++)
        {
            if (highcore[i] < highcore[j])
            {
                sapxep(&highcore[i], &highcore[j]);
            }
        }
    }
    for (int i = 0; i < 8; i++) 
    {
        sprintf_s(s2[i], "TOP %d:.............................................%d",i+1, highcore[i]);
        surface1[i] = TTF_RenderUTF8_Blended(font, s2[i], color);
        texture_[i] = SDL_CreateTextureFromSurface(renderer, surface1[i]);
        SDL_FreeSurface(surface1[i]);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture_[i], NULL, NULL, &texW, &texH);
        rect_[i] = { 16+i*20, 100+i*50, texW, texH };
        // SDL_BlitSurface(surface1, NULL, screen, &rect_);
        SDL_RenderCopy(renderer, texture_[i], NULL, &rect_[i]);
        SDL_DestroyTexture(texture_[i]);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    while (!quit1)
    {
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event1))
        {
            switch (event1.type)
            {
            case SDL_QUIT:
                quit1 = true;
                break;

            case SDL_KEYDOWN:
                if (event1.key.keysym.sym == SDLK_SPACE)
                    Run();
                if (event1.key.keysym.sym == SDLK_ESCAPE)
                    SDL_Quit();
                break;
            }
        }

    }

}
void Game::amstart()
{
    Mix_Music* music = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }
    music = Mix_LoadMUS("start.mp3");
    if (music == NULL)
    {
        printf("%s", Mix_GetError());
        // return -1;
    }
    Mix_PlayMusic(music, -1);
}



