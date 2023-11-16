#pragma once
#include "Enemy.h"
#include "Level.h"

class Boss : public Enemy
{
private:
	enum class ActionState {
		spawning,
		walking,
		attacking,
		jumping
	};
	ActionState m_ActionState;
	const float m_JumpSpeed;
	bool m_IsSpawnAnimationOver;
	bool m_IsGoingLeft;
	const Vector2f m_Acceleration;
	bool m_IsOnGround;

	float m_AccumMoveTime;
	float m_MovedDist;
	const float m_MinMovedDist;

	void DrawTextures(const Rectf& srcRect) const;
	void SetSpriteFrame(Rectf& srcRect) const;
	void DoFullSpawnAnimation();
	void SetNrOfFrames();
	void FlipTexture() const;
public:
	explicit Boss(const Point2f& pos);
	virtual ~Boss() override;
	virtual void Update(float elapsedSec) override;
	void HandleLevelCollision(const Level& level);
	virtual void Draw() const override;
	void FollowAvatar(const Rectf& avatarShape);
	Rectf GetWeaponRect() const;
	bool IsAttacking() const;
	bool IsGoingLeft() const;
	static int m_NrOfInstances;

};

