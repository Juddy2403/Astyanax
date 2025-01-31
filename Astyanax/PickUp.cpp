#include "pch.h"
#include "PickUp.h"
#include "Texture.h"
#include <iostream>


PickUp::PickUp(const Point2f& pos, const Type& type) :
	GameObject("Images/PickUps.png",1,5,pos,1,1,1),
	m_Type{ type },
	m_Velocity{ Vector2f{200.f,400.f} },
	m_Acceleration{ Vector2f{-100.f,-1200.f} },
	m_IsOnGround {false}
{
}

void PickUp::Draw() const
{
	if (m_IsAlive)
	{
		Rectf srcRect{};
		srcRect.width = m_Shape.width;
		srcRect.height = m_Shape.height;
		srcRect.left = int(m_Type) * srcRect.width;
		srcRect.bottom = srcRect.height;
		m_pTexture->Draw(m_Shape, srcRect);
	}
}

void PickUp::Update(float elapsedSec,const Level& level)
{
	if(!m_IsOnGround)
	MovePickup(elapsedSec);
	m_IsOnGround = level.IsOnGround(m_Shape);
	level.HandleCollision(m_Shape, m_Velocity, false);
}
bool PickUp::IsCollidingAvatar(const Rectf& avatarShape)
{
	if (utils::IsOverlapping(m_Shape, avatarShape) && m_IsAlive && m_IsOnGround)
	{
		m_IsAlive = false;
		return true;
	}
	return false;
}

PickUp::Type PickUp::GetType() const
{
	return Type{ m_Type };
}

bool PickUp::IsOnGround() const
{
	return m_IsOnGround;
}

void PickUp::MovePickup(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Velocity += m_Acceleration * elapsedSec;
}
