#include "pch.h"
#include "Flame.h"
#include <utils.h>
#include "Texture.h"

void Flame::MoveFlame(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Velocity.x += m_Acceleration.x * elapsedSec;
	m_Velocity.y += m_Acceleration.y * elapsedSec;
}

Flame::Flame(const Point2f& pos, bool isGoingLeft):
	GameObject("Images/Projectile2.png",1,1,pos,1,1,1)
	, m_Velocity{300.f,400.f}
	, m_Acceleration {-100.f, -1000.f}
{
	if (isGoingLeft) 
	{
		m_Velocity.x *= -1;
		m_Acceleration.x *= -1;
	}
}

bool Flame::OutsideLevelCollision(const Rectf& window)
{
	return (!utils::IsOverlapping(m_Shape, window));
}

void Flame::Draw() const
{
	m_pTexture->Draw(m_Shape);
}
