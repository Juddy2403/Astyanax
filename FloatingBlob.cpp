#include "pch.h"
#include "FloatingBlob.h"
#include <iostream>

int FloatingBlob::m_NrOfInstances{};

//respawn point 1800, 400
FloatingBlob::FloatingBlob(const Point2f& pos, bool isGoingRight) :
	Enemy("Images/Enemy1.png", pos, 2, Vector2f{ isGoingRight ? 150.f : -150.f,50.f }, 50, 2, 2, 6.f,100),
	m_AccumFlyTime{},
	m_FlyTime{ 0.6f }
{
	m_NrOfInstances++;
}

FloatingBlob::~FloatingBlob()
{
	Enemy::~Enemy();
	m_NrOfInstances--;
}

void FloatingBlob::Update(float elapsedSec)
{
	Enemy::Update(elapsedSec);
	ManageFlyTime(elapsedSec);
}

void FloatingBlob::ManageFlyTime(float elapsedSec)
{
	m_AccumFlyTime += elapsedSec;
	if (m_AccumFlyTime >= m_FlyTime)
	{
		m_Velocity.y *= -1;
		m_AccumFlyTime -= m_FlyTime;
	}
}
