#include "pch.h"
#include "Projectile.h"
#include "Texture.h"

Projectile::Projectile(const Point2f& pos, bool isGoingRight)
	: Enemy("Images/Projectile1.png",pos,1,Vector2f{ 50.f,-200.f },1,1,1,-1,0)
	, m_JumpSpeed{600.f}
	, m_IsOnGround{ false }
	, m_VerAcceleration {-981.f}
	, m_MaxNrOfJumps{4} //4
	, m_NrOfJumps{}
	, m_IsExploding{false}
	, m_IsDone{ false }
{
	if (!isGoingRight) m_Velocity.x *= -1;
}

bool Projectile::WeaponCollision(const Rectf& weaponShape, int powerPercent, const SoundManager& sound, int& score)
{
	if (utils::IsOverlapping(m_Shape, weaponShape) && m_IsAlive && !m_IsInvincible)
	{
		m_IsInvincible = true;
		m_Health -= powerPercent;
		if (m_Health <= 0)
		{
			score += m_ScorePerEnemy;
			sound.PlayEnemyDeathSound();
			m_IsAlive = false;
			delete m_pBlast;
			m_pBlast = new Blast{ Point2f {m_Shape.left, m_Shape.bottom} };
			ResetPosition();
		}
		if (m_pBlast == nullptr)
			m_pBlast = new Blast{ Point2f {m_Shape.left, m_Shape.bottom} };
		//Knockback();
		return true;
	}
	return false;
}

void Projectile::Update(float elapsedSec,const Level& level)
{
	if (m_IsAlive)
	{
		m_IsOnGround = level.IsOnGround(m_Shape);
		if (m_IsOnGround) Jump();
		MoveProjectile(elapsedSec);
	}
}

void Projectile::Jump()
{
	m_Velocity.y = m_JumpSpeed;
	m_NrOfJumps++;
	if (m_NrOfJumps == m_MaxNrOfJumps)
	{
		m_IsAlive = false;
		m_pBlast = new Blast{ Point2f{ m_Shape.left, m_Shape.bottom } };
		m_IsExploding = true;
	}
}


void Projectile::MoveProjectile(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Velocity.y += m_VerAcceleration * elapsedSec;
}


bool Projectile::IsExploding() const
{
	return m_IsExploding;
}
Rectf Projectile::GetExplosionRect() const
{
	if (m_pBlast != nullptr)
	{
		Rectf rect{ m_pBlast->GetShape() };
		rect.left -= 20.f;
		rect.bottom -= 20.f;
		rect.height += 40.f;
		rect.width += 40.f;
		return rect;
	}
	return Rectf();
}

bool Projectile::IsDone() const
{
	return m_IsDone;
}
