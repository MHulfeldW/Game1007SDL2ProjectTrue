#include "Animation.h"

void Animation::animate(int x, int w)
{
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}
		x = m_iSprite * w;
	}
	m_iFrame++;
}
