#include "pch.h"
#include <vector>
#include <Vector2f.h>
#include "Camera.h"
#include "Level.h"
#include "Texture.h"
#include "SvgParser.h"
#include "Enemy.h"
#include "Avatar.h"
#include "FloatingBlob.h"
#include "FloatingEyeball.h"
#include "Projectile.h"
#include "Camera.h"
#include "Pillar.h"
#include "Skeleton.h"
#include "Plant.h"
#include "HUD.h"
#include "Boss.h"
#include <iostream>

#pragma region Initialization
Level::Level()
	: m_pBackgroundTexture{ new Texture{ "Images/BackGround.png"} }
	, m_pGroundTexture{ new Texture{"Images/Ground.png" } }
	, m_BackgroundLeft{ 100.f }
	, m_Avatar{ new Avatar{} }
	, m_Sounds{ new SoundManager{} }
	, m_Score{}
	, m_BackgroundRow{}
	, m_FlashTime{ 0.1f }
	, m_AccumFlashTime{}
	, m_AvgYSpawnPos{564.f}
	, m_HasWon{false}
{
	m_Boundaries = Rectf{ 0.f,HUD::m_HudHeight, m_pBackgroundTexture->GetWidth(), m_pBackgroundTexture->GetHeight() / 3.f };
	SVGParser::GetVerticesFromSvgFile("Images/Ground2.svg", m_Vertices);
	for (std::vector<Point2f>& verVect : m_Vertices)
		for (Point2f& ver : verVect)
			ver.y += m_Boundaries.bottom;
	InitPillars();
	m_Sounds->PlayMusic();
}

void Level::InitPillars()
{ 
	const Point2f pillar1SpawnPoint{ 1500.f,260.f }, pillar2SpawnPoint{ 4100.f,356.f };
	m_pPillar.push_back(new Pillar{ pillar1SpawnPoint });
	m_pPillar.push_back(new Pillar{ pillar2SpawnPoint });
}
#pragma endregion

#pragma region Cleanup
Level::~Level()
{
	ClearTextures();
	ClearAvatar();
	ClearEnemies();
	ClearPillars();
	ClearSoundManager();
}

void Level::ClearSoundManager()
{
	delete m_Sounds;
}

void Level::ClearPillars()
{
	for (Pillar* ptr : m_pPillar)
		delete ptr;
	m_pPillar.clear();
}

void Level::ClearEnemies()
{
	for (Enemy* ptr : m_pEnemies)
		delete ptr;
	m_pEnemies.clear();
}

void Level::ClearAvatar()
{
	delete m_Avatar;
	m_Avatar = nullptr;
}

void Level::ClearTextures()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;
	delete m_pGroundTexture;
	m_pGroundTexture = nullptr;
}
#pragma endregion

#pragma region Update
void Level::UpdateBackgroundLeft(float elapsedSec, bool isGoingLeft)
{
	const float speed{ 80.f };
	if (isGoingLeft) m_BackgroundLeft -= speed * elapsedSec;
	else m_BackgroundLeft += speed * elapsedSec;
}

void Level::UpdateBackgroundRow(float elapsedSec)
{
	m_AccumFlashTime += elapsedSec;
	if (m_AccumFlashTime >= m_FlashTime)
	{
		m_AccumFlashTime = 0.f;
		m_BackgroundRow = (m_BackgroundRow == 1) ? 2 : 1;
	}
}

void Level::Update(float elapsedSec, const Camera& camera)
{
	if (m_Avatar->IsAlive())
	{
		m_Avatar->Update(elapsedSec, *this);
		m_Avatar->OutsideLevelCollision(GetBoundaries());
		if (m_Avatar->IsBoltHappening()) UpdateBackgroundRow(elapsedSec);
		else m_BackgroundRow = 0;
		ManageEnemyBehaviour(elapsedSec, camera);
		AddEnemies(GetWindowRect(camera));
		UpdatePillars(elapsedSec);
		
	}
}

void Level::UpdatePillars(float elapsedSec)
{
	for (Pillar* ptr : m_pPillar)
		ptr->Update(elapsedSec, *m_Avatar, *this);
}

