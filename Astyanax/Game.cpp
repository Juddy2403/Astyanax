#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "Level.h"
#include "Avatar.h"
#include "Texture.h"
#include <iostream>

bool Delay(const float delayTime, float& accumTime, float elapsedSec)
{
	accumTime += elapsedSec;
	if (accumTime >= delayTime)
	{
		accumTime = 0;
		return true;
	}
	return false;
}

Game::Game(const Window& window)
	:BaseGame{ window }
	, m_HUD{}
	, m_IsPaused{ false }
	, m_LifePoints{ 3 }
	, m_pGameOverText { new Texture {"Images/GameOver.png"} }
	, m_pLevelClearText{new Texture {"Images/RoundClear.png"}}
	, m_IsGameOver{false}
	, m_IsLevelClear{false}
	, m_GameOverTime{5.f}
	, m_GameOverAccumTime{}
	, m_pGameOverSong{new SoundStream{"Sounds/GameOver.mp3"}}
	, m_pLevelClearSong{ new SoundStream{"Sounds/BossClear.mp3"} }
	, m_HasWon{false}
	, m_LevelClearAccumTime{}
	, m_LevelClearTime{4.f}
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
	ClearGamePointers();
}

void Game::ClearGamePointers()
{
	delete m_pGameOverSong;
	m_pGameOverSong = nullptr;
	delete m_pGameOverText;
	m_pGameOverText = nullptr;
	delete m_pLevelClearSong;
	m_pLevelClearSong = nullptr;
	delete m_pLevelClearText;
	m_pLevelClearText = nullptr;
}

void Game::Initialize()
{
	InitLevel();
	InitCamera();
}

void Game::InitLevel()
{
	m_Level = new Level{};
}

void Game::InitCamera()
{
	m_Camera = new Camera{ GetViewPort().width, GetViewPort().height };
	m_Camera->SetLevelBoundaries(m_Level->GetBoundaries());
}

void Game::Cleanup()
{
	ClearCamera();
	ClearLevel();
}

void Game::ClearLevel()
{
	delete m_Level;
	m_Level = nullptr;
}

void Game::ClearCamera()
{
	delete m_Camera;
	m_Camera = nullptr;
}


void Game::Update(float elapsedSec)
{
	if (m_IsGameOver)
	{
		if (Delay(m_GameOverTime, m_GameOverAccumTime, elapsedSec))
		{
			m_IsGameOver = false;
			m_IsPaused = false;
		}
	}
	if (m_Level->HasWon() && !m_HasWon)
	{
		m_IsPaused = true;
		m_HasWon = true;
		m_pLevelClearSong->Play(false);

	}
	if (m_HasWon)
	{
		if (Delay(m_LevelClearTime, m_LevelClearAccumTime, elapsedSec))
		{
			m_LifePoints = 3;
			m_IsPaused = false;
			m_HasWon = false;
			Cleanup();
			Initialize();
			m_HUD.SetLifePoints(m_LifePoints);
		}
	}
	if (!m_IsPaused)
	{
		const Avatar* avatar = m_Level->GetAvatar();
		if (avatar->IsAlive())
		{
			m_Level->Update(elapsedSec, *m_Camera);
			ParallaxEffects(elapsedSec);
			m_HUD.Update(avatar, m_Level->GetScore());
		}
		else
		{
			m_Score = m_Level->GetScore();
			Cleanup();
			Initialize();
			m_Level->SetScore(m_Score);
			m_LifePoints--;
			m_HUD.SetLifePoints(m_LifePoints);
			if (m_LifePoints == 0)
			{
				m_IsGameOver = true;
				m_IsPaused = true;
				m_LifePoints = 3;
				m_pGameOverSong->Play(false);
			}
		}
	}

}

void Game::ParallaxEffects(float elapsedSec)
{
	const Avatar* avatar = m_Level->GetAvatar();
	if (m_Camera->IsMoving())
		m_Level->UpdateBackgroundLeft(elapsedSec, avatar->IsGoingLeft());
}

void Game::Draw() const
{
	const Avatar* avatar = m_Level->GetAvatar();
	ClearBackground();

	glPushMatrix();
	m_Level->DrawBackground();

	m_Camera->Transform(avatar->GetShape(), avatar->IsMovingHoriz());
	m_Level->Draw();

	glPopMatrix();

	m_HUD.Draw();

	if (m_IsGameOver) m_pGameOverText->Draw();
	if (m_HasWon) m_pLevelClearText->Draw(Point2f{ 0.f,HUD::m_HudHeight });

}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	if (e.keysym.sym == SDLK_ESCAPE) PauseUnpauseGame();
	if (!m_IsPaused)
	{
		switch (e.keysym.sym)
		{
		case SDLK_LSHIFT:
			m_Level->AvatarPowerUpButton();
			m_Level->GetSoundManager()->PlayChangePowerUpSound();
			break;
		case SDLK_z:
			m_Level->AvatarAttackButton();
			break;
		case SDLK_i:
			PrintControls();
			break;
		case SDLK_x:
			m_Level->AvatarPerformPowerUp();
			break;
		}
	}
}

void Game::PauseUnpauseGame()
{
	m_IsPaused = !m_IsPaused;
	if (m_IsPaused) m_Level->GetSoundManager()->PauseAllSounds();
	else  m_Level->GetSoundManager()->ResumeAllSounds();
	m_Level->GetSoundManager()->PlayPauseGameSound();
}

void Game::PrintControls()
{
	std::cout << "--- Controls --- \n";
	std::cout << "Arrows for moving and jumping \n";
	std::cout << "Z key for attacking \n";
	std::cout << "X key for using selected power up \n";
	std::cout << "LSHIFT for changing selected power up type \n";
	std::cout << "ESC for pausing the game \n";
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}

}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
