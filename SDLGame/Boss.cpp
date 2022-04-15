#include "Boss.h"
#include "Vector2.h"
#include "Game.h"


Boss::Boss(SDL_Renderer* renderer) : Sprite(renderer, "Assets/large.ship_.1.png")
{
	velocity.y = 400;
	tag = SpriteTag::BOSS;
}