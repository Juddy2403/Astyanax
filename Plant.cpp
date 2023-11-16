#include "pch.h"
#include "Plant.h"
#include "utils.h"
#include "GlobalFunctions.h"
#include <iostream>

int Plant::m_NrOfInstances{};

Plant::Plant(const Point2f& pos) :
	Enemy("Images/Enemy3.png", pos, 3, Vector2f{ 0.f,0.f }, 300, 3, 2, 10.f, 600)
	, m_pFlames{}
{
	m_NrOfInstances++;
}

Plant::~Plant()
{
	Enemy::~Enemy();
	for (Flame* ptr : m_pFlames) delete ptr;
	m_pFlames.clear();
	m_NrFramesPerSec--;
}

void Plant::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	if (m_AnimFrame == m_NrOfFrames-1 && m_pFlames.size() == 0) SpawnFlames();

	for (Flame* ptr : m_pFlames)
		ptr->MoveFlame(elapsedSec);
}

void Plant::SpawnFlames()
{
	Point2f flameSpawnPos{ m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height*2/3.f };
	m_pFlames.push_back(new Flame{ flameSpawnPos,false });
	m_pFlames.push_back(new Flame{ flameSpawnPos,true });
}

void Plant::Draw() const
{
	Enemy::Draw();
	for (Flame* ptr : m_pFlames) ptr->Draw();
}

void Plant::OutsideLevelFlameCollision(const Rectf& window)
{
	if(m_pFlames.size() != 0)
	{
		for (size_t i = 0; i < m_pFlames.size(); i++)
			if (m_pFlames[i]->OutsideLevelCollision(window))
			{
				delete m_pFlames[i];
				m_pFlames.erase(m_pFlames.begin() + i);
			}
	}
}

bool Plant::HasFlameHitAvatar(const Rectf& avatarShape)
{
	for (size_t i = 0; i < m_pFlames.size(); i++)
	{
		if (utils::IsOverlapping(m_pFlames[i]->GetShape(), avatarShape))
		{
			delete m_pFlames[i];
			m_pFlames.erase(m_pFlames.begin()+i);
			return true;
		}
	}
	return false;
}
