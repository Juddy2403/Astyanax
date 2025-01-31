#include "pch.h"
#include "GameObject.h"
#include "Texture.h"
#include <iostream>

GameObject::~GameObject()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

GameObject::GameObject(const std::string& path, int nrOfRows, int nrOfCols, const Point2f& pos,
	int nrOfFrames, int nrFramesPerSec, int health) :
	m_pTexture{ new Texture{path} },
	m_AnimTime{},
	m_AnimFrame{},
	m_Health{health},
	m_NrOfCols{ nrOfCols },
	m_NrOfRows{ nrOfRows },
	m_IsAlive{ true },
	m_NrOfFrames{ nrOfFrames },
	m_NrFramesPerSec{ nrFramesPerSec },
	m_AccumInvincibleTime{},
	m_InvincibleTime{ 1.5f },
	m_IsInvincible{ false },
	m_IsShowing{ true },
	m_AccumShowTime{},
	m_ShowTime{ 0.05f }
{
	m_ClipHeight = m_pTexture->GetHeight() / m_NrOfRows;
	m_ClipWidth = m_pTexture->GetWidth() / m_NrOfCols;
	m_Shape = Rectf{ pos.x,pos.y,m_ClipWidth,m_ClipHeight };
}

void GameObject::UpdateFrames(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 1.f / m_NrFramesPerSec)
	{
		++m_AnimFrame %= m_NrOfFrames;
		m_AnimTime -= 1.f / m_NrFramesPerSec;
	}
}
void GameObject::Update(float elapsedSec)
{
	UpdateFrames(elapsedSec);
	if (m_IsInvincible)
	{
		ManageInvincibleTime(elapsedSec);
		ManageShowTime(elapsedSec);
	}
	else m_IsShowing = true;
}

Rectf GameObject::GetShape() const
{
	return m_Shape;
}
bool GameObject::IsAlive() const
{
	return m_IsAlive;
}
void GameObject::ManageShowTime(float elapsedSec)
{
	m_AccumShowTime += elapsedSec;
	if (m_AccumShowTime >= m_ShowTime)
	{
		m_AccumShowTime = 0.f;
		m_IsShowing = !m_IsShowing;
	}
}
void GameObject::ManageInvincibleTime(float elapsedSec)
{
	m_AccumInvincibleTime += elapsedSec;
	if (m_AccumInvincibleTime >= m_InvincibleTime)
	{
		m_AccumInvincibleTime = 0.f;
		m_IsInvincible = false;
		m_IsShowing = true;
	}
}
bool GameObject::IsInvincible() const
{
	return m_IsInvincible;
}