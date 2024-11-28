#include "Actor.h"
#include "Component.h"

Component::Component(Actor* owner, int updateOrder)
	: mOwner(owner),
	mUpdateOrder(updateOrder)
{
	// 오너 액터에 컴포넌트 추가
	mOwner->AddComponent(this);
}

Component::~Component()
{
	// 오너 액터에서 컴포넌트 제거
	mOwner->RemoveComponent(this);
	mOwner = nullptr;
}

void Component::Update(float deltaTime)
{
}