void Level::ManageEnemyBehaviour(float elapsedSec, const Camera& camera)
{
	for (Enemy* ptr : m_pEnemies)
	{
		if (!m_Avatar->IsBindHappening() && typeid(*ptr) != typeid(FloatingEyeball)) ptr->Update(elapsedSec); //enemies can't move during bind
		ptr->DoBlast(elapsedSec);
		ptr->ManagePickUp(elapsedSec, *this, *m_Avatar);
		if (typeid(*ptr) == typeid(FloatingEyeball))  ManageFloatingEyeball(ptr, elapsedSec);

		ResetSpawnPoint(ptr, GetWindowRect(camera));
		ManageHitEnemy(ptr, camera, elapsedSec);

		if (ptr->IsAlive() == false && typeid(*ptr) == typeid(Boss)) m_HasWon = true;

		if (ptr->IsAlive())
		{
			if(!m_Avatar->IsBindHappening())
			{
				if (typeid(*ptr) == typeid(FloatingEyeball)) KeepInWindow(ptr, camera);
				if (typeid(*ptr) == typeid(Plant)) ManagePlant(ptr, camera);
				if (typeid(*ptr) == typeid(Skeleton)) ManageSkeleton(ptr);
				if (typeid(*ptr) == typeid(Boss))
				{
					Boss* boss = static_cast<Boss*>(ptr);
					boss->HandleLevelCollision(*this);
					boss->FollowAvatar(m_Avatar->GetShape());
				}
				if (IsHittingAvatar(ptr)) m_Avatar->Hit(*m_Sounds);
			}
			ptr->OutsideLevelCollision(GetBoundaries());
		}
	}
}

void Level::ManagePlant(Enemy* ptr, const Camera& camera)
{
	Plant* plant = static_cast<Plant*>(ptr);
	if (plant->HasFlameHitAvatar(m_Avatar->GetCollisionRect()))  m_Avatar->Hit(*m_Sounds);
	plant->OutsideLevelFlameCollision(GetWindowRect(camera));
}

void Level::ManageHitEnemy(Enemy* ptr, const Camera& camera, float elapsedSec)
{ 
	//normal weapon collision
	if (ptr->WeaponCollision(m_Avatar->GetWeaponShape(), m_Avatar->GetHitPower(), *m_Sounds, m_Score))
	{
		if (typeid(*ptr) != typeid(Skeleton) && typeid(*ptr) != typeid(Plant) &&
			typeid(*ptr) != typeid(Boss)) ptr->Knockback();
		else if (typeid(*ptr) == typeid(Skeleton))
		{
			Skeleton* skeleton = static_cast<Skeleton*>(ptr);
			skeleton->Knockback(skeleton->IsGoingLeft());
		}
		else if (typeid(*ptr) == typeid(Boss))
		{
			Boss* boss = static_cast<Boss*>(ptr);
			boss->Knockback(boss->IsGoingLeft());
		}
	}
	//so that enemies can take more than 1 hit
	if (m_Avatar->IsBindHappening()) ptr->ManageInvincibleTime(elapsedSec);
	//powerup hit
	if (utils::IsOverlapping(ptr->GetShape(), GetWindowRect(camera)) &&
		(m_Avatar->IsBlastHappening() || m_Avatar->IsBoltHappening()))
	{
		ptr->WeaponCollision(GetWindowRect(camera), m_Avatar->GetPowerUpAttackPower(), *m_Sounds, m_Score);
	}
}

Rectf Level::GetWindowRect(const Camera& camera) const
{
	const Rectf window{ Rectf{ camera.GetCameraPos().x
	,HUD::m_HudHeight,camera.GetWindow().width,camera.GetWindow().height- HUD::m_HudHeight } };
	return window;
}

void Level::ManageFloatingEyeball(Enemy* ptr, float elapsedSec)
{
	FloatingEyeball* floatingEyeball = static_cast<FloatingEyeball*>(ptr);
	if (!m_Avatar->IsBindHappening())
	{
		floatingEyeball->Update(elapsedSec, *this);
		if (floatingEyeball->IsProjectileHit(*m_Avatar))
			m_Avatar->Hit(*m_Sounds);
	}
	floatingEyeball->DoProjectileBlast(elapsedSec);
}

void Level::ManageSkeleton(Enemy* ptr)
{
	Skeleton* skeleton = static_cast<Skeleton*>(ptr);
	skeleton->HandleLevelCollision(*this);
	skeleton->FollowAvatar(m_Avatar->GetShape());
}
#pragma endregion

