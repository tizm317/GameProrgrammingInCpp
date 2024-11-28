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
	// SpriteComponent ���� �� �ؽ�ó ����
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));

	// MoveComponent ���� �� ���� �̵� �ӵ� ����
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(600.0f);

	// CircleComponent ���� �� ������ ����
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


