#include "Enemy_Bullet.h"

Enemy_Bullet::Enemy_Bullet(SDL_Renderer* renderer) : Sprite(renderer, "Assets/Lasers/laserGreen11.png")
{
	velocity.y = 400;
	tag = SpriteTag::ENEMY_BULLET;
}
