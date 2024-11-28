#pragma once
#include "Component.h"
#include "Math.h"

class CircleComponent : public Component
{
public:
	CircleComponent(class Actor* owner);

	void SetRadius(float radius) { mRadius = radius; }
	float GetRadius() const;

	const Vector2& GetCenter() const;

private:
	// 반지름
	float mRadius;
};

// 충돌 체크 함수
bool Intersect(const CircleComponent& a, const CircleComponent& b);

