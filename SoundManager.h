#pragma once
#include "SoundEffect.h"
#include "SoundStream.h"

class SoundManager final
{
private:
	const SoundStream* m_pLevelMusic;
	const SoundEffect* m_pAttackSound;
	const SoundEffect* m_pEnemyDeathSound;
	const SoundEffect* m_pAvatarHitSound;
	const SoundEffect* m_pAvatarDeathSound;
	const SoundEffect* m_pChangePowerUpSound;
	const SoundEffect* m_pPauseGameSound;
	const SoundEffect* m_pBlastAttackSound;
	const SoundEffect* m_pBoltAttackSound;
	const SoundEffect* m_pBindAttackSound;
public:
	explicit SoundManager();

	~SoundManager();
	SoundManager& operator=(const SoundManager& rhs) = delete;
	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& rhs) = delete;
	SoundManager(SoundManager&& other) = delete;

	void PlayMusic() const;
	void PlayAttackSound() const;
	void PlayEnemyDeathSound() const;
	void PlayAvatarHitSound() const;
	void PlayAvatarDeathSound() const;
	void PlayChangePowerUpSound() const;
	void PlayPauseGameSound() const;
	void PlayBlastAttackSound() const;
	void PlayBoltAttackSound() const;
	void PlayBindAttackSound() const;
	void PauseAllSounds() const;
	void ResumeAllSounds() const;
};

