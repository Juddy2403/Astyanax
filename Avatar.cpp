#include "pch.h"
#include "Avatar.h"
#include "utils.h"
#include "Texture.h"
#include "Level.h"
#include "SoundManager.h"

#include <iostream>

Avatar::Avatar()
	: GameObject("Images/AvatarAnimations.png", 5, 9, Point2f{ 150.f,364.f }, 2, 2, 20) //150
	, m_HorSpeed{ 200.f }
	, m_JumpSpeed{ 700.f }
	, m_Velocity{ Vector2f{0.f,0.f} }
	, m_Acceleration{ Vector2f{-200.f,-1400.f} }
	, m_ActionState{ ActionState::moving }
	, m_Weapon{ WeaponType::axe }
	, m_IsAttacking{}
	, m_IsOnGround{ false }
	, m_IsGoingLeft{ false }
	, m_IsDieing{ false }
	, m_AccumDieingTime{}
	, m_DieingTime{ 1.7f }
	, m_CanMove{ true }
	, m_SPPoints{ 20 }
	, m_PowerPercent{ 0 }
	, m_PowerUp{}
	, m_AttackFrameCount{}
	, m_PowerReloadTime{ 0.1f }
	, m_AccumPowerReloadTime{}
	, m_HitPowerPercent{}
	, m_PowerUpManager{}
	, m_IsInPowerUpStance{ false }
	, m_PowerUpStanceTime{ 0.6f }
	, m_AccumPowerUpStanceTime{}
	, m_MaxHp{m_Health}
	, m_MaxPower{100}
	, m_AccumSPRegenTime{}
	, m_SPRegenTime{5.f}
	, m_MaxSPPoints{ 20 }
{}
#pragma region Update
void Avatar::Update(float elapsedSec, Level& level)
{
	m_PowerUpManager.Update(elapsedSec);
	ManageSPRegenTime(elapsedSec);
	if (m_IsInPowerUpStance) ManagePowerUpStanceTime(elapsedSec);

	if (m_ActionState != ActionState::parrying && m_CanMove) CheckStateTransition();
	else if (m_IsOnGround) m_ActionState = ActionState::moving;

	if (m_IsDieing)
	{
		ManageDieingTime(elapsedSec);
		SetUnableToMoveState();
	}
	else m_IsShowing = true;

	//Power gauge regen
	if (m_PowerPercent < m_MaxPower)
		ManagePowerGauge(elapsedSec);

	if (m_ActionState == ActionState::moving || m_ActionState == ActionState::parrying)
		MoveAvatar(elapsedSec);

	SetFramesState();
	GameObject::Update(elapsedSec);

	//The shape that collides with the level
	Rectf collisionRect{ GetCollisionRect() };
	m_IsOnGround = level.IsOnGround(collisionRect);

	//HandleCollision only resets x and y pos
	level.HandleCollision(collisionRect, m_Velocity, m_IsGoingLeft);
	ResetShape(collisionRect);

	if (m_IsOnGround && !m_IsDieing && !m_IsInPowerUpStance) m_CanMove = true;
}

void Avatar::CheckStateTransition()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_ActionState = ActionState::moving;
		m_Velocity.x = -m_HorSpeed;
		m_IsGoingLeft = true;
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_ActionState = ActionState::moving;
		m_Velocity.x = m_HorSpeed;
		m_IsGoingLeft = false;
	}
	if (m_IsOnGround)
	{
		if (pStates[SDL_SCANCODE_UP])
		{
			m_AnimFrame = 0;
			m_AnimTime = 0;
			m_ActionState = ActionState::moving;
			m_Velocity.y = m_JumpSpeed;
		}
		if (!pStates[SDL_SCANCODE_LEFT] && !pStates[SDL_SCANCODE_RIGHT] && !pStates[SDL_SCANCODE_UP] && !pStates[SDL_SCANCODE_DOWN])
		{
			m_ActionState = ActionState::waiting;
			m_Velocity.x = 0.f;
		}
		if (pStates[SDL_SCANCODE_DOWN])
		{
			if (m_ActionState != ActionState::ducking)
			{
				m_AnimFrame = 0;
				m_AnimTime = 0;
			}
			m_Velocity.x = 0;
			m_ActionState = ActionState::ducking;
		}
	}

}