#pragma region Helper update functions
void Level::AddEnemies(const Rectf& window)
{ 
	AddFloatingBlobs(window);
	AddFloatingEyeballs(window);
	AddSkeleton(window);
	AddPlants(window);
	AddBoss(window);
}

void Level::AddBoss(const Rectf& window)
{
	const Point2f bossSpawnPoint{ 6000.f,250.f };
	if (Boss::m_NrOfInstances == 0 && utils::IsPointInRect(bossSpawnPoint,
		Rectf{ window.left - window.width / 6,window.bottom,window.width,window.height }))
		m_pEnemies.push_back(new Boss(bossSpawnPoint));
}

void Level::AddPlants(const Rectf& window)
{
	const Point2f plantSpawnPoint{ 2110.f,300.f };
	if (Plant::m_NrOfInstances == 0 && utils::IsPointInRect(plantSpawnPoint,
		Rectf{ window.left - window.width / 6,window.bottom,window.width,window.height }))
		m_pEnemies.push_back(new Plant(plantSpawnPoint));
}

void Level::AddSkeleton(const Rectf& window)
{
	const Point2f skeletonSpawnPoint{ 1000.f,400.f };
	if (Skeleton::m_NrOfInstances == 0 && utils::IsPointInRect(skeletonSpawnPoint,
		Rectf{ window.left - window.width / 6,window.bottom,window.width,window.height }))
		m_pEnemies.push_back(new Skeleton(skeletonSpawnPoint));
}

void Level::AddFloatingEyeballs(const Rectf& window)
{
	const Point2f eyeball1SpawnPoint{ 1400.f,m_AvgYSpawnPos }, eyeball2SpawnPoint{ 1450, m_AvgYSpawnPos };;
	if (FloatingEyeball::m_NrOfInstances == 0 && utils::IsPointInRect(eyeball1SpawnPoint, window))
		m_pEnemies.push_back(new FloatingEyeball(eyeball1SpawnPoint, false));
	if (FloatingEyeball::m_NrOfInstances == 1 && utils::IsPointInRect(eyeball1SpawnPoint,
		Rectf{ window.left - window.width - 50.f,window.bottom,window.width,window.height }))
		m_pEnemies.push_back(new FloatingEyeball(eyeball2SpawnPoint, true));
}

void Level::AddFloatingBlobs(const Rectf& window)
{
	const Point2f blob1SpawnPoint{ 750.f,m_AvgYSpawnPos }, blob2SpawnPoint{ 1100.f,m_AvgYSpawnPos + 30.f },
		blob3SpawnPoint{ 2400.f,m_AvgYSpawnPos };
	if (FloatingBlob::m_NrOfInstances == 0)
		m_pEnemies.push_back(new FloatingBlob(blob1SpawnPoint, false));
	if (FloatingBlob::m_NrOfInstances == 1)
		m_pEnemies.push_back(new FloatingBlob(blob2SpawnPoint, false));
	if (FloatingBlob::m_NrOfInstances == 2 && window.left >= blob3SpawnPoint.x)
		m_pEnemies.push_back(new FloatingBlob(blob3SpawnPoint, true));
}

void Level::ResetSpawnPoint(Enemy* ptr, const Rectf& window)
{
	if (typeid(*ptr) == typeid(FloatingBlob)) ManageResetPosFloatingBlob(ptr, window);
	if (typeid(*ptr) == typeid(FloatingEyeball)) ManageResetPosFloatingEyeball(window, ptr);
	if (typeid(*ptr) == typeid(Skeleton)) ManageResetPosSkeleton(window, ptr);
	if (typeid(*ptr) == typeid(Plant)) ManageResetPosPlant(window, ptr);

}

void Level::ManageResetPosPlant(const Rectf& window, Enemy* ptr)
{
	const Point2f plantSpawnPoint2{ 2500.f,350.f };
	ResetPlantPos(plantSpawnPoint2, window, ptr);
	const Point2f plantSpawnPoint3{ 3400.f,200.f };
	ResetPlantPos(plantSpawnPoint3, window, ptr);
	const Point2f plantSpawnPoint4{ 5150.f,250.f };
	ResetPlantPos(plantSpawnPoint4, window, ptr);
}

