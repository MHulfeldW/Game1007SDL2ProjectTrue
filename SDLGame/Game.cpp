#include "Game.h"
#include <SDL_image.h>
#include <iostream>
#include "Bullet.h"
#include "Enemy_Bullet.h"
#include "Animation.h"
#include "Boss.h"

Game::Game()
{
}

int Game::run()
{
	// Setting up SDL features...
	int flags = SDL_INIT_EVERYTHING;
	if (SDL_Init(flags) != 0) // if initialized SDL correctly...
	{
		std::cout << "SDL failed to initialize" << std::endl;
		return -1;
	}

	// Create the window
	pWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeX, windowSizeY, SDL_WINDOW_SHOWN);
	if (pWindow == NULL) // If 
	{
		std::cout << "window failed to create" << std::endl;
		return -1;
	}

	// The renderer is needed to draw to the screen
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pRenderer == NULL)
	{
		std::cout << "renderer failed to create" << std::endl;
		return -1;
	}

	int whatWasInitialized = IMG_Init(IMG_INIT_PNG);
	if (whatWasInitialized != IMG_INIT_PNG)
	{
		std::cout << "SDL_img failed to initialize" << std::endl;
		return -1;
	}

	//Setup Mixer for sound
	int mixInitState = Mix_Init(0);
	if (mixInitState != 0)
	{
		printf("Mix Init encountered an error: %s\n", Mix_GetError());
	}
	else
	{
		if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048)!=0)
		{
			printf("Mix Open Audio encountered an error: %s\n", Mix_GetError());
		}
	}

	const char* filePathBGM = "Assets/Sounds/ix.wav";
	
	//Load Audio
	bgm = Mix_LoadWAV(filePathBGM);
	if (bgm == NULL)
	{
		printf("LoadWav for %s encountered an error: %s\n", filePathBGM , Mix_GetError());
	}
	Mix_PlayChannel((int)AudioChannel::MUSIC, bgm, -1);
	Mix_Volume((int)AudioChannel::MUSIC, 64);

	const char* filePathLaser = "Assets/Sounds/laser1.wav";

	laserBlast = Mix_LoadWAV(filePathLaser);
	if (laserBlast == NULL)
	{
		printf("LoadWav for %s encountered an error: %s\n", filePathLaser, Mix_GetError());
	}

	const char* filePathExplosion = "Assets/Sounds/explosion01.wav";
	explosion = Mix_LoadWAV(filePathExplosion);
	const char* filePathAlert = "Assets/Sounds/alert-normal.wav";
	alert = Mix_LoadWAV(filePathAlert);
	const char* filePathBoss = "Assets/Sounds/bosslaser.wav";
	bossLaser = Mix_LoadWAV(filePathBoss);
	const char* filePathBossBGM = "Assets/Sounds/boss bgm.wav";
	bossBGM = Mix_LoadWAV(filePathBossBGM);
	setVolume(volumeScale);

	//Sprites
	myShip = Sprite(pRenderer, "Assets/Spaceship_tut.png");
	myShip.setPosition(windowSizeX/2 - myShip.getSize().x*0.5, windowSizeY - myShip.getSize().y);
	myShip.tag = SpriteTag::PLAYER;

	myBackground = Sprite(pRenderer, "Assets/corona_up.png");
	myBackground.setSize(windowSizeX,windowSizeY);

	myBackground2 = Sprite(pRenderer, "Assets/corona_down.png");
	myBackground2.setSize(windowSizeX, windowSizeY);
	myBackground2.setPosition(0, -myBackground2.getSize().y);

	myBackground3 = Sprite(pRenderer, "Assets/corona_up.png");
	myBackground3.setSize(windowSizeX, windowSizeY);
	myBackground3.setPosition(0, -myBackground2.getSize().y * 2);

	myExplosion = Sprite(pRenderer, "Assets/explosion.png");
	youLose = Sprite(pRenderer, "Assets/Game_Over.png");
	youWin = Sprite(pRenderer, "Assets/You_Win.png");


	bIsRunning = true;

	//Main game loop
	while (bIsRunning)
	{
		input();
		
		update(fixedDeltaTime);

		draw();

		SDL_Delay(fixedDeltaTime * 1000); // wait some milliseconds
		gameTime += fixedDeltaTime;
	}

	cleanup();

	return 0;
}

