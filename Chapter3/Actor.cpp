#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	: mState(State::EActive),
	mPosition(Vector2(0.0f, 0.0f)),
	mScale(1.0f),
	mRotation(0.0f),
	mGame(game) // 의존성 주입
{
	// 게임에 액터 추가
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);

	// Delete Components
	// ~Component 는 RemoveComponent 호출해서, 들고있는 목록에서만 빠짐
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
	// 모든 컴포넌트 Update
	for (auto c : mComponents)
		c->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime)
{
	// Actor 서브 클래스에서 필요시 재정의
}

void Actor::AddComponent(Component* component)
{
	// updateOrder 순 정렬 상태 유지
	const int order = component->GetUpdateOrder();

	// 이미 정렬된 상태에서 끼워넣기
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

Vector2 Actor::GetForward() const
{
	return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation));
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	// 활성화 여부 체크
	if (mState == State::EActive)
	{
		// 모든 컴포넌트의 ProcessInput 호출
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		// 커스텀 액터 인풋 함수 호출
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}