void Level::ManageResetPosSkeleton(const Rectf& window, Enemy* ptr)
{
	const Point2f skeletonSpawnPoint2{ 1800.f,300.f };
	ResetSkeletonPos(skeletonSpawnPoint2, window, ptr);
	const Point2f skeletonSpawnPoint3{ 4100.f,350.f };
	ResetSkeletonPos(skeletonSpawnPoint3, window, ptr);
	const Point2f skeletonSpawnPoint4{ 5500.f,250.f };
	ResetSkeletonPos(skeletonSpawnPoint4, window, ptr);
}

void Level::ManageResetPosFloatingEyeball(const Rectf& window, Enemy* ptr)
{
	const Point2f spawnPoint{ 4500.f,m_AvgYSpawnPos };
	if (utils::IsPointInRect(spawnPoint, window) && ptr->GetVelocity().x < 0) ptr->ResetSpawnPoint(spawnPoint);
}

void Level::ManageResetPosFloatingBlob(Enemy* ptr, const Rectf& window)
{
	const float spawnOffset{ 1000.f };
	if (ptr->GetVelocity().x < 0)
		ptr->ResetSpawnPoint(Point2f{ window.left + window.width + spawnOffset,m_AvgYSpawnPos });
	else ptr->ResetSpawnPoint(Point2f{ window.left - spawnOffset,m_AvgYSpawnPos });
}

void Level::ResetPlantPos(const Point2f& plantSpawnPos, const Rectf& window, Enemy* ptr)
{
	if (utils::IsPointInRect(plantSpawnPos, Rectf{ window.left - window.width / 6,window.bottom,window.width,window.height })
		&& ptr->GetRespawnPoint().x != plantSpawnPos.x)
	{
		ptr->ResetSpawnPoint(plantSpawnPos);
		ptr->Respawn();
	}
}

void Level::ResetSkeletonPos(const Point2f& skeletonSpawnPos, const Rectf& window, Enemy* ptr)
{
	if (utils::IsPointInRect(skeletonSpawnPos, window) && ptr->GetRespawnPoint().x != skeletonSpawnPos.x)
	{
		ptr->ResetSpawnPoint(skeletonSpawnPos);
		ptr->Respawn();
	}
}

void Level::KeepInWindow(Enemy* ptr, const Camera& camera)
{
	FloatingEyeball* enemyNrTwo = static_cast<FloatingEyeball*>(ptr);
	enemyNrTwo->StayInWindow(Rectf{ camera.GetWindow().left + camera.GetCameraPos().x,camera.GetWindow().bottom
		,camera.GetWindow().width,camera.GetWindow().height });
}

bool Level::IsHittingAvatar(Enemy* ptr)
{
	bool retFlag;
	bool retVal = SkeletonWeaponCollision(ptr, retFlag);
	if (retFlag) return retVal;

	bool retFlag2;
	bool retVal2 = BossWeaponCollision(ptr, retFlag2);
	if (retFlag2) return retVal2;

	return utils::IsOverlapping(ptr->GetShape(), m_Avatar->GetCollisionRect()) &&
		!m_Avatar->IsInvincible() ;
}
bool Level::BossWeaponCollision(Enemy* ptr, bool& retFlag)
{
	retFlag = true;
	if (typeid(*ptr) == typeid(Boss))
	{
		Boss* boss = static_cast<Boss*>(ptr);
		if (utils::IsOverlapping(boss->GetWeaponRect(), m_Avatar->GetCollisionRect()) && !m_Avatar->IsInvincible()
			&& boss->IsAttacking())
			return true;
	}
	retFlag = false;
	return {};
}
bool Level::SkeletonWeaponCollision(Enemy* ptr, bool& retFlag)
{
	retFlag = true;
	if (typeid(*ptr) == typeid(Skeleton))
	{
		Skeleton* skeleton = static_cast<Skeleton*>(ptr);
		if (utils::IsOverlapping(skeleton->GetWeaponRect(), m_Avatar->GetCollisionRect()) && !m_Avatar->IsInvincible()
			&& skeleton->IsAttacking())
			return true;
	}
	retFlag = false;
	return {};
}
#pragma endregion

#pragma region Draw
void Level::DrawBackground() const
{
	Rectf srcRect{ m_Boundaries };
	srcRect.left = -m_BackgroundLeft;
	Rectf destRect{ m_Boundaries };
	destRect.bottom = m_Boundaries.height * m_BackgroundRow;
	m_pBackgroundTexture->Draw(srcRect, destRect);
}
void Level::DrawGround() const
{
	Rectf srcRect{ m_Boundaries }, destRect{ m_Boundaries };
	destRect.bottom = m_Boundaries.height * m_BackgroundRow;
	m_pGroundTexture->Draw(srcRect, destRect);
}

