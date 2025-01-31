#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include "utils.h"
#include "Blast.h"
#include "Avatar.h"
#include "Level.h"
#include "SoundManager.h"
#include "PickUp.h"
#include <iostream>


Enemy::Enemy(const std::string& path, const Point2f& spawnPoint, int nrOfCols,
	const Vector2f& velocity, int health, int nrOfFrames, int nrFramesPerSec, float respawnTime, int scorePerEnemy) :
	GameObject(path, 1, nrOfCols, spawnPoint, nrOfFrames, nrFramesPerSec, health),
	m_Velocity{ velocity },
	m_SpawnPoint{ spawnPoint },
	m_RespawnTime{ respawnTime },
	m_AccumRespawnTime{},
	m_MaxHealth{ health },
	m_pBlast{ nullptr },
	m_HorSpeed{velocity.x},
	m_VerSpeed{velocity.y},
	m_IsVelocityReset{true},
	m_ScorePerEnemy{scorePerEnemy},
	m_pPickUp{nullptr}
{

}

Enemy::~Enemy()
{
	//GameObject::~GameObject();
	delete m_pBlast;
	m_pBlast = nullptr;
	delete m_pPickUp;
	m_pPickUp = nullptr;
}

void Enemy::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_Shape.width;
	srcRect.height = m_Shape.height;
	srcRect.left = m_AnimFrame * srcRect.width;
	srcRect.bottom = srcRect.height;

	if (m_IsAlive && m_IsShowing) m_pTexture->Draw(m_Shape, srcRect);
	if (m_pBlast != nullptr) m_pBlast->Draw();
	if (m_pPickUp != nullptr) m_pPickUp->Draw();

}

void Enemy::Update(float elapsedSec)
{
	if (m_IsAlive)
	{
		GameObject::Update(elapsedSec);
		MoveEnemy(elapsedSec);
		if (!m_IsInvincible && !m_IsVelocityReset)
		{
			m_Velocity = Vector2f{ m_HorSpeed,m_VerSpeed };
			m_IsVelocityReset = true;
		}
	}
	else ManageRespawnTime(elapsedSec);
}

void Enemy::MoveEnemy(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Enemy::DoBlast(float elapsedSec)
{
	if (m_pBlast != nullptr)
	{
		m_pBlast->Update(elapsedSec);
		if (m_pBlast->IsTimeUp())
		{
			delete m_pBlast;
			m_pBlast = nullptr;
		}
	}
}

void Enemy::Respawn()
{
	m_AccumRespawnTime = m_RespawnTime;
}

Point2f Enemy::GetRespawnPoint() const
{
	return m_SpawnPoint;
}

void Enemy::ManageRespawnTime(float elapsedSec)
{
	m_AccumRespawnTime += elapsedSec;
	if (m_AccumRespawnTime >= m_RespawnTime)
	{
		m_AccumRespawnTime = 0.f;
		m_IsAlive = true;
		ResetPosition();
		m_IsShowing = true;
		m_IsInvincible = false;
		m_Health = m_MaxHealth;
	}
}

void Enemy::ResetPosition()
{
	m_Shape.bottom = m_SpawnPoint.y;
	m_Shape.left = m_SpawnPoint.x;
}

bool Enemy::WeaponCollision(const Rectf& weaponShape, int powerPercent, const SoundManager& sound, int& score)
{ 
	if (utils::IsOverlapping(m_Shape, weaponShape) && m_IsAlive && !m_IsInvincible)
	{
		m_IsInvincible = true;
		m_Health-= powerPercent;
		if (m_Health <= 0)
		{
			score += m_ScorePerEnemy;
			sound.PlayEnemyDeathSound();
			m_IsAlive = false;
			delete m_pBlast;
			m_pBlast = new Blast{ Point2f {m_Shape.left, m_Shape.bottom} };
			GeneratePickUp();
			ResetPosition();
		}
		if (m_pBlast == nullptr)
			m_pBlast = new Blast{ Point2f {m_Shape.left, m_Shape.bottom} };
		//Knockback();
		return true;
	}
	return false;
}

void Enemy::GeneratePickUp()
{
	int pickUpChance{ rand() % 10 };
	if (pickUpChance == 0)
	{
		if (m_pPickUp == nullptr)
		{
			m_pPickUp = new PickUp{ Point2f{ m_Shape.left,m_Shape.bottom + 1 } ,
				PickUp::Type(rand() % 4 + 1) };
		}
	}
}

void Enemy::ManagePickUp(float elapsedSec, const Level& level, Avatar& avatar)
{
	if(m_pPickUp != nullptr)
	{
		m_pPickUp->Update(elapsedSec, level);
		if (m_pPickUp->IsCollidingAvatar(avatar.GetCollisionRect()) && m_pPickUp->IsOnGround())
		{
			avatar.ReceivePickup(m_pPickUp->GetType());
			delete m_pPickUp;
			m_pPickUp = nullptr;
		}
	}
}

void Enemy::Knockback(bool isGoingLeft)
{
	//m_Velocity = Vector2f{ (-m_Velocity.x / m_Velocity.x) * 300.f,-300.f }; //wtf?

	Vector2f knockbackVelocity{ 200.f,300.f };
	if (m_Velocity.x > 0 || !isGoingLeft) knockbackVelocity.x *= -1;
	if (m_Velocity.y > 0) knockbackVelocity.y *= -1;
	m_Velocity = knockbackVelocity;
	m_Shape.bottom += 5.f; //so that m_IsOnGround is no longer true and the enemy gets pushed horizontally too
	m_IsVelocityReset = false;
}

Vector2f Enemy::GetVelocity() const
{
	return m_Velocity;
}

void Enemy::OutsideLevelCollision(const Rectf& window)
{
	if (!utils::IsOverlapping(m_Shape, window)) m_IsAlive = false;
}


void Enemy::ResetSpawnPoint(const Point2f& pos)
{
	m_SpawnPoint = pos;
}