#ifndef ENTITY_H_
#define ENTITY_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

class Entity {
public:
    Entity(SDL_Renderer* renderer);
    virtual ~Entity();

    float x, y, width, height;

    virtual void Update(float delta);
    virtual void Render(float delta);

    bool Collides(Entity* other);
protected:
    SDL_Renderer* renderer;

};

#endif
