#pragma once

#include "Game.h"
#include "Component.h"
#include "Math.h"

class Actor
{
public:
	// Actor ���� ������
	enum class State
	{
		EActive,
		EPaused,
		EDead
	};

	// ������
	Actor(class Game* game);

	// �Ҹ���
	virtual ~Actor();

	// Update �Լ���
	void Update(float deltaTime);				// Game ���� ȣ��
	void UpdateComponents(float deltaTime);		// ������ ��� ������Ʈ Update
	virtual void UpdateActor(float deltaTime);	// Ư�� ���Ϳ� Ưȭ�� Update

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


	// ������Ʈ �߰� / ����
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// ���� ����
	State mState;

	// Transform
	Vector2		mPosition;	// ���� �߽���
	float		mScale;		// ���� ���� (1.0f = 100%)
	float		mRotation;	// ȸ�� ���� (����)

	// ���Ͱ� ������ ������Ʈ��
	std::vector<Component*> mComponents;

	// Game ������ (�߰� ���� ���� �� Game �� �����ϱ� ����)
	Game* mGame;
};