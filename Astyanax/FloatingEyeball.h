#pragma once
#include "Enemy.h"
#include <vector>
#include "Level.h"
#include "Avatar.h"

class Projectile;
class FloatingEyeball final : public Enemy
{
private:
	float m_AccumFlyTime;
	const float m_FlyTime;
	void ManageFlyTime(float elapsedSec);
	std::vector<Projectile*> m_pProjectiles;
	//Projectile drop time
	float m_AccumDropTime;
	const float m_DropTime;
	const float m_AvgHeight;
	const float m_MaxHeightDist;
public:
	static int m_NrOfInstances;

	explicit FloatingEyeball(const Point2f& pos, bool isGoingRight);
	virtual ~FloatingEyeball() override;
	void Update(float elapsedSec, Level& level);
	void ManageProjectiles(float elapsedSec, Level& level);
	void DiscardProjectiles(int idx);
	virtual bool WeaponCollision(const Rectf& weaponShape, int powerPercent,const SoundManager& sound ,int& score) override;
	virtual void Draw() const override;
	bool IsProjectileHit(const Avatar& avatar);
	void ManageProjectileSpawn(float elapsedSec);
	void DoProjectileBlast(float elapsedSec);
	void StayInWindow(const Rectf& windowSize);
};

