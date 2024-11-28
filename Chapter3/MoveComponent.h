#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// �ٸ� ������Ʈ ���� ���� ������Ʈ
	MoveComponent(class Actor* owner, int updateOrder = 10);

	// ���� �̵� ����
	void Update(float deltaTime) override;

	// Speed getters / setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }

	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// ȸ�� ���� (�ʴ� ����)
	float mAngularSpeed;
	// ���� �̵� ���� (�ʴ� ����)
	float mForwardSpeed;
};

