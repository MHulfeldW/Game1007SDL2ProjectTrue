#include "Bullet.h"

Bullet::Bullet(SDL_Renderer* renderer) : Sprite(renderer, "Assets/Lasers/laserBlue01.png")
{
	velocity.y = -400;
	tag = SpriteTag::BULLET;
}