void Avatar::AttackButtonHit()
{
	if (!m_IsAttacking)
	{
		m_IsAttacking = true;
		m_HitPowerPercent = m_PowerPercent;
		m_AnimTime = 0;
		m_AnimFrame = 0;
		m_PowerPercent = 0;
	}
}

void Avatar::PowerUpButtonHit()
{
	m_PowerUp = PowerUpType((int(m_PowerUp) + 1) % 3);
}

void Avatar::PerformPowerUp(const SoundManager& sound)
{
	if (!m_PowerUpManager.IsAnyPowerUpActive())
	{
		int SPCost{ m_PowerUpManager.GetSPCost(m_Weapon,m_PowerUp) };
		if (m_SPPoints - SPCost >= 0)
		{
			m_IsInPowerUpStance = true;
			SetUnableToMoveState();
			m_SPPoints -= SPCost;
			switch (m_PowerUp)
			{
			case PowerUpType::blast:
				m_PowerUpManager.BeginBlast(Point2f{ m_Shape.left + m_Shape.width / 2.f,
						m_Shape.bottom + m_Shape.height / 2.f });
				sound.PlayBlastAttackSound();
				break;
			case PowerUpType::bolt:
				m_PowerUpManager.BeginBolt();
				sound.PlayBoltAttackSound();
				break;
			case PowerUpType::bind:
				m_PowerUpManager.BeginBind();
				sound.PlayBindAttackSound();
				break;
			}
		}
	}
}

void Avatar::SetUnableToMoveState()
{
	m_CanMove = false;
	if (m_IsOnGround) m_Velocity.x = 0.f;
}

bool Avatar::IsBlastHappening() const
{
	return m_PowerUpManager.IsBlastHappening();
}

bool Avatar::IsBindHappening() const
{
	return m_PowerUpManager.IsBindHappening();
}

bool Avatar::IsBoltHappening() const
{
	return m_PowerUpManager.IsBoltHappening();
}

void Avatar::ManageDieingTime(float elapsedSec)
{
	m_AccumDieingTime += elapsedSec;
	if (m_AccumDieingTime >= m_DieingTime)
	{
		m_AccumDieingTime = 0.f;
		m_IsAlive = false;
	}
}

void Avatar::MoveAvatar(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	//gravitational force
	if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;

	//air resistance when jumping
	if (!m_IsOnGround)
	{
		if (m_Velocity.x >= -elapsedSec * m_Acceleration.x)
		{
			m_Velocity.x += elapsedSec * m_Acceleration.x;
			if (m_Velocity.x < 0.f) m_Velocity.x = 0.f;
		}
		if (m_Velocity.x <= elapsedSec * m_Acceleration.x)
		{
			m_Velocity.x -= elapsedSec * m_Acceleration.x;
			if (m_Velocity.x > 0.f) m_Velocity.x = 0.f;
		}
	}
}

void Avatar::SetFramesState()
{
	if (m_ActionState == ActionState::parrying) SetNrOfFrames(1, 1);
	else if (m_IsDieing) SetNrOfFrames(2, 1);
	else
	{
		if (!m_IsAttacking)
		{
			if (m_ActionState == ActionState::waiting)
			{
				if (!m_IsOnGround) SetNrOfFrames(1, 1);
				else SetNrOfFrames(2, 2);
			}
			else if (m_ActionState == ActionState::moving)
			{
				if (!m_IsOnGround) SetNrOfFrames(1, 1);
				else SetNrOfFrames(3, 6);
			}
			//if its ducking
			else SetNrOfFrames(1, 1);
		}
		else SetNrOfFrames(3, 9);
	}

}

void Avatar::SetNrOfFrames(int nrOfFrames, int nrFramesPerSec)
{
	m_NrOfFrames = nrOfFrames;
	m_NrFramesPerSec = nrFramesPerSec;
}

