#include "Asteroid.h"
#include "MoveComponent.h"
#include "Random.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "Game.h"

Asteroid::Asteroid(class Game* game)
	: Actor(game)
{
	// 랜덤하게 위치 , 방향 초기화
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 728.0f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);

	// 
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}