void Game::input()
{
	//SDL uses an event queue. Process the next thing in the queue using SDL_GetEvents
	//To get another event from the queue, use SDL_PollEvent
	SDL_Event lastEvent;

	//& gives the "memory address of" the thing we use it on. So here we give SDLPollEvent the address of our lastEvent variable 
	while(SDL_PollEvent(&lastEvent))
	{
		//Keydown? There are several parts of a button press. pressing down, held, and then letting go
		if (lastEvent.type == SDL_KEYDOWN)
		{
			const float volumeStep = 0.1f;
			switch (lastEvent.key.keysym.sym)
			{
			case(SDLK_UP):
				setVolume(volumeScale + volumeStep);
				std::cout << "Pressing up raises volume\n";
				break;
			case(SDLK_DOWN):
				setVolume(volumeScale - volumeStep);
				std::cout << "Pressing down lowers volume\n";
				break;
			case(SDLK_SPACE):
				isShootPressed = true;
				break;
			case(SDLK_w):
				isUpPressed = true;
				break;
			case(SDLK_d):
				isRightPressed = true;
				break;
			case(SDLK_s):
				isDownPressed = true;
				break;
			case(SDLK_a):
				isLeftPressed = true;
				break;
			}

		} else if (lastEvent.type == SDL_KEYUP)
		{
			switch (lastEvent.key.keysym.sym)
			{
			case(SDLK_SPACE):
				isShootPressed = false;
				break;
			case(SDLK_w):
				isUpPressed = false;
				break;
			case(SDLK_d):
				isRightPressed = false;
				break;
			case(SDLK_s):
				isDownPressed = false;
				break;
			case(SDLK_a):
				isLeftPressed = false;
				break;
			}
		}
	}
}

void Game::update(const float deltaTime)
{
	
	bool* pBossSpawn = &bossSpawned;
	
	updateBG();
	updateCollisionChecks(deltaTime);

	//Timer for Boss
	if (myShip.isMarkedForDeletion)
	{
		countdown = 0;
	}
	countdown += deltaTime;
	if (countdown < timeForBoss)
	{
		if (myShip.isMarkedForDeletion)
		{
			enemySpawnInterval = 2.0f;
		}
		enemySpawnTimer -= deltaTime;

		if (enemySpawnTimer < 0.0f)
		{
			if (!myShip.isMarkedForDeletion)
			{
				
				spawnEnemy(deltaTime);
				
			}
			enemySpawnTimer = enemySpawnInterval;

			if (enemySpawnInterval > enemySpawnIntervalMin)
			{
				enemySpawnInterval -= 0.1;
			}
		}
	}
  
	updatePlayerActions(deltaTime);
	if(countdown >= timeForBoss + 2.0f)
	{ 
		Mix_HaltChannel((int)AudioChannel::MUSIC);
		if (*pBossSpawn==false)
		{
			
			Mix_PlayChannel((int)AudioChannel::ALERT, alert, 0);
			
			Mix_PlayChannel((int)AudioChannel::BOSSBGM, bossBGM, -1);
			updateBoss();
			*pBossSpawn = true;
			
		}
		spawnBossBullets(deltaTime);
	}	
	if (myShip.isMarkedForDeletion)
	{
		Mix_HaltChannel((int)AudioChannel::BOSSBGM);
		for (int i = 0; i < sprites.size(); i++)
		{
			Sprite* pSprite = sprites[i];

			
			if (pSprite->tag == SpriteTag::BOSS)
			{
				pSprite->isMarkedForDeletion;
				
			}
		}
		*pBossSpawn = false;
		//Worked and broke from one try to the next, even though i made no changes to it
		/*if(Mix_Playing((int)AudioChannel::MUSIC ==  0))
		{ */
			Mix_PlayChannel((int)AudioChannel::MUSIC, bgm, -1);
		/*}*/
	}
		
	
	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite = sprites[i];
	
		if (pSprite->position.y >= windowSizeY/3 - pSprite->getSize().y && pSprite->tag == SpriteTag::BOSS)
		{
			pSprite->position.y = windowSizeY /3 - pSprite->getSize().y;
			int* pOffset = &xOffset;
			
			
			pSprite->position.x = pSprite->position.x + xOffset;
			if (pSprite->position.x >= windowSizeX- pSprite->getSize().x)
			{
				*pOffset = -2;
			}
			else if (pSprite->position.x <= 1)
			{
				*pOffset = 2;
			}
			
		}
	}

	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite = sprites[i];

		if (pSprite->position.x > windowSizeX
			|| pSprite->position.x < 0
			|| pSprite->position.y > windowSizeY
			|| pSprite->position.y < -pSprite->getSize().y
			)
		{
			pSprite->isMarkedForDeletion = true;
		}

		pSprite->update(deltaTime);
	}
	myShip.update(deltaTime);

	//Loop to Destroy sprites as needed
	for (auto iterator = sprites.begin(); iterator != sprites.end();)
	{
		//To get an element from an iterator, use the dereference operator *, which usually turns a pointer into an object
		Sprite* pSprite = (*iterator);

		// -> is a shortcut for dereferencing a pointer, then using the "." operator on it,
		// i.e. a->b is equivalent to (*a).b
		if ((*pSprite).isMarkedForDeletion)
		{
			
			//clear the memory from the image of the sprite!
			pSprite->cleanup();
			// destroy it!
			delete pSprite; // deallocate memory that this pointer points to
			pSprite = nullptr;
			iterator = sprites.erase(iterator); // Remove this element from the container
		}
		else
		{
			iterator++;
		}
	}
	spawnEnemyBullets(deltaTime);

	sprites.shrink_to_fit();
}

