#pragma once
#include <SDL_image.h>
#include "Vector2.h"
//Sprite class
//A sprite is an image with a position that can be moved around, and drawn to the screen




/*
A Sprite contains data for (data members):
- Image
- Position
- Size
- FlipState
- RotationDegrees

A Sprite can do (functions):
- moveBy(xOffset, yOffset)
- setPosition(x, y)
- setSize(width, height)
- setRotation(degrees)
- setImage(filename)

	ship2 =  Sprite(pRenderer, "ship2.png")
	ship2.setPosition(50,500)
	ship2.draw(pRenderer);
	ship2.moveBy(2, 0)

*/
enum class SpriteTag
{
	NONE = -1,
	PLAYER,
	ENEMY,
	ENEMY_BULLET,
	OBSTACLE,
	BULLET,
	BOSS
};

class Sprite
{
private:
	SDL_Texture* pTexture;
	SDL_Rect src; // source rectangle for rendering. Represents the part of the texture to draw when rendering this sprite
	SDL_Rect dst; // destination rectangle for rendering. Represents where the sprite will be drawn on-screen, and how big it will be when drawn
	int m_iFrame = 0,
		m_iFrameMax = 4,
		m_iSprite = 0,
		m_iSpriteMax = 11;

public:
	bool isMarkedForDeletion = false;
	Vector2 position = { 0, 0 };
	Vector2 velocity = { 0, 0 };

	SpriteTag tag = SpriteTag::NONE;

	//Constructors. Special functions used when creating an object of this class.
	Sprite(); // Default constructor
	Sprite(SDL_Renderer* pRenderer, const char* filename); // 

	//Member Functions. <return type><name>(<parameters>)
	
	//Accessor or 'getter' function (only gets info about the object
	Vector2 getSize();

	//designed so we can write lines which look like regular language e.g ship->isCollidingWith(asteroid)
	bool isCollidingWith(Sprite* other);
	
	//Modifier or 'setter' function (changes something about the object)
	void setPosition(const float x, const float y);
	void moveBy(const float xOffset, const float yOffset);
	void setSize(int width, int height);
	void animate(Sprite obj, Sprite obj2);
	
	virtual void update(const float deltaTime);
	void draw(SDL_Renderer* pRenderer) const; // const after the function says it will not change anything

	void cleanup();
};

