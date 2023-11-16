#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(float width, float height) :
	m_Width{ width }
	, m_Height{ height }
	, m_LevelBoundaries{}
	, m_xPosition{}
	, m_IsMoving{true}
	, m_HasReachedBorder{false}
{
}

void Camera::Transform(const Rectf& target, bool isMoving)
{
	Track(target, isMoving);
	Clamp();
	glTranslatef(-m_xPosition, 0, 0);
}

Point2f Camera::GetCameraPos() const
{
	return Point2f{ m_xPosition,0.f };
}

bool Camera::IsMoving() const
{
	return m_IsMoving;
}

Rectf Camera::GetWindow() const
{
	return Rectf(0,0,m_Width,m_Height);
}

void Camera::Track(const Rectf& target, bool isMoving)
{
	if (target.left - m_xPosition <= m_Width / 4)
	{
		m_xPosition = target.left - m_Width / 4;
		SetMovingState(isMoving);

	}
	else if (target.left + target.width - m_xPosition >= m_Width / 4 * 3)
	{
		m_xPosition = target.left + target.width - m_Width / 4 * 3;
		SetMovingState(isMoving);
	}
	else m_IsMoving = false;
}

void Camera::SetMovingState(bool isMoving)
{
	if (isMoving && !m_HasReachedBorder) m_IsMoving = true;
	else m_IsMoving = false;
}

void Camera::Clamp()
{
	if (m_xPosition < m_LevelBoundaries.left)
	{
		m_xPosition = m_LevelBoundaries.left + 1.f;
		m_HasReachedBorder = true;
	}
	else if (m_xPosition + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		m_xPosition = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width - 1.f;
		m_HasReachedBorder = true;
	}
	else m_HasReachedBorder = false;
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
	m_LevelBoundaries.width += 1.f;
}