void Game::draw()
{
	//Render
	SDL_SetRenderDrawColor(pRenderer, 255, 200, 200, 255); // Choose a color
	SDL_RenderClear(pRenderer); // Clear canvas to color chosen

	myBackground.draw(pRenderer);
	myBackground2.draw(pRenderer);
	myBackground3.draw(pRenderer);

	if (myShip.isMarkedForDeletion)
	{
		youLose.draw(pRenderer);
		youLose.setPosition(windowSizeX / 2 - youLose.getSize().x / 2, windowSizeY / 2 - youLose.getSize().y / 2);
	}
	
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->draw(pRenderer);
	}



	
	myShip.draw(pRenderer);

	SDL_RenderPresent(pRenderer); // Make updated canvas visible on screen
}

void Game::quit()
{
	bIsRunning = false;
}

void Game::cleanup()
{
	// Clean up
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);

	myShip.cleanup();
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->cleanup();
		delete sprites[i];
	}
	myBackground.cleanup();
	myBackground2.cleanup();
	myBackground3.cleanup();
	myExplosion.cleanup();

}



void Game::updatePlayerActions(const float deltaTime)
{
	//acceleration = change in velocity over time
	float acceleration = 3000;
	float deltaV = acceleration * deltaTime;
	

	Vector2 inputVector =
	{
		0,
		0
	};

	if (isUpPressed)
	{
		inputVector.y -= 1;
	}
	if (isRightPressed)
	{
		inputVector.x += 1;
	}
	
	if (isDownPressed)
	{
		inputVector.y += 1;
	
	}
	if (isLeftPressed)
	{
		inputVector.x -= 1;
	}

	//Normalize the input vector by finding its length...
	//Pythagorean theorem sqrt(x^2 + y^2)
	float length = sqrtf(pow(inputVector.x, 2) + pow(inputVector.y, 2));
	float lenghReciprocal = 1.0f / length;

	//Then dividing it by its length (dividing vector by scalar)
	Vector2 inputVectorNormalized =
	{
		inputVector.x * lenghReciprocal,
		inputVector.y * lenghReciprocal
	};

	//Apply acceleration in the direction of that input vector
	Vector2 finalVectorDeltaV =
	{
		inputVector.x * deltaV,
		inputVector.y * deltaV
	};

	myShip.velocity.x += finalVectorDeltaV.x;
	myShip.velocity.y += finalVectorDeltaV.y;

	float damping = 0.9f / deltaTime;
	myShip.velocity.x *= damping * deltaTime;
	myShip.velocity.y *= damping * deltaTime;

	timeBeforeNextShot -= deltaTime;

	if (isShootPressed)
	{
		if (timeBeforeNextShot <= 0.0f)
		{
			//Shoot!
			Mix_PlayChannel((int)AudioChannel::LASER_BLAST, laserBlast, 0);
			std::cout << "Space plays single sound of laser shooting\n";
			
			int projectilesPerShot = 3;
			float spread = 1.0f;
			float bulletSpeed = 400.0f;
			for (int i = 0; i < projectilesPerShot; i++)
			{
				float angle = ((spread / (projectilesPerShot - 1)) * i) + (spread);

				/*std::cout << "shoot!" << std::endl;*/
				Bullet* pNewBullet = new Bullet(pRenderer); // the new keyword creates an instance of that class type, and returns a pointer to it
				// The danger of the new keyword is that we are now responsible for deallocating the memory for this object with the keyword delete
				Sprite* pBulletCastedToSprite = (Sprite*)pNewBullet; // cast from child class to base class pointer

				Vector2 launchPosition = Vector2{ myShip.position.x + (myShip.getSize().x * 0.5f) - (pNewBullet->getSize().x * 0.5f),
													myShip.position.y
				};

				Vector2 launchVelocity = Vector2{ cos(angle) * bulletSpeed,
												  sin(angle) * -bulletSpeed
				};

				pNewBullet->position = launchPosition;
				/*pNewBullet->velocity = launchVelocity;*/

				sprites.push_back(pBulletCastedToSprite);
			}

			timeBeforeNextShot = timeBetweenShots;
		}
	}
	if (myShip.position.x < 0)
	{
		myShip.position.x = 0;
	}
	else if (myShip.position.x > myBackground.getSize().x - myShip.getSize().x)
	{
		myShip.position.x = myBackground.getSize().x - myShip.getSize().x;
	}

	if (myShip.position.y < 0)
	{
		myShip.position.y = 0;
	}
	else if (myShip.position.y > myBackground.getSize().y - myShip.getSize().y)
	{
		myShip.position.y = myBackground.getSize().y - myShip.getSize().y;
	}

}

