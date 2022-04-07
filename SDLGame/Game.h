// Welcome to the lab! Today we will try to destroy our targets on collision with bullets
// If you need it for reference, the code from last week has been pushed to GitHub
// 
// 
// At the end of all the labs... we will do..
// -Basic collision detection between sprites... todo
// -Hold space to shoot... done
// -Spawn targets to shoot... done
// -Detect when player touches a target... todo
// -Detect when player bullet touches a target... todo
// -Play music and sounds... todo
// 
// Will NOT show you in the labs
// -How to make enemies shoot back
// -Infinitely scrolling background
// -Sorting sprites
// -Bosses etc.
// -Hitpoints/lives for the player
// -Win/Loss/Progression for the game
// -Screen boundaries


#pragma once
#include <SDL.h> 
#include "Sprite.h"
#include <vector>
#include <SDL_mixer.h>
#include <minmax.h>

enum class AudioChannel
{
	ANY = -1,
	MUSIC,
	LASER_BLAST
};

class Game
{
	int windowSizeX = 800; // Declaring integer called windowSizeX, assigning it the value 800
	int windowSizeY = 900;
	const char* windowName = "Hello SDL";
	SDL_Window* pWindow = nullptr; // Declaring a SDL_Window* (pointer to an object of type SDL_Window) called pWindow, assigning it the value nullptr
	SDL_Renderer* pRenderer = nullptr;
	bool bIsRunning = false;

	Sprite myShipTest;

	Sprite myShip;
	Sprite myBackground;
	Sprite myBackground2;
	Sprite myBackground3;
	std::vector<Sprite*> sprites;

	const float timeBetweenShots = 0.25f; // this represents fire rate (1/fire rate to be exact)
	float timeBeforeNextShot = 0.0f; // this will tick down
	const float timeBetweenEnemyShots = 0.72f;
	float timeBeforeNextEnemyShot = 0.0f;

	float gameTime = 0.0f; // seconds since start of game
	float fixedDeltaTime = 0.016f; // time between frames 0.016 == 1/60

	bool isShootPressed = false;
	bool isUpPressed	= false;
	bool isDownPressed	= false;
	bool isLeftPressed	= false;
	bool isRightPressed	= false;

	bool isShooting = false;

	//Enemy spawns
	float enemySpawnTimer = 0.0f;
	float enemySpawnInterval = 2.0f;
	const float enemySpawnIntervalMin = 0.2f;

	//Audio
	Mix_Chunk* bgm;
	Mix_Chunk* laserBlast;

	float volumeScale = 0.5f;
	float baseVolumeLaser = 32.0f;
	float baseVolumeMusic = 64.0f;

public:
	Game();

	int run();
	void input();
	void update(const float deltaTime);
	void draw();
	void quit();
	void cleanup();

	void setVolume(float a_volumeScale);

	void updatePlayerActions(const float deltaTime);
	void updateCollisionChecks();
	void spawnEnemyBullets(const float deltaTime);
	void spawnEnemy(const float deltaTime);
	void updateBG();
};