void Avatar::UpdateFrames(float elapsedSec)
{

	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 1.f / m_NrFramesPerSec)
	{

		//Full animation for attacking
		if (m_IsAttacking)
		{
			m_AttackFrameCount++;
			if (m_AttackFrameCount >= m_NrOfFrames)
			{
				m_IsAttacking = false;
				m_AttackFrameCount = 0;
			}

		}
		if (m_NrOfFrames != 1)
			++m_AnimFrame %= m_NrOfFrames;
		m_AnimTime = 0;
	}
}
#pragma endregion

#pragma region Collision
void Avatar::OutsideLevelCollision(const Rectf& window)
{
	Rectf collisionRect{ GetCollisionRect() };
	if (collisionRect.left < window.left && m_Velocity.x < 0.f)
	{
		collisionRect.left = window.left;
		m_Velocity.x = 0.f;
	}
	if (collisionRect.left + collisionRect.width > window.left + window.width && m_Velocity.x > 0.f)
	{
		collisionRect.left = window.left + window.width - collisionRect.width;
		m_Velocity.x = 0.f;
	}
	if (collisionRect.bottom + collisionRect.height < window.bottom)
	{
		m_IsAlive = false;
	}
	ResetShape(collisionRect);
}

Rectf Avatar::GetCollisionRect() const
{ //TODO: maybe make a variable out of this shit
	Rectf avatarShape{ m_Shape }; 
	avatarShape.left += 100.f;
	avatarShape.width = 60.f;
	avatarShape.height -= 60.f;

	if (m_ActionState == ActionState::ducking)
		avatarShape.height /= 2.f;
	return avatarShape;
}

void Avatar::ResetShape(const Rectf& collisionRect)
{
	m_Shape.left = collisionRect.left - 100.f;
	m_Shape.bottom = collisionRect.bottom;
}

Rectf Avatar::GetWeaponShape() const
{
	if (m_IsAttacking) //TODO: maybe make a variable out of this shit
	{
		Rectf weaponRect{ m_Shape };
		weaponRect.left += 120.f;  	//+150
		weaponRect.width -= 120.f;	//-150
		weaponRect.bottom += 50.f;	//+50
		weaponRect.height -= 60.f;	//-80
		if (m_IsGoingLeft)
			weaponRect.left -= 120.f; //-130
		if (m_ActionState == ActionState::ducking)
		{
			weaponRect.left -= 10.f;   //-20
			weaponRect.height -= 30.f; //-30
			weaponRect.bottom -= 50.f; //-50
			weaponRect.width += 20.f;  //+20
		}
		return weaponRect;
	}
	else return Rectf{};
}

void Avatar::Hit(const SoundManager& sound)
{
	if (!m_IsDieing)
	{
		m_ActionState = ActionState::parrying;
		m_IsInvincible = true;
		m_Health--;
		sound.PlayAvatarHitSound();
		if (m_IsOnGround)
		{
			if (m_IsGoingLeft) m_Velocity.x = m_HorSpeed * 2.f;
			else m_Velocity.x = -m_HorSpeed * 2.f;
			m_Velocity.y = m_JumpSpeed / 2.f;
			m_Shape.bottom += 3.f;
		}
		else
		{
			if (m_IsGoingLeft) m_Velocity.x = m_HorSpeed / 8.f;
			else m_Velocity.x = -m_HorSpeed / 8.f;
			m_Velocity.y = -m_JumpSpeed / 4.f;
		}
		m_CanMove = false;

		if (m_Health == 0)
		{
			m_IsDieing = true;
			m_AnimFrame = 0;
			m_AnimTime = 0;
			sound.PlayAvatarDeathSound();
		}
	}
}
#pragma endregion

#pragma region Draw
void Avatar::Draw() const
{
	m_PowerUpManager.Draw();

	Rectf srcRect{};
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	SetSpriteColumn(srcRect);
	SetSpriteRow(srcRect.bottom);

	FlipAvatar();

	if (m_IsShowing || m_IsDieing)
		m_pTexture->Draw(Point2f{ 0.f,0.f }, srcRect);
}

