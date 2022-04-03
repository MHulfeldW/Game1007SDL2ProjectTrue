#pragma once
#include "Sprite.h"
// Declaring a new class Bullet, which inherits from Sprite
//You can say Bullet is a Child/Derived class of the Parent/Base class Sprite
//Bullet can use all of the data members and functions (that are public or protected) in Sprite
//Bullet can also override some of those functions to do its own version of update or its own draw etc.
class Bullet : public Sprite // A Bullet is a type of Sprite
{
public:
	Bullet(SDL_Renderer* renderer);
};

