#pragma once

#include "Game.h"
#include "Component.h"
#include "Math.h"

class Actor
{
public:
	// Actor 상태 추적용
	enum class State
	{
		EActive,
		EPaused,
		EDead
	};

	// 생성자
	Actor(class Game* game);

	// 소멸자
	virtual ~Actor();

	// Update 함수들
	void Update(float deltaTime);				// Game 에서 호출
	void UpdateComponents(float deltaTime);		// 액터의 모든 컴포넌트 Update
	virtual void UpdateActor(float deltaTime);	// 특정 액터에 특화된 Update

	// Getters , Setters
	const State		GetState() const	{ return mState; }
	const Vector2	GetPosition() const { return mPosition; }
	const float		GetScale() const	{ return mScale; }
	const float		GetRotation() const { return mRotation; }

	Game* GetGame() const { return mGame; }

	void SetState(State state)					{ mState = state; }
	void SetPosition(const Vector2 position)	{ mPosition = position; }
	void SetScale(float scale)					{ mScale = scale; }
	void SetRotation(float rotation)			{ mRotation = rotation; }


	// 컴포넌트 추가 / 제거
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// 액터 상태
	State mState;

	// Transform
	Vector2		mPosition;	// 액터 중심점
	float		mScale;		// 액터 배율 (1.0f = 100%)
	float		mRotation;	// 회전 각도 (라디안)

	// 액터가 보유한 컴포넌트들
	std::vector<Component*> mComponents;

	// Game 포인터 (추가 액터 생성 등 Game 에 접근하기 위함)
	Game* mGame;
};