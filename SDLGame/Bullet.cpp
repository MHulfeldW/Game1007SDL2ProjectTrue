#include "Bullet.h"

Bullet::Bullet(SDL_Renderer* renderer) : Sprite(renderer, "Assets/Lasers/laserblue01.png")
{
	velocity.y = -400;
	tag = SpriteTag::BULLET;
}
