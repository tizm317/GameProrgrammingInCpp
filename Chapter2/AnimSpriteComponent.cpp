#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{

}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// 현재 프레임 갱신 : 프레임 레이트, 델타 시간 기반
		mCurrFrame += mAnimFPS * deltaTime;

		// 애니메이션 텍스처 수 초과하면 현재 프레임 래핑함
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		// 현재 텍스처를 설정
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	// 텍스처 벡터 설정
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0)
	{
		// 현재 프레임 0으로 초기화
		mCurrFrame = 0.0f;

		// 첫번째 프레임 설정
		SetTexture(mAnimTextures[0]);
	}
}
