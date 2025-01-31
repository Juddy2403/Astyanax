#pragma once
class Texture;
class Avatar;
class HUD final
{
private:
	const static Color4f m_Orange;
	const static Color4f m_Yellow;

	const Rectf m_BlackBackground;
	int m_Score;
	const int m_ScoreNrOfDigits;
	const Texture* m_pScoreText;
	const Texture* m_pScoreNrText;
	const Texture* m_pR1Text;
	const Texture* m_pLivesImage;
	const Texture* m_pNrLivesText;
	
	const Texture* m_pPWYellowText;
	const Texture* m_pPWOrangeText;
	const Texture* m_pLFYellowText;
	const Texture* m_pLFOrangeText;
	const Texture* m_pSPYellowText;
	const Texture* m_pSPOrangeText;

	const Texture* m_pSPImage;
	const Texture* m_pLFImage;
	const Texture* m_pPowerUpImage;

	int m_HealthPoints;
	int m_SpellPoints;
	int m_LifePoints;
	const float m_PowerGaugeLen;
	const float m_PowerGaugeHeight;
	int m_PowerPercent;
	int m_PowerUpNr;
public:
	const static float m_HudHeight;

	explicit HUD();
	~HUD();
	HUD& operator=(const HUD& rhs) = delete;
	HUD(const HUD& other) = delete;
	HUD& operator=(HUD&& rhs) = delete;
	HUD(HUD&& other) = delete;

	void Draw() const;
	void Update(const Avatar* avatar, int score);
	void SetLifePoints(int lifePoints);
};

