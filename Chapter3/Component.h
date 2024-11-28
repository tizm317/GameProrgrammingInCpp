#pragma once
#include <cstdint> // uint8_t

class Component
{
public:
	// ������ (���� �������� ���� ����)
	Component(class Actor* owner = nullptr, int updateOrder = 100);

	// �Ҹ���
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }

	//
	virtual void ProcessInput(const uint8_t* keyState) {}
	
protected:
	// ������ ����
	Actor* mOwner;

	// ������Ʈ ������Ʈ ����
	int mUpdateOrder;
};