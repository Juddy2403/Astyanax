#pragma once
#include "GameObject.h"
#include <Vector2f.h>

class Flame final : public GameObject
{
private:
	 Vector2f m_Acceleration;
	Vector2f m_Velocity;
public:
	explicit Flame(const Point2f& pos, bool isGoingLeft);
	bool OutsideLevelCollision(const Rectf& window);
	void MoveFlame(float elapsedSec);
	virtual void Draw() const override;

};

