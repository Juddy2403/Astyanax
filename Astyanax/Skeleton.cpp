#include "pch.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Blast.h"
#include <iostream>
#include <utils.h>
//#include "GlobalFunctions.h"

int Skeleton::m_NrOfInstances{};

Skeleton::Skeleton(const Point2f& pos):
	Enemy("Images/Enemy4.png", pos, 6, Vector2f{ 200.f,0.f },300,3,3,10.f,1000)
	, m_ActionState{ActionState::spawning}
	, m_IsSpawnAnimationOver{false}
	, m_Acceleration{ Vector2f{0.f,-1000.f} }
	, m_JumpSpeed{600.f}
	, m_IsGoingLeft{true}
	, m_IsOnGround{true}
	, m_AccumMoveTime{}
	, m_MovedDist{}
	, m_MinMovedDist{5.f}
{
	m_NrOfInstances++;
	m_Velocity = Vector2f{ 0.f,0.f };
}

Skeleton::~Skeleton()
{
	Enemy::~Enemy();
	m_NrOfInstances--;
}

void Skeleton::Update(float elapsedSec)
{
	if (!m_IsVelocityReset && m_IsOnGround) m_Velocity.x = 0.f;
	if (m_Velocity.y >= m_Acceleration.y) m_Velocity.y += m_Acceleration.y * elapsedSec;
	//if(m_ActionState == ActionState::jumping) std::cout << "jumpy" << "\n";
	Enemy::Update(elapsedSec);
	SetNrOfFrames();
	DoFullSpawnAnimation();

	if (m_IsOnGround && m_ActionState == ActionState::jumping && m_Velocity.y <= 0)	
	{
		m_ActionState = ActionState::walking;
	}
}


void Skeleton::HandleLevelCollision(const Level& level)
{
	m_IsOnGround = level.IsOnGround(m_Shape);
	if(level.IsCollidingWithWalls(m_Shape) && m_ActionState == ActionState::walking)
	{
		m_ActionState = ActionState::jumping;
		m_Shape.bottom += 5.f;
		m_Velocity.y = m_JumpSpeed;
	}
	level.HandleCollision(m_Shape, m_Velocity, m_IsGoingLeft);
	//velocity.x is reset in handle collision so i have to reset it again here
	if(m_ActionState == ActionState::jumping)
		if (m_IsGoingLeft) m_Velocity.x = -m_HorSpeed; else m_Velocity.x = m_HorSpeed;
}

void Skeleton::DoFullSpawnAnimation()
{ //hopefully there's a better way to do this but for now it just works
	if (m_IsSpawnAnimationOver && m_AnimFrame == 0 && m_ActionState == ActionState::spawning)
		m_ActionState = ActionState::walking;
	if (m_ActionState == ActionState::spawning)
	{
		m_Velocity = Vector2f{ 0.f,0.f };
		if(m_AnimFrame == 2) m_IsSpawnAnimationOver = true;
	}
		
	if (!m_IsAlive) 
	{
		m_ActionState = ActionState::spawning;
		m_AnimFrame = 0;
		m_IsSpawnAnimationOver = false;
		m_Velocity = Vector2f{0,0};
		m_NrOfFrames = 3;
	}
} 

void Skeleton::SetNrOfFrames()
{
	switch (m_ActionState)
	{
	case ActionState::walking:
	case ActionState::attacking:
		m_NrOfFrames = 2;
		break;
	case ActionState::jumping:
		m_NrOfFrames = 1;
		break;
	}
}

void Skeleton::Draw() const
{
	Rectf srcRect{m_Shape};
	srcRect.bottom = srcRect.height;
	SetSpriteFrame(srcRect);
	DrawTextures(srcRect);
	//utils::DrawRect(GetWeaponRect());
	//utils::DrawRect(m_Shape);
}

void Skeleton::FlipTexture() const
{
	if (!m_IsGoingLeft)
	{
		glScalef(-1, 1, 1);
		glTranslatef(-m_Shape.left - m_Shape.width, m_Shape.bottom, 0);
	}
	else glTranslatef(m_Shape.left, m_Shape.bottom, 0);
}

void Skeleton::DrawTextures(const Rectf& srcRect) const
{
	glPushMatrix();
	FlipTexture();
	if (m_IsAlive && m_IsShowing) m_pTexture->Draw(Point2f{}, srcRect);
	glPopMatrix();
	if (m_pBlast != nullptr) m_pBlast->Draw();
}

void Skeleton::SetSpriteFrame(Rectf& srcRect) const
{
	switch (m_ActionState)
	{
	case ActionState::walking:
		srcRect.left = (m_AnimFrame + 2) * srcRect.width;
		break;
	case ActionState::attacking:
		srcRect.left = (m_AnimFrame + 4) * srcRect.width;
		break;
	case ActionState::jumping:
		srcRect.left = (m_AnimFrame + 4) * srcRect.width;
		break;
	default:
		srcRect.left = m_AnimFrame * srcRect.width;
	}
}

void Skeleton::FollowAvatar(const Rectf& avatarShape)
{
	const int closeUpDist{ 40 };
	if(m_IsVelocityReset && m_ActionState != ActionState::jumping)
	{
		if (m_Shape.left +closeUpDist > avatarShape.left + avatarShape.width )
		{
			m_IsGoingLeft = true;
			if (m_ActionState == ActionState::attacking) m_ActionState = ActionState::walking;
			if (m_ActionState == ActionState::walking) m_Velocity.x = -m_HorSpeed;
		}
		else if (m_Shape.left + m_Shape.width - closeUpDist  < avatarShape.left )
		{
			m_IsGoingLeft = false;
			if (m_ActionState == ActionState::attacking) m_ActionState = ActionState::walking;
			if (m_ActionState == ActionState::walking) m_Velocity.x = m_HorSpeed;
		}
		else
		{
			if (m_ActionState == ActionState::walking) m_ActionState = ActionState::attacking;
			m_Velocity.x = 0.f;
		}
	}
}

Rectf Skeleton::GetWeaponRect() const
{
	Rectf weaponRect{ m_Shape };
	weaponRect.height *= 2 / 3.f;
	weaponRect.bottom += m_Shape.height - weaponRect.height;
	weaponRect.width *= 1.5f;
	weaponRect.left = m_Shape.left + m_Shape.width / 2 - weaponRect.width / 2;

	return weaponRect;
}

bool Skeleton::IsAttacking() const
{
	return m_ActionState == ActionState::attacking;
}

bool Skeleton::IsGoingLeft() const
{
	return m_IsGoingLeft;
}


