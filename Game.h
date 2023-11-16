#pragma once
#include "BaseGame.h"
#include "HUD.h"
//#include "SoundEffect.h"
#include "SoundStream.h"

class Texture;
class Level;
class Camera;
class HUD;

class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void ClearGamePointers();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void PauseUnpauseGame();
	void PrintControls();
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:
	Level* m_Level;
	Camera* m_Camera;
	Texture* m_pGameOverText;
	Texture* m_pLevelClearText;
	HUD m_HUD;
	int m_LifePoints;
	int m_Score;
	bool m_IsPaused;
	bool m_IsGameOver;
	bool m_IsLevelClear;
	const float m_GameOverTime;
	float m_GameOverAccumTime;
	const float m_LevelClearTime;
	float m_LevelClearAccumTime;
	bool m_HasWon;
	SoundStream* m_pGameOverSong;
	SoundStream* m_pLevelClearSong;
	// FUNCTIONS
	void Initialize();
	void InitLevel();
	void InitCamera();
	void ParallaxEffects(float elapsedSec);
	void Cleanup();
	void ClearLevel();
	void ClearCamera();
	void ClearBackground() const;
};