#pragma once
#include "Enemy.h"
#include "Level.h"
#include "Blast.h"
class Projectile final :  public Enemy
{
private:
	float m_VerAcceleration;
	float m_JumpSpeed;
	int m_NrOfJumps;
	const int m_MaxNrOfJumps;
	bool m_IsOnGround;
	bool m_IsExploding;
	bool m_IsDone;

	void MoveProjectile(float elapsedSec);
	void Jump();
public:
	explicit Projectile(const Point2f& pos, bool isGoingRight);
	virtual bool WeaponCollision(const Rectf& weaponShape, int powerPercent, const SoundManager& sound, int& score) override;
	void Update(float elapsedSec,const Level& level);
	bool IsExploding() const;
	Rectf GetExplosionRect() const;
	bool IsDone() const;
};