void Game::updateCollisionChecks(const float deltaTime)
{
	deathTimer -= deltaTime;
	if (deathTimer <= 0.0f)
	{
		myShip.isMarkedForDeletion = false;
		
	}
	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite = sprites[i];
		if (pSprite->tag == SpriteTag::OBSTACLE ||pSprite->tag==SpriteTag::ENEMY_BULLET || pSprite->tag==SpriteTag::BOSS) //Player should only be deleted if touching bullet or enemies
		{	
			
			if (myShip.isCollidingWith(pSprite))
			{
			
				if(!myShip.isMarkedForDeletion)
				{ 
					myShip.isMarkedForDeletion = true;
					deathTimer = timeBetweenDeath;
					//On death we clean the screen to set the game to the default state
					for (int i = 0; i < sprites.size(); i++)
					{
						Sprite* pSprite = sprites[i];
						pSprite->isMarkedForDeletion;
						pSprite->cleanup();
					
					}
				}
				
				pSprite->isMarkedForDeletion = true;
				
			}
		}
	}
	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSpriteA = sprites[i];

		for (int j = i+1; j < sprites.size(); j++)
		{
			Sprite* pSpriteB = sprites[j];

			if (pSpriteA->isCollidingWith(pSpriteB))
			{
				if (pSpriteA->tag == SpriteTag::BULLET   && pSpriteB->tag == SpriteTag::OBSTACLE ||
					pSpriteA->tag == SpriteTag::OBSTACLE && pSpriteB->tag == SpriteTag::BULLET)
				{	
					
					pSpriteA->isMarkedForDeletion = true;
					pSpriteB->isMarkedForDeletion = true; 
					if(pSpriteA->tag == SpriteTag::OBSTACLE || pSpriteB->tag == SpriteTag::OBSTACLE 
					)
					{ 
						Mix_PlayChannel((int)AudioChannel::EXPLOSION, explosion, 0);
						Mix_Volume((int)AudioChannel::EXPLOSION, 22);
					}
				}
				if (pSpriteA->tag == SpriteTag::BOSS && pSpriteB->tag == SpriteTag::BULLET ||
					pSpriteA->tag == SpriteTag::BULLET && pSpriteB->tag == SpriteTag::BOSS)
				{
					Mix_PlayChannel((int)AudioChannel::EXPLOSION, explosion, 0);
					Mix_Volume((int)AudioChannel::EXPLOSION, 22);
					
					*pBossHit++;
				/*	if (pSpriteA->tag == SpriteTag::BULLET)
					{*/
						pSpriteA->isMarkedForDeletion = true;
				/*	}*/
					/*if (pSpriteB->tag == SpriteTag::BULLET)
					{*/
						pSpriteB->isMarkedForDeletion = true;
					/*}*/
					
					/*if (*pBossHit >= 3)
					{
						
						if (pSpriteA->tag == SpriteTag::BOSS)
						{
							pSpriteA->isMarkedForDeletion = true;
							Mix_PlayChannel((int)AudioChannel::EXPLOSION, explosion, 0);
							Mix_Volume((int)AudioChannel::EXPLOSION, 22);
							*pBossHit = 0;
						}
						if (pSpriteB->tag == SpriteTag::BOSS)
						{
							pSpriteB->isMarkedForDeletion = true;
							Mix_PlayChannel((int)AudioChannel::EXPLOSION, explosion, 0);
							Mix_Volume((int)AudioChannel::EXPLOSION, 22);
							*pBossHit = 0;
						}
					
					}*/
				}
				
			}
		}
	}
}

