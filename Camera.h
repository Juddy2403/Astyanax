#pragma once

class Camera final
{
private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	float m_xPosition;
	bool m_IsMoving;
	bool m_HasReachedBorder;

	void Track(const Rectf& target, bool isMoving);
	void SetMovingState(bool isMoving);
	void Clamp();
public:
	explicit Camera(float width, float height);
	Camera& operator=(const Camera& rhs) = delete;
	Camera(const Camera& other) = delete;
	Camera& operator=(Camera&& rhs) = delete;
	Camera(Camera&& other) = delete;
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target, bool isMoving);
	Point2f GetCameraPos() const;
	Rectf GetWindow() const;
	bool IsMoving() const;

};

