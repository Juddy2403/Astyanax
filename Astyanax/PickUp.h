#pragma once
#include "Level.h"
#include "GameObject.h"

class Level;
class Texture;
class PickUp final : public GameObject
{
public:
	enum class Type {
		weapon,
		power,
		wings,
		redPotion,
		bluePotion
	};
	explicit PickUp(const Point2f& pos, const Type& type);

	virtual void Draw() const override;       
	void Update(float elapsedSec,const Level& level);
	bool IsCollidingAvatar(const Rectf& avatarShape);
	Type GetType() const;
	bool IsOnGround() const;
protected:
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	const Type m_Type;
	bool m_IsOnGround;

	void MovePickup(float elapsedSec);

};