void Game::spawnEnemyBullets(const float deltaTime)
{
	
	for (int i = 0; i < sprites.size(); i++)
	{	
			
		Sprite* pSprite = sprites[i];
		if (pSprite->tag == SpriteTag::OBSTACLE)
		{	
			timeBeforeNextEnemyShot -= deltaTime;
			if(timeBeforeNextEnemyShot<=0.0f && !myShip.isMarkedForDeletion)
			{ 
				timeBeforeNextEnemyShot = timeBetweenEnemyShots;


					Mix_PlayChannel((int)AudioChannel::LASER_BLAST, laserBlast, 0);
					Enemy_Bullet* pNewEnemyBullet = new Enemy_Bullet(pRenderer); // the new keyword creates an instance of that class type, and returns a pointer to it
					// The danger of the new keyword is that we are now responsible for deallocating the memory for this object with the keyword delete
					Sprite* pEnemyBulletCastedToSprite = (Sprite*)pNewEnemyBullet; // cast from child class to base class pointer

					Vector2 launchPosition = Vector2{ pSprite->position.x + (pSprite->getSize().x * 0.5f) - (pNewEnemyBullet->getSize().x * 0.5f),
															pSprite->position.y + pSprite->getSize().y
					};

					pNewEnemyBullet->position = launchPosition;

					sprites.push_back(pEnemyBulletCastedToSprite);

			}
			
		}
	}
	
}

void Game::spawnEnemy(const float deltaTime)
{
	int startVelocityVarianceX = 50;
	int startVelocityY = 200;

	const int NUM_ENEMY_SPRITES = 3;
	const char* enemySpriteImages[NUM_ENEMY_SPRITES] =
	{
		"Assets/small.drone_.1.png",
		"Assets/part2artship1.png",
		"Assets/ship7.png",
	};
	const char* spriteToSpawn = enemySpriteImages[rand() % NUM_ENEMY_SPRITES];

	Sprite* pNewEnemy = new Sprite(pRenderer, spriteToSpawn);
	pNewEnemy->tag = SpriteTag::OBSTACLE;

	//Set start at random position near top of screen
	Vector2 size = pNewEnemy->getSize();

	Vector2 spawnPosition = Vector2{ float(rand() % (windowSizeX - (int)size.x)),
									-size.y
	};
	pNewEnemy->position = spawnPosition;

	//Set start velocity
	Vector2 spawnVelocity = Vector2{ float((rand() % startVelocityVarianceX) - (startVelocityVarianceX / 3)),
									float(startVelocityY)
	};
	pNewEnemy->velocity = spawnVelocity;

	//Add to list of sprites to update/draw
	sprites.push_back(pNewEnemy);
	
	
}

