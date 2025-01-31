#pragma once
#include "WeaponTypeEnum.h"
#include "PowerUpTypeEnum.h"

class Avatar;
class Texture;
class PowerUpManager final
{
private:
	const Texture* m_pBlastTexture;
	float m_BlastCenterDist;
	const int m_NrOfFireballs;
	const float m_AngleBetweenFireballs;
	const float m_FireballSpeed;
	Point2f m_FireballStartPos;

	void ManageBlastTime(float elapsedSec);
	bool m_IsBlastHappening;
	const float m_BlastTime;
	float m_AccumBlastTime;

	void ManageBindTime(float elapsedSec);
	bool m_IsBindHappening;
	const float m_BindTime;
	float m_AccumBindTime;

	void ManageBoltTime(float elapsedSec);
	bool m_IsBoltHappening;
	const float m_BoltTime;
	float m_AccumBoltTime;
public:
	explicit PowerUpManager();
	~PowerUpManager();
	void Draw() const;
	void Update(float elapsedSec);

	bool IsBlastHappening() const;
	bool IsBindHappening() const;
	bool IsBoltHappening() const;
	bool IsAnyPowerUpActive() const;

	int GetSPCost(const WeaponType& weaponType, const PowerUpType& powerUpType) const;
	int GetAttackPower(const WeaponType& weaponType, const PowerUpType& powerUpType) const;
	void BeginBlast(const Point2f& avatarCenter);
	void BeginBolt();
	void BeginBind();

};

