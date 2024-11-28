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
	// ������
	float mRadius;
};

// �浹 üũ �Լ�
bool Intersect(const CircleComponent& a, const CircleComponent& b);

