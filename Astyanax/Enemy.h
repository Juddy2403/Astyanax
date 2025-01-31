#pragma once
#include <Vector2f.h>
#include "GameObject.h"
#include "SoundManager.h"
#include "Avatar.h"
#include "Level.h"

class Texture;
class Blast;
class PickUp;
class Enemy: public GameObject
{
protected:
	Vector2f m_Velocity;
	const float m_HorSpeed;
	const float m_VerSpeed;
	bool m_IsVelocityReset;
	const int m_MaxHealth;
	Point2f m_SpawnPoint;
	const float m_RespawnTime;
	float m_AccumRespawnTime;
	Blast* m_pBlast;
	const int m_ScorePerEnemy;
	PickUp* m_pPickUp;

	void ResetPosition();
	virtual void ManageRespawnTime(float elapsedSec);
	void MoveEnemy(float elapsedSec);
	void GeneratePickUp();
public:
	explicit Enemy(const std::string& path, const Point2f& spawnPoint, int nrOfCols, 
		const Vector2f& velocity, int health, int nrOfFrames, int nrFramesPerSec, float respawnTime, int scorePerEnemy);
	virtual ~Enemy() override;
	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;
	virtual bool WeaponCollision(const Rectf& weaponShape, int powerPercent, const SoundManager& sound,int &score);
	virtual void ManagePickUp(float elapsedSec, const Level& level, Avatar& avatar);
	virtual void Knockback(bool isGoingLeft = true);
	Vector2f GetVelocity() const;
	void OutsideLevelCollision(const Rectf& window);
	void ResetSpawnPoint(const Point2f& pos);
	void DoBlast(float elapsedSec);
	void Respawn();
	Point2f GetRespawnPoint() const;
};

