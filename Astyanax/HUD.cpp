#include "pch.h"
#include "HUD.h"
#include "Texture.h"
#include "utils.h"
#include "Avatar.h"
#include <iostream>

const Color4f HUD::m_Orange{Color4f{1.f, 0.1f, 0.f, 1.f}};
const Color4f HUD::m_Yellow{Color4f{1.f, 0.7f, 0.f, 1.f}};
const float HUD::m_HudHeight{164.f};

HUD::HUD() : m_BlackBackground(0.f, 0.f, 768.f, m_HudHeight)
, m_Score{}
, m_pScoreText{ new Texture{"SCORE","zig.ttf",34, Color4f{1.f,1.f,1.f,1.f}} }
, m_pScoreNrText{ new Texture{"0000000","zig.ttf",34, Color4f{1.f,1.f,1.f,1.f}} }
, m_pR1Text{ new Texture{"R1-1","zig.ttf",36, Color4f{1.f,1.f,1.f,1.f}} }
, m_pLivesImage{ new Texture{"Images/lives.png"} }
, m_pNrLivesText{ new Texture{"=03","zig.ttf",34, Color4f{1.f,1.f,1.f,1.f}} }
, m_pPWYellowText{ new Texture{"PW","zig.ttf",34, m_Yellow } }
, m_pPWOrangeText{ new Texture{"PW","zig.ttf",34, m_Orange } }
, m_pLFYellowText{ new Texture{"LF","zig.ttf",34, m_Yellow } }
, m_pLFOrangeText{ new Texture{"LF","zig.ttf",34, m_Orange } }
, m_pSPYellowText{ new Texture{"SP","zig.ttf",34, m_Yellow } }
, m_pSPOrangeText{ new Texture{"SP","zig.ttf",34, m_Orange } }
, m_pSPImage{ new Texture{"Images/SP.png"} }
, m_pLFImage{ new Texture{"Images/LF.png"} }
, m_pPowerUpImage{ new Texture{"Images/PowerUp.png"} }
, m_HealthPoints{ 20 }
, m_SpellPoints{ 20 }
, m_PowerGaugeLen{200}
, m_PowerGaugeHeight{20}
, m_PowerPercent{100}
, m_PowerUpNr{}
, m_ScoreNrOfDigits{7}
, m_LifePoints{3}
{
}

HUD::~HUD()
{
	delete m_pScoreText;
	delete m_pScoreNrText;
	delete m_pR1Text;
	delete m_pLivesImage;
	delete m_pNrLivesText;
	delete m_pPWYellowText;
	delete m_pPWOrangeText;
	delete m_pLFYellowText;
	delete m_pLFOrangeText;
	delete m_pSPYellowText;
	delete m_pSPOrangeText;
	delete m_pSPImage;
	delete m_pLFImage;
	delete m_pPowerUpImage;
}

void HUD::Draw() const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	utils::FillRect(m_BlackBackground);

	float firstRowY{ 5 }, secondRowY{ 45 }, thirdRowY{ 85 }, fourthRowY{ 123 }, firstColX{ 10 }, secondColX{ 210 };
	float underColorBorder{ 4 };

	m_pScoreText->Draw(Point2f{ (secondColX - firstColX) / 2.f - m_pScoreText->GetWidth() / 2.f + firstColX,fourthRowY });
	m_pScoreNrText->Draw(Point2f{ firstColX,thirdRowY });

	m_pR1Text->Draw(Point2f{ firstColX,firstRowY });
	m_pLivesImage->Draw(Point2f{ firstColX,secondRowY });
	m_pNrLivesText->Draw(Point2f{ firstColX  + m_pLivesImage->GetWidth(),secondRowY });

	m_pPWOrangeText->Draw(Point2f{ secondColX,fourthRowY - underColorBorder });
	m_pPWYellowText->Draw(Point2f{ secondColX,fourthRowY });
	Rectf powerGauge{ secondColX + m_pPWOrangeText->GetWidth() + 1 , fourthRowY, m_PowerGaugeLen*m_PowerPercent/100.f, m_PowerGaugeHeight };
	utils::SetColor(m_Orange);
	utils::FillRect(powerGauge);
	powerGauge.bottom += underColorBorder;
	utils::SetColor(m_Yellow);
	utils::FillRect(powerGauge);

	m_pLFOrangeText->Draw(Point2f{ secondColX,secondRowY - underColorBorder });
	m_pLFYellowText->Draw(Point2f{ secondColX,secondRowY });
	for (size_t i = 0; i < m_HealthPoints; i++)
	{

		m_pLFImage->Draw(Point2f{ secondColX + m_pLFOrangeText->GetWidth() + (m_pLFImage->GetWidth() + 3) * i,secondRowY + underColorBorder });
	}

	m_pSPOrangeText->Draw(Point2f{ secondColX,firstRowY - underColorBorder });
	m_pSPYellowText->Draw(Point2f{ secondColX,firstRowY });
	for (size_t i = 0; i < m_SpellPoints; i++)
	{
		m_pSPImage->Draw(Point2f{ secondColX + m_pSPOrangeText->GetWidth() + (m_pSPImage->GetWidth() - 3) * i,firstRowY + underColorBorder });
	}

	Rectf destRect{ Rectf{ firstColX + m_pR1Text->GetWidth() + 10,firstRowY + 5,m_pPowerUpImage->GetHeight(),m_pPowerUpImage->GetHeight()} };
	Rectf srcRect{ m_pPowerUpImage->GetWidth() / 3 * m_PowerUpNr,0.f,m_pPowerUpImage->GetHeight(),m_pPowerUpImage->GetHeight() };
	m_pPowerUpImage->Draw(destRect, srcRect);
}

void HUD::Update(const Avatar* avatar, int score)
{
	m_PowerPercent = avatar->GetPowerPercent();
	m_HealthPoints = avatar->GetHealth();
	m_SpellPoints = avatar->GetSPPoints();
	m_PowerUpNr = avatar->GetPowerUp();
	m_Score = score;
	std::string scoreNr{std::to_string(m_Score)};
	int scoreSize{ int(scoreNr.size()) };
	if(scoreSize<m_ScoreNrOfDigits)
		for (int i = 0; i < m_ScoreNrOfDigits- scoreSize; i++)
		{
			scoreNr = "0" + scoreNr;
		}
	delete m_pScoreNrText;
	m_pScoreNrText = new Texture{ scoreNr,"zig.ttf",34, Color4f{1.f,1.f,1.f,1.f} };
}

void HUD::SetLifePoints(int lifePoints)
{
	m_LifePoints = lifePoints;
	std::string lifePointText{ "=0"+std::to_string(lifePoints) };
	delete m_pNrLivesText;
	m_pNrLivesText = new Texture{ lifePointText,"zig.ttf",34, Color4f{1.f,1.f,1.f,1.f} };
}
