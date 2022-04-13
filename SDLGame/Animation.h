#pragma once
#include "Sprite.h"

class Animation : public Sprite
{
private:
	int m_iFrame = 0,
		m_iFrameMax = 4,
		m_iSprite = 0,
		m_iSpriteMax = 8;
public:
	void animate(int x, int w);
};

