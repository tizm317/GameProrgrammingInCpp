#include "Actor.h"
#include <algorithm>

Actor::Actor(Game* game)
	: mState(State::EActive),
	mPosition(Vector2(0.0f, 0.0f)),
	mScale(1.0f),
	mRotation(0.0f),
	mGame(game) // ������ ����
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);

	// Delete Components
	// ~Component �� RemoveComponent ȣ���ؼ�, ����ִ� ��Ͽ����� ����
	while (!mComponents.empty())
		delete mComponents.back();
}

void Actor::Update(float deltaTime)
{
	if (mState == State::EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	// ��� ������Ʈ Update
	for (auto c : mComponents)
		c->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime)
{
	// Actor ���� Ŭ�������� �ʿ�� ������
}

void Actor::AddComponent(Component* component)
{
	// updateOrder �� ���� ���� ����
	const int order = component->GetUpdateOrder();

	// �̹� ���ĵ� ���¿��� �����ֱ�
	auto it = mComponents.begin();
	for (; it != mComponents.end(); it++)
	{
		if (order < (*it)->GetUpdateOrder())
			break;
	}

	mComponents.insert(it, component);
}

void Actor::RemoveComponent(Component* component)
{
	for(int i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i] == component)
		{
			mComponents.erase(mComponents.begin() + i);
			break;
		}
	}
}