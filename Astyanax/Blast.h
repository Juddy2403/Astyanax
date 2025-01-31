#pragma once
#include "GameObject.h"

class Texture;
class Blast final: public GameObject
{
private:
	bool m_IsTimeUp;
public:
	explicit Blast(const Point2f& pos);
	virtual void UpdateFrames(float elapsedSec) override;
	virtual void Draw() const override;
	bool IsTimeUp() const;
};

