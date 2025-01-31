#include "pch.h"
#include "Blast.h"
#include "Texture.h"

Blast::Blast(const Point2f& pos): GameObject("Images/Blast.png",1,7,pos,7,14,1),
	m_IsTimeUp{false}
{
}

void Blast::UpdateFrames(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 1.f / m_NrFramesPerSec)
	{
		if (m_AnimFrame == m_NrOfFrames) m_IsTimeUp = true;
		++m_AnimFrame;
		m_AnimTime -= 1.f / m_NrFramesPerSec;
	}
}

void Blast::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_Shape.width;
	srcRect.height = m_Shape.height;
	srcRect.left = m_AnimFrame * srcRect.width;
	srcRect.bottom = srcRect.height;

	m_pTexture->Draw(m_Shape, srcRect);
}

bool Blast::IsTimeUp() const
{
	return m_IsTimeUp;
}