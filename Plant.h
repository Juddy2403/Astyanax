#pragma once
#include "Enemy.h"
#include "Flame.h"
#include <vector>

class Plant final : public Enemy
{
public:
	static int m_NrOfInstances;

	explicit Plant(const Point2f& pos);
	virtual ~Plant() override;
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	void OutsideLevelFlameCollision(const Rectf& window);
	bool HasFlameHitAvatar(const Rectf& avatarShape);
private:

	std::vector<Flame*> m_pFlames;
	void SpawnFlames();
};

