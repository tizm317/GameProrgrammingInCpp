#include "MoveComponent.h"
#include "Math.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	// 1. ȸ�� �ڵ�
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	// 2. �̵� �ڵ�
	if (!Math::NearZero(mForwardSpeed))
	{
		Vector2 pos = mOwner->GetPosition();

		// ȭ�� ���� �ڵ� (Only for Asteroids)
		const float SIZE = 100.0f;
		if (pos.x < 0.0f - SIZE)			pos.x = 1024.0f;
		else if (pos.x > 1024.0f + SIZE)	pos.x = 0.0f;

		if (pos.y < 0.0f - SIZE)			pos.y = 768.0f;
		else if (pos.y > 768.0f + SIZE)		pos.y = 0.0f;

		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}