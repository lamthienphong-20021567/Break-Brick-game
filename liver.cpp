#include "liver.h"

liver::liver(SDL_Renderer* renderer) : Entity(renderer) {
    SDL_Surface* surface = IMG_Load("smallh.png");
    hearttexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

}

liver::~liver() {
    // Clean resources
    SDL_DestroyTexture(hearttexture);
}

void liver::Update(float delta) {

}

void liver::Render(float delta) {
    // Render heart
    for (int i = 1; i <= live; i++)
    {
        SDL_Rect srcrect;

        srcrect.x = 0;
        srcrect.y = 0;
        srcrect.w = 50;
        srcrect.h = 50;

        SDL_Rect dstrect;
        dstrect.x = 800 - 10*2*i;
        dstrect.y = 0;
        dstrect.w = 20;
        dstrect.h = 17;
        SDL_RenderCopy(renderer, hearttexture, &srcrect, &dstrect);
    }
}

