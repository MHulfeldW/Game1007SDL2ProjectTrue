#include "Boss.h"
#include "Vector2.h"
#include "Game.h"


Boss::Boss(SDL_Renderer* renderer) : Sprite(renderer, "Assets/Boss.png")
{
	velocity.y = 400;
	tag = SpriteTag::BOSS;
	
}

void Boss::update(const float deltaTime)
{
	Sprite::update(deltaTime);
	const char* filePathBoss = "Assets/Sounds/bosslaser.wav";
	bossLaser = Mix_LoadWAV(filePathBoss);
	timeBeforeNextShot -= deltaTime;
	if (timeBeforeNextShot<= 0.0f)
	{
		int projectilesPerShot = 3;
		float spread = 1.0f;
		double bulletSpeed = 400.0f;
		for (int i = 0; i < projectilesPerShot; i++)
		{
			double angle = ((spread / (projectilesPerShot - 1)) * i) + (spread);

			Mix_PlayChannel((int)AudioChannel::BOSS, bossLaser, 0);
			//Enemy_Bullet* pNewEnemyBullet = new Enemy_Bullet(pRenderer); // the new keyword creates an instance of that class type, and returns a pointer to it
			//// The danger of the new keyword is that we are now responsible for deallocating the memory for this object with the keyword delete
			//Sprite* pEnemyBulletCastedToSprite = (Sprite*)pNewEnemyBullet; // cast from child class to base class pointer

			//Vector2 launchPosition = Vector2{ position.x + (getSize().x * 0.5f) - (pNewEnemyBullet->getSize().x * 0.5f),
			//										position.y + getSize().y
			//};
			//Vector2 launchVelocity = Vector2{ cos(angle) * bulletSpeed,
			//						  sin(angle) * bulletSpeed
			//};
			//pNewEnemyBullet->position = launchPosition;
			//pNewEnemyBullet->velocity = launchVelocity;

			//sprites.push_back(pEnemyBulletCastedToSprite);

		}
	}
}
