#include "Actor.h"
#include "SpriteComponent.h"

//
const float Pi = 3.1415926535f;
float ToDegrees(float radians)
{
	return radians * 180.0f / Pi;
}

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexHeight(0)
	, mTexWidth(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect rect;
		rect.w = static_cast<int> (mTexWidth * mOwner->GetScale());
		rect.h = static_cast<int> (mTexHeight * mOwner->GetScale());

		// �»��
		rect.x = static_cast<int> (mOwner->GetPosition().x - rect.w / 2);
		rect.y = static_cast<int> (mOwner->GetPosition().y - rect.h / 2);

		// ��������Ʈ �׸���
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &rect, -ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// �ؽ�ó �ʺ� ���� ���
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}