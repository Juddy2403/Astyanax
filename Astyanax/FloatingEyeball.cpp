#include "pch.h"
#include "FloatingEyeball.h"
#include "Projectile.h"
#include <iostream>

int FloatingEyeball::m_NrOfInstances{};

FloatingEyeball::FloatingEyeball(const Point2f& pos, bool isGoingRight) : Enemy("Images/Enemy2.png", pos, 2,
	Vector2f{ isGoingRight ? 400.f : -400.f,-100.f }, 200, 2, 2, 12.f,400),
	m_AccumFlyTime{},
	m_FlyTime{ 0.3f },
	m_pProjectiles{},
	m_AccumDropTime{},
	m_DropTime{3.f},
	m_AvgHeight{ m_SpawnPoint.y },
	m_MaxHeightDist{50.f}
{
	
	m_NrOfInstances++;
}

FloatingEyeball::~FloatingEyeball()
{
	Enemy::~Enemy();
	for (Projectile* ptr : m_pProjectiles)
		delete ptr;
	m_pProjectiles.clear();
	m_NrOfInstances--;
}

void FloatingEyeball::Update(float elapsedSec, Level& level)
{
	Enemy::Update(elapsedSec);
	if(m_IsAlive)
	{
		ManageFlyTime(elapsedSec);
		ManageProjectileSpawn(elapsedSec);
	}
	ManageProjectiles(elapsedSec, level);
}

void FloatingEyeball::ManageProjectiles(float elapsedSec, Level& level)
{
	for (int i{}; i < m_pProjectiles.size(); i++)
	{
		m_pProjectiles[i]->Update(elapsedSec, level);
		DiscardProjectiles(i);
	}
}

void FloatingEyeball::DiscardProjectiles(int idx)
{
	if (m_pProjectiles[idx]->IsDone())
	{
		delete m_pProjectiles[idx];
		m_pProjectiles.erase(m_pProjectiles.begin() + idx);
	}
}

void FloatingEyeball::ManageProjectileSpawn(float elapsedSec)
{
	m_AccumDropTime += elapsedSec;
	if (m_AccumDropTime >= m_DropTime)
	{
		m_AccumDropTime = 0.f;                                  
		m_pProjectiles.push_back(new Projectile{ Point2f{m_Shape.left,m_Shape.bottom},(m_Velocity.x > 0) });
	}
}

void FloatingEyeball::DoProjectileBlast(float elapsedSec)
{
	for (int i{}; i < m_pProjectiles.size(); i++)
	{
		m_pProjectiles[i]->DoBlast(elapsedSec);
		DiscardProjectiles(i);
	}
}

void FloatingEyeball::ManageFlyTime(float elapsedSec)
{
	m_AccumFlyTime += elapsedSec;
	if (m_AccumFlyTime >= m_FlyTime)
	{
		if (abs(m_Shape.bottom - m_AvgHeight) >= m_MaxHeightDist)
		{
			if(m_Shape.bottom > m_AvgHeight && m_Velocity.y > 0)
				m_Velocity.y *= -1;
			if (m_Shape.bottom < m_AvgHeight && m_Velocity.y < 0)
				m_Velocity.y *= -1;
		}
		else m_Velocity.y *= -1;
		m_AccumFlyTime -= m_FlyTime;
	}
}

bool FloatingEyeball::WeaponCollision(const Rectf& weaponShape, int powerPercent, const SoundManager& sound, int& score)
{
	for (Projectile* ptr : m_pProjectiles)
	{
		ptr->WeaponCollision(weaponShape, powerPercent, sound, score);
	}
	return Enemy::WeaponCollision(weaponShape, powerPercent, sound, score);
}
bool FloatingEyeball::IsProjectileHit(const Avatar& avatar)
{
	for (Projectile* ptr : m_pProjectiles)
	{
		if (utils::IsOverlapping(ptr->GetExplosionRect(), avatar.GetCollisionRect()) &&
			ptr->IsExploding() && !avatar.IsInvincible())
			return true;
	}
	return false;
}

void FloatingEyeball::Draw() const
{
	Enemy::Draw();
	for (Projectile* ptr : m_pProjectiles)
	{
		ptr->Draw();
	}
}


void FloatingEyeball::StayInWindow(const Rectf& windowSize)
{
	if ((m_Velocity.x > 0 && m_Shape.left + m_Shape.width + 50.f >= windowSize.left + windowSize.width) ||
		(m_Velocity.x < 0 && m_Shape.left - 50.f <= windowSize.left))
		m_Velocity.x *= -1;
}