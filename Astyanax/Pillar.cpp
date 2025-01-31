#include "pch.h"
#include "Pillar.h"
#include "PickUp.h"
#include "Texture.h"
#include <utils.h>
#include <iostream>


Pillar::Pillar(const Point2f& pos) :
	GameObject("Images/Pillar.png",1,1,pos,1,1,200),
	m_pPickUp{}
{
}

Pillar::~Pillar()
{
	GameObject::~GameObject();
	delete m_pPickUp;
	m_pPickUp = nullptr;
}

void Pillar::Draw() const
{
	if (m_IsAlive)
	{
		if(m_IsShowing)
			m_pTexture->Draw(m_Shape);
	}
	else m_pPickUp->Draw();
}

void Pillar::Update(float elapsedSec, Avatar& avatar,const Level& level)
{
	if(m_IsAlive)
	{
		ManageTakingHit(elapsedSec, avatar);
		ManageDieing();
		GameObject::Update(elapsedSec);
	} 
	else ManagePickUp(elapsedSec, level, avatar);
}

void Pillar::ManagePickUp(float elapsedSec,const Level& level,Avatar& avatar)
{
	m_pPickUp->Update(elapsedSec, level);
	if (m_pPickUp->IsCollidingAvatar(avatar.GetCollisionRect()) && m_pPickUp->IsOnGround()
		&& m_pPickUp->GetType() == PickUp::Type::weapon)
		avatar.ReceivePickup(m_pPickUp->GetType());
}

void Pillar::ManageDieing()
{
	if (m_Health <= 0)
	{
		m_IsAlive = false;
		m_pPickUp = new PickUp{ Point2f{ m_Shape.left,m_Shape.bottom+1 },PickUp::Type::weapon };
	}
}

void Pillar::ManageTakingHit(float elapsedSec,const Avatar& avatar)
{
	if (m_IsInvincible)
		ManageInvincibleTime(elapsedSec);
	else if (utils::IsOverlapping(m_Shape, avatar.GetWeaponShape()))
	{
		m_Health-= avatar.GetHitPower();
		m_IsInvincible = true;
	}
}
