#include "BGSpriteComponent.h"

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
		// 왼쪽으로 이동
		bg.mOffset.x -= mScrollSpeed * deltaTime;

		// 화면 범위 벗어나는 경우
		if (bg.mOffset.x < -mScreenSize.x)
		{
			// 마지막 배경 이미지 오른쪽으로 위치시킴
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1; // -1 안하면 경계선 보임 1픽셀
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

		// 좌상단 위치 계산 - offset 만큼 이동
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

		// 이전 배경 이미지의 오른쪽에 위치 시킴
		BGT.mOffset.x = count * mScreenSize.x;
		BGT.mOffset.y = 0;
		mBGTextures.push_back(BGT);

		count++;
	}
}
