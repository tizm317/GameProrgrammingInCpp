#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	: Actor(game)
	, mLaserCooldown(0.0f)
{
	// 1. 스프라이트 컴포넌트 생성 및 텍스처 설정
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// 2. InputComponent 생성
	InputComponent* ic = new InputComponent(this);

	// 키 설정
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);

	// 최대 속도 설정
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// 레이저 쿨 다운 초기화
		mLaserCooldown = 0.5f;
	}
}