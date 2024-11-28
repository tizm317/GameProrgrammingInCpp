#pragma once

#include <vector>
#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Update(float deltaTime) override;

	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);

	// 애니메이션 FPS 얻거나 설정하는 함수
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	// 애니메이션에 사용되는 텍스쳐들
	std::vector<SDL_Texture*> mAnimTextures;

	// 현재 프레임
	float mCurrFrame;

	// 애니메이션 프레임 레이트
	float mAnimFPS;
};

