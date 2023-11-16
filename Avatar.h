#pragma once
#include <Vector2f.h>
#include "GameObject.h"
#include "SoundManager.h"
#include "PowerUpManager.h"
#include "PowerUpTypeEnum.h"
#include "WeaponTypeEnum.h"
#include "PickUp.h"

class Level;
class PowerUpManager;
class Avatar final : public GameObject
{
private:
	enum class ActionState {
		waiting,
		moving,
		ducking,
		parrying
	};

	PowerUpType m_PowerUp;
	PowerUpManager m_PowerUpManager;
	float m_HorSpeed;
	float m_JumpSpeed;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	ActionState m_ActionState;
	WeaponType m_Weapon;
	bool m_IsAttacking;
	bool m_IsOnGround;
	bool m_IsGoingLeft;
	bool m_CanMove;
	const int m_MaxHp;
	int m_SPPoints;
	int m_PowerPercent;
	int m_HitPowerPercent;
	int m_AttackFrameCount;
	//Dieing animation
	bool m_IsDieing;
	const float m_DieingTime;
	float m_AccumDieingTime;

	//SP regen
	const float m_SPRegenTime;
	const int m_MaxSPPoints;
	float m_AccumSPRegenTime;
	void ManageSPRegenTime(float elapsedSec);

	//PowerUp stance
	bool m_IsInPowerUpStance;
	const float m_PowerUpStanceTime;
	float m_AccumPowerUpStanceTime;
	void ManagePowerUpStanceTime(float elapsedSec);

	//Power gauge time management
	const float m_PowerReloadTime;
	float m_AccumPowerReloadTime;
	int m_MaxPower;
	void ManagePowerGauge(float elapsedSec);
	void SetSpriteColumn(Rectf& srcRect) const;
	void CheckStateTransition();
	void MoveAvatar(float elapsedSec);
	void SetSpriteRow(float& bottom) const;
	void FlipAvatar() const;
	void SetFramesState();
	void ResetShape(const Rectf& collisionRect);
	void SetNrOfFrames(int nrOfFrames, int nrFramesPerSec);
	void ManageDieingTime(float elapsedSec);
	void SetUnableToMoveState();
	virtual void UpdateFrames(float elapsedSec) override;

public:
	explicit Avatar();
	void Update(float elapsedSec, Level& level);
	void OutsideLevelCollision(const Rectf& window);
	virtual void Draw() const override;

	void Hit(const SoundManager& sound);
	void ReceivePickup(const PickUp::Type& type);

	void AttackButtonHit();
	void PowerUpButtonHit();
	void PerformPowerUp(const SoundManager& sound);

	bool IsGoingLeft() const;
	bool IsMovingHoriz() const;
	bool IsBlastHappening() const;
	bool IsBindHappening() const;
	bool IsBoltHappening() const;

	Rectf GetWeaponShape() const;
	Rectf GetCollisionRect() const;
	int GetPowerUpAttackPower() const;
	int GetHealth() const;
	int GetPowerPercent() const;
	int GetHitPower() const;
	int GetSPPoints() const;
	int GetPowerUp() const;
	


};

