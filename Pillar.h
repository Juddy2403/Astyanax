#pragma once
#include "Avatar.h"
#include "GameObject.h"

class Texture;
class PickUp;
class Pillar final : public GameObject
{
private:
	PickUp* m_pPickUp;
public:
	explicit Pillar(const Point2f& pos);
	virtual ~Pillar() override;
	virtual void Draw() const override;
	void Update(float elapsedSec, Avatar& avatar,const Level& level);
	void ManagePickUp(float elapsedSec,const Level& level,Avatar& avatar);
	void ManageDieing();
	void ManageTakingHit(float elapsedSec,const Avatar& avatar);
};

