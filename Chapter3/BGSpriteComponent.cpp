#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	//, mScreenSize({ 0.0f, 0.0f })
	, mScrollSpeed(0.0f)
{
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	for (auto& bg : mBGTextures)
	{
		bg.mOffset.x -= mScrollSpeed * deltaTime;
		if (bg.mOffset.x < -mScreenSize.x)
		{
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	for (auto& bg : mBGTextures)
	{
		SDL_Rect rect;
		rect.w = static_cast<int>(mScreenSize.x);
		rect.h = static_cast<int>(mScreenSize.y);

		rect.x = static_cast<int>(mOwner->GetPosition().x - rect.w / 2 + bg.mOffset.x);
		rect.y = static_cast<int>(mOwner->GetPosition().y - rect.h / 2 + bg.mOffset.y);
	
		SDL_RenderCopy(renderer, bg.mTexture, nullptr, &rect);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture BGT;
		BGT.mTexture = tex;

		BGT.mOffset.x = count * mScreenSize.x;
		BGT.mOffset.y = 0;
		mBGTextures.push_back(BGT);

		count++;
	}
}
