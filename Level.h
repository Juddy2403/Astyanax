#pragma once
#include "utils.h"
#include "SoundManager.h"

#pragma region classForwarding 
class SoundManager;
class Texture;
class Pillar;
class Projectile;
class Avatar;
class Camera;
class Enemy;
class EnemyNrOne;
#pragma endregion

class Level final
{
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	const Texture* m_pBackgroundTexture;
	const Texture* m_pGroundTexture;
	Rectf m_Boundaries;
	float m_BackgroundLeft;
	int m_Score;
	int m_BackgroundRow;
	SoundManager* m_Sounds;
	const float m_AvgYSpawnPos;
	bool m_HasWon;
	//Game objects
	Avatar* m_Avatar;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Pillar*> m_pPillar;

	//Bolt background animation
	const float m_FlashTime;
	float m_AccumFlashTime;
	void UpdateBackgroundRow(float elapsedSec);

	void InitPillars();
	void ClearSoundManager();
	void ClearPillars();
	void ClearEnemies();
	void ClearAvatar();
	void ClearTextures();
	void AddEnemies(const Rectf& window);
	void AddBoss(const Rectf& window);
	void AddPlants(const Rectf& window);
	void AddSkeleton(const Rectf& window);
	void AddFloatingEyeballs(const Rectf& window);
	void AddFloatingBlobs(const Rectf& window);
	void UpdatePillars(float elapsedSec);
	void ManageEnemyBehaviour(float elapsedSec, const Camera& camera);
	void ManagePlant(Enemy* ptr, const Camera& camera);
	void ManageHitEnemy(Enemy* ptr, const Camera& camera, float elapsedSec);
	void ManageSkeleton(Enemy* ptr);
	void ResetSpawnPoint(Enemy* ptr, const Rectf& window);
	void ManageResetPosPlant(const Rectf& window, Enemy* ptr);
	void ManageResetPosSkeleton(const Rectf& window, Enemy* ptr);
	void ManageResetPosFloatingEyeball(const Rectf& window, Enemy* ptr);
	void ManageResetPosFloatingBlob(Enemy* ptr, const Rectf& window);
	void ResetPlantPos(const Point2f& plantSpawnPoint2, const Rectf& window, Enemy* ptr);
	void ResetSkeletonPos(const Point2f& skeletonSpawnPoint2, const Rectf& window, Enemy* ptr);
	void KeepInWindow(Enemy* ptr, const Camera& camera);
	void ManageFloatingEyeball(Enemy* ptr, float elapsedSec);
	bool IsHittingAvatar(Enemy* ptr);
	bool BossWeaponCollision(Enemy* ptr, bool& retFlag);
	bool SkeletonWeaponCollision(Enemy* ptr, bool& retFlag);
	void DrawPillars() const;
	void DrawEnemies() const;
	void DrawGround() const;
	Rectf GetWindowRect(const Camera& camera) const;

	bool IsCollidingHorizontalLine(const std::vector<Point2f>& ver,const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	bool IsCollidingVerticalLines(const std::vector<Point2f>& ver,const Rectf& actorShape, utils::HitInfo& hitInfo) const;
	void ResetVerticalPos(Vector2f& actorVelocity, Rectf& actorShape, utils::HitInfo& hitInfo) const;
	void ResetHorizontalPos(Vector2f& actorVelocity, Rectf& actorShape, utils::HitInfo& hitInfo, bool isGoingLeft) const;

public:
	explicit Level();
	~Level();
	Level& operator=(const Level& rhs) = delete;
	Level(const Level& other) = delete;
	Level& operator=(Level&& rhs) = delete;
	Level(Level&& other) = delete;
	void Draw() const;
	void DrawBackground() const;
	void Update(float elapsedSec, const Camera& camera);
	void UpdateBackgroundLeft(float elapsedSec, bool isGoingLeft);
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isGoingLeft) const;
	bool IsOnGround(const Rectf& actorShape) const;
	Rectf GetBoundaries() const;
	Avatar* GetAvatar() const;
	int GetScore() const;
	SoundManager* GetSoundManager() const;
	bool IsCollidingWithWalls(const Rectf& actorShape) const;
	void AvatarPerformPowerUp();
	void AvatarAttackButton();
	void AvatarPowerUpButton();
	void SetScore(int score);
	bool HasWon() const;
};

