#include "SpriteComponent.h"
#include "Actor.h"
#include "Math.h" // ToDegrees
#include "Game.h"

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

		// 좌상단
		rect.x = static_cast<int> (mOwner->GetPosition().x - rect.w / 2);
		rect.y = static_cast<int> (mOwner->GetPosition().y - rect.h / 2);

		// 스프라이트 그리기
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &rect, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	// 텍스처 너비 높이 얻기
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
