#pragma once

class Texture;
class GameObject
{
protected:
	const Texture* m_pTexture;
	Rectf m_Shape;
	bool m_IsAlive;
	int m_Health;
	//sprite variables
	const int m_NrOfCols;
	const int m_NrOfRows;
	float m_ClipHeight;
	float m_ClipWidth;
	int m_NrOfFrames;
	int m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;
	//Invincible mode when takes damage
	float m_AccumInvincibleTime;
	const float m_InvincibleTime;
	bool m_IsInvincible;
	//Damage animation
	bool m_IsShowing;
	const float m_ShowTime;
	float m_AccumShowTime;
	
	void ManageShowTime(float elapsedSec);
	virtual void UpdateFrames(float elapsedSec);
public:
	explicit GameObject(const std::string& path, int nrOfRows, int nrOfCols, const Point2f& pos,
	int nrOfFrames, int nrFramesPerSec, int health);
	void ManageInvincibleTime(float elapsedSec);

	virtual ~GameObject();
	GameObject& operator=(const GameObject& rhs) = delete;
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& rhs) = delete;
	GameObject(GameObject&& other) = delete;

	virtual void Update(float elapsedSec);
	virtual void Draw() const = 0;
	virtual bool IsAlive() const;
	virtual Rectf GetShape() const;
	bool IsInvincible() const;

};

