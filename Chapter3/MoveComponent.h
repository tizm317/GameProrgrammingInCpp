#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// 다른 컴포넌트 보다 먼저 업데이트
	MoveComponent(class Actor* owner, int updateOrder = 10);

	// 액터 이동 구현
	void Update(float deltaTime) override;

	// Speed getters / setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }

	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// 회전 제어 (초당 라디안)
	float mAngularSpeed;
	// 전방 이동 제어 (초당 단위)
	float mForwardSpeed;
};

