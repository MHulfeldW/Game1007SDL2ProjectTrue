#include "Sprite.h"
#include <minmax.h>

Sprite::Sprite()
{
	pTexture = nullptr;
	src.x = 0;
	src.y = 0;
	src.w = 0;
	src.h = 0;

	dst.x = 0;
	dst.y = 0;
	dst.w = 0;
	dst.h = 0;
}

Sprite::Sprite(SDL_Renderer* pRenderer, const char* filename) : Sprite()
{
	pTexture = IMG_LoadTexture(pRenderer, filename);
	SDL_QueryTexture(pTexture, NULL, NULL, &dst.w, &dst.h);
	src.w = dst.w;
	src.h = dst.h;
}

void Sprite::setPosition(const float x, const float y)
{
	dst.x = x;
	dst.y = y;
	position.x = x;
	position.y = y;
}

void Sprite::moveBy(const float xOffset, const float yOffset)
{
	dst.x += xOffset;
	dst.y += yOffset;
	position.x += xOffset;
	position.y += yOffset;
}

void Sprite::setSize(int width, int height)
{
	dst.w = width;
	dst.h = height;
}



void Sprite::update(const float deltaTime)
{
	//Displacement is a change in position
	//Velocity is a displacement over time 
	// calculate displacment with vector multiplication by a scalar (velocity * time)
	Vector2 displacement =
	{
		 velocity.x * deltaTime,
		 velocity.y * deltaTime
	};

	//Use velocity and time to update our sprite position using vector addition (position + displacement)
	position.x += displacement.x;
	position.y += displacement.y;

	dst.x = position.x;
	dst.y = position.y;
}

bool Sprite::isCollidingWith(Sprite* other)
{
	//1. Imagine circles around each sprite
	//1.1. Define radius of each based on size of sprite
	//1.2. Define position based on position of sprite
	float myRadius = max(getSize().x, getSize().y) * 0.5f;
	float otherRadius = max(other->getSize().x, other->getSize().y) * 0.5f;
	
	//2. Find the sum of both radii
	float sumRadii = myRadius + otherRadius;

	//3. Find the distance between the circles
	//3.1 subtract positions to get vector from one to the other
	Vector2 displacement = Vector2
	{
		(other->position.x + other->getSize().x*0.5f) - (position.x+ getSize().x * 0.5f),
		(other->position.y + other->getSize().y * 0.5f) - (position.y + getSize().y * 0.5f),
	};
	//3.2 use pythagorean theorem to find length of the displacment vector between sprites
	//sqrt(x^2+y^2)
	float distance = sqrt(pow(displacement.x, 2) + pow(displacement.y, 2));

	//4. If distance between them is less than the sum of radii, they are colliding 
	//if(d <= r1+r2) then true
	if (distance <= sumRadii)
	{
		return true;
	}
	{
		return false;
	}	
}

Vector2 Sprite::getSize()
{
	return Vector2{(float)dst.w, (float)dst.h};
}

void Sprite::draw(SDL_Renderer* pRenderer) const
{
	SDL_RenderCopy(pRenderer, pTexture, &src, &dst); // Draw texture
}

void Sprite::cleanup()
{
	SDL_DestroyTexture(pTexture);
}
void Sprite::animate(Sprite obj, Sprite obj2, SDL_Renderer* pRenderer)
{	
	obj.src.w = obj.src.h;
	obj.dst.w = obj2.src.w;
	obj.dst.h = obj2.src.h;
	obj.dst.x = obj2.dst.x;
	obj.dst.y = obj2.dst.y;
	
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}		
	}
	obj.src.x = m_iSprite * obj.src.w;
	m_iFrame++;
	obj.draw(pRenderer);
}

