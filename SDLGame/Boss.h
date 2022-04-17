#pragma once
#include "Sprite.h"
#include "Game.h"
#include "Enemy_Bullet.h"

class Boss :
    public Sprite
{
public:
    Boss(SDL_Renderer* renderer);
    const float timeBetweenShots = 1.0f; 
    float timeBeforeNextShot = 0.0f;
    Mix_Chunk* bossLaser;
    void update(const float deltaTime);
};

