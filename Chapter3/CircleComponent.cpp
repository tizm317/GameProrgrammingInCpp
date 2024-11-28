#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner)
	: Component(owner)
	, mRadius(0.0f)
{
}

float CircleComponent::GetRadius() const 
{
	return mOwner->GetScale() * mRadius; 
}

const Vector2& CircleComponent::GetCenter() const
{
	// 소유자 액터 위치 반환
	return mOwner->GetPosition();
}

#include <iostream>
bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	//std::cout << "(" << a.GetCenter().x << "," << a.GetCenter().y << ") (";
	//std::cout << b.GetCenter().x << "," << b.GetCenter().y << ")" << '\n';

	// 중심점 사이 거리 제곱값 계산
	Vector2 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();
	//std::cout << "(" << diff.x << "," << diff.y << ") (";
	//std::cout << distSq << '\n';

	// 반지름 합의 제곱값 계산
	float radiusSumSq = a.GetRadius() + b.GetRadius();
	radiusSumSq *= radiusSumSq;

	//std::cout << radiusSumSq << '\n';

	return distSq <= radiusSumSq;
}