void Game::updateBG()
{
	const int yOffset = 5;
	myBackground.setPosition(0, myBackground.position.y + yOffset);
	myBackground2.setPosition(0, myBackground2.position.y + yOffset);
	myBackground3.setPosition(0, myBackground3.position.y + yOffset);

	if (myBackground.position.y == myBackground.getSize().y)
	{
		myBackground.setPosition(0, -myBackground.getSize().y);
	}
	if (myBackground2.position.y == myBackground2.getSize().y)
	{
		myBackground2.setPosition(0, -myBackground2.getSize().y);
	}
	if (myBackground3.position.y == myBackground3.getSize().y)
	{
		myBackground3.setPosition(0, -myBackground3.getSize().y);
	}
}

void Game::setVolume(float a_volumeScale)
{
	volumeScale = a_volumeScale;
	if (volumeScale < 0.0f)
	{
		volumeScale = 0;
	}
	if (volumeScale > 1.0f)
	{
		volumeScale = 1.0f;
	}

	volumeScale = max(min(volumeScale, 1.0),0.0);
	
	Mix_Volume((int)AudioChannel::MUSIC, baseVolumeMusic * volumeScale);
	Mix_Volume((int)AudioChannel::LASER_BLAST, baseVolumeMusic * volumeScale);
	Mix_Volume((int)AudioChannel::ALERT, baseVolumeMusic * volumeScale);
	Mix_Volume((int)AudioChannel::EXPLOSION, baseVolumeMusic * volumeScale);
	Mix_Volume((int)AudioChannel::BOSS, baseVolumeMusic * volumeScale);
	Mix_Volume((int)AudioChannel::BOSSBGM, baseVolumeMusic * volumeScale);
}

void Game::spawnBossBullets(const float deltaTime)
{
	for (int i = 0; i < sprites.size(); i++)
	{

		Sprite* pSprite = sprites[i];
		if (pSprite->tag == SpriteTag::BOSS)
		{
			timeBeforeNextBossShot -= deltaTime;
			if (timeBeforeNextBossShot <= 0.0f && !myShip.isMarkedForDeletion)
			{
				timeBeforeNextBossShot = timeBetweenBossShots;
				int projectilesPerShot = 3;
				float spread = 1.0f;
				float bulletSpeed = 400.0f;
				for (int i = 0; i < projectilesPerShot; i++)
				{
					float angle = ((spread / (projectilesPerShot - 1)) * i) + (spread);

					Mix_PlayChannel((int)AudioChannel::BOSS, bossLaser, 0);
					Enemy_Bullet* pNewEnemyBullet = new Enemy_Bullet(pRenderer); // the new keyword creates an instance of that class type, and returns a pointer to it
					// The danger of the new keyword is that we are now responsible for deallocating the memory for this object with the keyword delete
					Sprite* pEnemyBulletCastedToSprite = (Sprite*)pNewEnemyBullet; // cast from child class to base class pointer

					Vector2 launchPosition = Vector2{ pSprite->position.x + (pSprite->getSize().x * 0.5f) - (pNewEnemyBullet->getSize().x * 0.5f),
															pSprite->position.y + pSprite->getSize().y
					};
					Vector2 launchVelocity = Vector2{ cos(angle) * bulletSpeed,
											  sin(angle) * bulletSpeed
					};
					pNewEnemyBullet->position = launchPosition;
					pNewEnemyBullet->velocity = launchVelocity;

					sprites.push_back(pEnemyBulletCastedToSprite);

				}


			}

		}
	}
}

void Game::updateBoss()
{
	
		int startVelocity = 200;

		Sprite* pBoss = new Sprite(pRenderer, "Assets/Boss.png");
	
		pBoss->tag = SpriteTag::BOSS;
	
		Vector2 size = pBoss->getSize();
	
		Vector2 spawnPosition = Vector2{float(windowSizeX/2 - ((int)size.x/2)),-size.y};
		pBoss->position = spawnPosition;

		Vector2 spawnVelocity = Vector2{ 0,float(startVelocity)};
		
		pBoss->velocity = spawnVelocity;
		

		sprites.push_back(pBoss);
	
		
}
	
