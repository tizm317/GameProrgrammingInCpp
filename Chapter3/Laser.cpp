#include "Laser.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Laser::Laser(Game* game)
	: Actor(game)
	, mLifeTime(1.0f)
{
	// SpriteComponent 생성 및 텍스처 설정
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));

	// MoveComponent 생성 및 전방 이동 속도 설정
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(600.0f);

	// CircleComponent 생성 및 반지름 설정
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(10.0f);
}


void Laser::UpdateActor(float deltaTime)
{
	mLifeTime -= deltaTime;

	if (mLifeTime <= 0.0f)
	{
		SetState(State::EDead);
	}
	else
	{
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				SetState(State::EDead);
				ast->SetState(State::EDead);
				break;
			}
		}
	}

}


