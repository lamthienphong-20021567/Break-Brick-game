#ifndef LIVER_H_
#define LIVER_H_

#include "Entity.h"
#include <stdlib.h>

class liver : public Entity {
public:
    liver(SDL_Renderer* renderer);
    ~liver();

    void Update(float delta);
    void Render(float delta);
    int live = 5;

private:
    SDL_Texture* hearttexture;
};

#endif

