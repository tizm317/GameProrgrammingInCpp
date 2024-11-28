#include "Actor.h"
#include "Component.h"

Component::Component(Actor* owner, int updateOrder)
	: mOwner(owner),
	mUpdateOrder(updateOrder)
{
	// ���� ���Ϳ� ������Ʈ �߰�
	mOwner->AddComponent(this);
}

Component::~Component()
{
	// ���� ���Ϳ��� ������Ʈ ����
	mOwner->RemoveComponent(this);
	mOwner = nullptr;
}

void Component::Update(float deltaTime)
{
}