void Avatar::SetSpriteColumn(Rectf& srcRect) const
{
	if (m_IsDieing) srcRect.left = (m_AnimFrame + 6) * srcRect.width;
	else if (m_IsInPowerUpStance)
		srcRect.left = 8 * srcRect.width;
	else
	{
		if (m_ActionState == ActionState::parrying)
			srcRect.left = (m_AnimFrame + int(m_Weapon) * 2) * srcRect.width;
		else
		{
			srcRect.left = (m_AnimFrame + int(m_Weapon) * 3) * srcRect.width;
		}
	}
}

void Avatar::FlipAvatar() const
{
	if (m_IsGoingLeft)
	{
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width, m_Shape.bottom, 0);
	}
	else glTranslatef(m_Shape.left, m_Shape.bottom, 0);
}

void Avatar::SetSpriteRow(float& bottom) const
{
	if (m_ActionState == ActionState::parrying || m_IsDieing || m_IsInPowerUpStance)
		bottom = m_ClipHeight * 5;
	else if (m_ActionState == ActionState::ducking) bottom = m_ClipHeight * 4;
	else {
		if (!m_IsOnGround)
		{
			bottom = m_ClipHeight * 3;
		}
		else if (m_ActionState == ActionState::waiting || m_ActionState == ActionState::moving)
		{
			if (m_IsAttacking)
			{
				bottom = m_ClipHeight * 2;
			}
			else
			{
				bottom = m_ClipHeight;
			}
		}
	}

}
#pragma endregion

void Avatar::ReceivePickup(const PickUp::Type& type)
{
	switch (type)
	{
	case PickUp::Type::bluePotion:
		m_Health = m_MaxHp;
		break;
	case PickUp::Type::redPotion:
		m_Health++;
		break;
	case PickUp::Type::wings:
		m_HorSpeed += 30.f;
		break;
	case PickUp::Type::power:
		m_MaxPower += 20;
		break;
	case PickUp::Type::weapon:
		m_Weapon = WeaponType(int(m_Weapon) + 1);
		break;
	}
}

bool Avatar::IsGoingLeft() const
{
	return m_IsGoingLeft;
}

bool Avatar::IsMovingHoriz() const
{
	return m_Velocity.x > 0.001f || m_Velocity.x < -0.001f;
}

int Avatar::GetPowerPercent() const
{
	return m_PowerPercent;
}

int Avatar::GetPowerUpAttackPower() const
{
	return m_PowerUpManager.GetAttackPower(m_Weapon,m_PowerUp);
}

int Avatar::GetHealth() const
{
	return m_Health;
}

int Avatar::GetSPPoints() const
{
	return m_SPPoints;
}

int Avatar::GetPowerUp() const
{
	return int(m_PowerUp);
}

void Avatar::ManageSPRegenTime(float elapsedSec)
{
	m_AccumSPRegenTime += elapsedSec;
	if (m_AccumSPRegenTime >= m_SPRegenTime)
	{
		if(m_SPPoints < m_MaxSPPoints)
		m_SPPoints++;
		m_AccumSPRegenTime = 0.f;
	}
}

void Avatar::ManagePowerUpStanceTime(float elapsedSec)
{
	m_AccumPowerUpStanceTime += elapsedSec;
	if (m_AccumPowerUpStanceTime >= m_PowerUpStanceTime)
	{
		m_IsInPowerUpStance = false;
		m_CanMove = true;
		m_AccumPowerUpStanceTime = 0.f;
	}
}

void Avatar::ManagePowerGauge(float elapsedSec)
{
	m_AccumPowerReloadTime += elapsedSec;
	if (m_AccumPowerReloadTime >= m_PowerReloadTime)
	{
		m_PowerPercent += 10;
		m_AccumPowerReloadTime -= m_PowerReloadTime;
	}
}

int Avatar::GetHitPower() const
{
	int hitPower{ m_HitPowerPercent };

	if (m_Weapon == WeaponType::spear) hitPower /= 2;
	else if (m_Weapon == WeaponType::sword) hitPower = int(hitPower * 1.5f);

	return hitPower;
}