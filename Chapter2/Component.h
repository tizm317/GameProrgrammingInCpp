#pragma once


class Component
{
public:
	// ������ (���� �������� ���� ����)
	Component(class Actor* owner = nullptr, int updateOrder = 100);

	// �Ҹ���
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }
	
protected:
	// ������ ����
	Actor* mOwner;

	// ������Ʈ ������Ʈ ����
	int mUpdateOrder;
};