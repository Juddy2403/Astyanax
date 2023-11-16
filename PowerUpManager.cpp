#include "pch.h"
#include "PowerUpManager.h"
#include "Texture.h"
#include "Avatar.h"

PowerUpManager::PowerUpManager() :
	m_pBlastTexture{ new Texture{"Images/fireball.png"} }
	, m_BlastCenterDist{}
	, m_IsBlastHappening{ false }
	, m_NrOfFireballs{ 8 }
	, m_FireballStartPos{}
	, m_FireballSpeed{ 1100.f }
	, m_AngleBetweenFireballs{ 0.785f }
	, m_BlastTime{ 0.8f }
	, m_AccumBlastTime{}
	, m_IsBindHappening{ false }
	, m_BindTime{ 4.5f }
	, m_AccumBindTime{}
	, m_IsBoltHappening{ false }
	, m_BoltTime{ 1.f }
	, m_AccumBoltTime{}
{}
PowerUpManager::~PowerUpManager()
{
	delete m_pBlastTexture;
}

void PowerUpManager::Draw() const
{
	if (m_IsBlastHappening)
		for (size_t i = 0; i < m_NrOfFireballs; i++)
		{
			m_pBlastTexture->Draw(Point2f{
				m_FireballStartPos.x + m_BlastCenterDist * cosf(m_AngleBetweenFireballs * i),
				m_FireballStartPos.y + m_BlastCenterDist * sinf(m_AngleBetweenFireballs * i) });
		}
}

void PowerUpManager::Update(float elapsedSec)
{
	if (m_IsBlastHappening)
	{
		m_BlastCenterDist += elapsedSec * m_FireballSpeed;
		ManageBlastTime(elapsedSec);
	}
	if (m_IsBindHappening) ManageBindTime(elapsedSec);
	if (m_IsBoltHappening) ManageBoltTime(elapsedSec);
}

void PowerUpManager::ManageBlastTime(float elapsedSec)
{
	m_AccumBlastTime += elapsedSec;
	if (m_AccumBlastTime >= m_BlastTime)
	{
		m_IsBlastHappening = false;
		m_AccumBlastTime = 0.f;
		m_BlastCenterDist = 0.f;
	}
}

void PowerUpManager::ManageBindTime(float elapsedSec)
{
	m_AccumBindTime += elapsedSec;
	if (m_AccumBindTime >= m_BindTime)
	{
		m_IsBindHappening = false;
		m_AccumBindTime = 0.f;
	}
}

void PowerUpManager::ManageBoltTime(float elapsedSec)
{
	m_AccumBoltTime += elapsedSec;
	if (m_AccumBoltTime >= m_BoltTime)
	{
		m_IsBoltHappening = false;
		m_AccumBoltTime = 0.f;
	}
}

bool PowerUpManager::IsBlastHappening() const
{
	return m_IsBlastHappening;
}

bool PowerUpManager::IsBindHappening() const
{
	return m_IsBindHappening;
}

bool PowerUpManager::IsBoltHappening() const
{
	return m_IsBoltHappening;
}

bool PowerUpManager::IsAnyPowerUpActive() const
{
	return (m_IsBindHappening || m_IsBlastHappening || m_IsBoltHappening);
}

int PowerUpManager::GetSPCost(const WeaponType& weaponType, const PowerUpType& powerUpType) const
{
	switch (weaponType)
	{
	case WeaponType::axe:
		switch (powerUpType)
		{
		case PowerUpType::bind: return 5; break;
		case PowerUpType::blast: return 6; break;
		case PowerUpType::bolt: return 10; break;
		}
		break;
	case WeaponType::spear:
		switch (powerUpType)
		{
		case PowerUpType::bind: return 3; break;
		case PowerUpType::blast: return 4; break;
		case PowerUpType::bolt: return 5; break;
		}
		break;
	case WeaponType::sword:
		switch (powerUpType)
		{
		case PowerUpType::bind: return 6; break;
		case PowerUpType::blast: return 10; break;
		case PowerUpType::bolt: return 20; break;
		}
		break;
	}
	return 0;
}

int PowerUpManager::GetAttackPower(const WeaponType& weaponType, const PowerUpType& powerUpType) const
{
	switch (weaponType)
	{
	case WeaponType::axe:
	case WeaponType::spear:
		switch (powerUpType)
		{
		case PowerUpType::blast: return 400; break;
		case PowerUpType::bolt: return 800; break;
		}
		break;
	case WeaponType::sword:
		switch (powerUpType)
		{
		case PowerUpType::blast: return 800; break;
		case PowerUpType::bolt: return 1600; break;
		}
		break;
	}
	return 0;
}

void PowerUpManager::BeginBlast(const Point2f& avatarCenter)
{
	if (!m_IsBlastHappening)
	{
		m_IsBlastHappening = true;
		m_FireballStartPos = avatarCenter;
	}
}

void PowerUpManager::BeginBolt()
{
	m_IsBoltHappening = true;
}

void PowerUpManager::BeginBind()
{
	m_IsBindHappening = true;
}
