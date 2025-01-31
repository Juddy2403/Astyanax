#include "pch.h"
#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager():
	m_pLevelMusic{new SoundStream{"Sounds/LevelMusic.mp3"}}
	, m_pAttackSound{ new SoundEffect{"Sounds/Attack.mp3"} }
	, m_pEnemyDeathSound{ new SoundEffect{"Sounds/EnemyDeath.mp3"} }
	, m_pAvatarHitSound{ new SoundEffect{"Sounds/PlayerHit.mp3"} }
	, m_pAvatarDeathSound{ new SoundEffect{"Sounds/PlayerDeath.mp3"} }
	, m_pChangePowerUpSound{ new SoundEffect{"Sounds/PowerChange.mp3"} }
	, m_pPauseGameSound{ new SoundEffect{"Sounds/PauseJingle.mp3"} }
	, m_pBlastAttackSound{ new SoundEffect{"Sounds/BlastAttack.mp3"} }
	, m_pBoltAttackSound{ new SoundEffect{"Sounds/BoltAttack.mp3"} }
	, m_pBindAttackSound{ new SoundEffect{"Sounds/BindAttack.mp3"} }
{
}

SoundManager::~SoundManager()
{
	delete m_pLevelMusic;
	delete m_pAttackSound;
	delete m_pEnemyDeathSound;
	delete m_pAvatarHitSound;
	delete m_pAvatarDeathSound;
	delete m_pChangePowerUpSound;
	delete m_pPauseGameSound;
	delete m_pBlastAttackSound;
	delete m_pBoltAttackSound;
	delete m_pBindAttackSound;
}

void SoundManager::PlayMusic() const
{
	m_pLevelMusic->SetVolume(40); 
	m_pLevelMusic->Play(true);
}

void SoundManager::PlayAttackSound() const
{
	m_pAttackSound->Play(0);
}

void SoundManager::PlayEnemyDeathSound() const
{
	m_pEnemyDeathSound->Play(0);
}

void SoundManager::PlayAvatarHitSound() const
{
	m_pAvatarHitSound->Play(0);
}

void SoundManager::PlayAvatarDeathSound() const
{
	m_pAvatarDeathSound->Play(0);
}

void SoundManager::PlayChangePowerUpSound() const
{
	m_pChangePowerUpSound->Play(0);
}

void SoundManager::PlayPauseGameSound() const
{
	m_pPauseGameSound->Play(0);
}

void SoundManager::PlayBlastAttackSound() const
{
	m_pBlastAttackSound->Play(0);
}

void SoundManager::PlayBoltAttackSound() const
{
	m_pBoltAttackSound->Play(0);
}

void SoundManager::PlayBindAttackSound() const
{
	m_pBindAttackSound->Play(12);
}

void SoundManager::PauseAllSounds() const
{
	m_pLevelMusic->Pause();
	m_pAttackSound->PauseAll();
	m_pEnemyDeathSound->PauseAll();
	m_pAvatarHitSound->PauseAll();
}

void SoundManager::ResumeAllSounds() const
{
	m_pLevelMusic->Resume();
	m_pAttackSound->ResumeAll();
	m_pEnemyDeathSound->ResumeAll();
	m_pAvatarHitSound->ResumeAll();
}
