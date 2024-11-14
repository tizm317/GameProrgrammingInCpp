#include "Actor.h"
#include "Component.h"

Component::Component(Actor* owner, int updateOrder)
	: mOwner(owner),
	mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
	mOwner = nullptr;
}

void Component::Update(float deltaTime)
{
}
