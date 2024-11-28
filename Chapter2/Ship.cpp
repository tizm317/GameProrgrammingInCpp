#include "Ship.h"
#include "AnimSpriteComponent.h"

Ship::Ship(Game* game)
	: Actor(game)
	, mRightSpeed(0.0f)
	, mDownSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims =
	{
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
	};
	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	// ��ġ ����
	if (pos.y <= 0.0f)
		pos.y = 0.0f;

	const float width = GetGame()->GetResolution().x;
	const float height = GetGame()->GetResolution().y;
	if (pos.y >= height)
		pos.y = height;

	if (pos.x <= 0.0f)
		pos.x = 0.0f;

	// ȭ�� ���ݱ���
	if (pos.x >= width / 2.0f)
		pos.x = width / 2.0f;


	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
	// �¿� �̵�
	const float rightSpeedValue = 250.0f;
	mRightSpeed = 0.0f;
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += rightSpeedValue;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= rightSpeedValue;
	}

	// ���� �̵�
	const float downSpeedValue = 300.0f;
	mDownSpeed = 0.0f;
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed -= downSpeedValue;
	}
	if (state[SDL_SCANCODE_S])
	{
		mDownSpeed += downSpeedValue;
	}
}