void Level::Draw() const
{
	DrawGround();
	DrawEnemies();
	DrawPillars();
	m_Avatar->Draw();
}

void Level::DrawPillars() const
{
	for (Pillar* pillar : m_pPillar)
		pillar->Draw();
}

void Level::DrawEnemies() const
{
	for (Enemy* ptr : m_pEnemies)
	{
		ptr->Draw();
	}
}
#pragma endregion

#pragma region Ground collision
void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool isGoingLeft) const
{
	for (const std::vector<Point2f>& ver : m_Vertices)
	{
		utils::HitInfo hitInfo{};
		//collision with horizontal char line
		if (IsCollidingHorizontalLine(ver, actorShape, hitInfo))
		{
			//std::cout << "Horiz collision \n";
			ResetHorizontalPos(actorVelocity, actorShape, hitInfo, isGoingLeft);
			if (IsCollidingVerticalLines(ver, actorShape, hitInfo))
			{
				//std::cout << "Ver collision \n";
				ResetVerticalPos(actorVelocity, actorShape, hitInfo);
			}
		}
		//collision with vertical char line
		else if (IsCollidingVerticalLines(ver, actorShape, hitInfo))
		{
			//std::cout << "Ver collision \n";
			ResetVerticalPos(actorVelocity, actorShape, hitInfo);
		}
	}
}

void Level::ResetVerticalPos(Vector2f& actorVelocity, Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	actorShape.bottom = hitInfo.intersectPoint.y;
	if (actorVelocity.y < 0)
	{
		actorVelocity.y = 0.f;
	}
}

void Level::ResetHorizontalPos(Vector2f& actorVelocity, Rectf& actorShape, utils::HitInfo& hitInfo, bool isGoingLeft) const
{
	if (!isGoingLeft)
		actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
	else
		actorShape.left = hitInfo.intersectPoint.x;
	actorVelocity.x = 0.f;
}

bool Level::IsCollidingHorizontalLine(const std::vector<Point2f>& ver, const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	Point2f bottomLeft{ actorShape.left,actorShape.bottom + actorShape.height / 10 };
	Point2f bottomRight{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height / 10 };

	return utils::Raycast(ver, bottomLeft, bottomRight, hitInfo);
}

bool Level::IsCollidingVerticalLines(const std::vector<Point2f>& ver, const Rectf& actorShape, utils::HitInfo& hitInfo) const
{
	float borderDist{ 5.f };
	Point2f bottomLeft{ actorShape.left + borderDist,actorShape.bottom - 2.f };
	Point2f bottomRight{ actorShape.left + actorShape.width - borderDist,actorShape.bottom - 2.f };
	Point2f middleTop{ actorShape.left + actorShape.width / 2.f, actorShape.bottom + actorShape.height };

	return utils::Raycast(ver, bottomLeft, middleTop, hitInfo) ||
		utils::Raycast(ver, bottomRight, middleTop, hitInfo);
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo{};
	for (std::vector<Point2f> ver : m_Vertices)
		if (IsCollidingVerticalLines(ver, Rectf{ actorShape.left, actorShape.bottom - 1.f,actorShape.width,
			actorShape.height }, hitInfo))
			return true;
	return false;

}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}
#pragma endregion

Avatar* Level::GetAvatar() const
{
	return m_Avatar;
}

int Level::GetScore() const
{
	return m_Score;
}

SoundManager* Level::GetSoundManager() const
{
	return m_Sounds;
}

bool Level::IsCollidingWithWalls(const Rectf& actorShape) const
{
	for (const std::vector<Point2f>& ver : m_Vertices)
	{
		utils::HitInfo hitInfo{};
		if (IsCollidingHorizontalLine(ver, actorShape, hitInfo)) return true;
	}
	return false;
}

void Level::AvatarPerformPowerUp()
{
	m_Avatar->PerformPowerUp(*m_Sounds);
}

void Level::AvatarAttackButton()
{
	m_Avatar->AttackButtonHit();
	m_Sounds->PlayAttackSound();
}

void Level::AvatarPowerUpButton()
{
	m_Avatar->PowerUpButtonHit();
}

void Level::SetScore(int score)
{
	m_Score = score;
}




bool Level::HasWon() const
{
	return m_HasWon;
}
