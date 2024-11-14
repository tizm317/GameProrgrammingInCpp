#pragma once


class Component
{
public:
	// 생성자 (순서 작을수록 빨리 갱신)
	Component(class Actor* owner = nullptr, int updateOrder = 100);

	// 소멸자
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }
	
protected:
	// 소유자 액터
	Actor* mOwner;

	// 컴포넌트 업데이트 순서
	int mUpdateOrder;
};