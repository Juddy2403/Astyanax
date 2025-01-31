#pragma once
#include "Enemy.h"

class FloatingBlob final : public Enemy
{
private:
    float m_AccumFlyTime;
    const float m_FlyTime;
    void ManageFlyTime(float elapsedSec);
public:
    static int m_NrOfInstances;

    explicit FloatingBlob(const Point2f& pos, bool isGoingRight);
    virtual ~FloatingBlob() override;
    virtual void Update(float elapsedSec) override;
};

