#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Actor.h"


class BGSpriteComponent : public SpriteComponent
{
public:
	// 생성자
		// 배경은 다른 이미지보다 보통 뒤에 위치하므로 10으로 작게 설정 (먼저 그림 = 뒤)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	// 배경 텍스처 설정
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	// 스크린 사이즈 설정
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }

	// 스크롤 속도 설정 / 얻기
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	// 배경 이미지와 오프셋 값을 캡슐화한 구조체
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};

	// 배경 텍스처
	std::vector<BGTexture> mBGTextures;

	// 배경 사이즈
	Vector2 mScreenSize;

	// 스크롤 속도
	float mScrollSpeed;
};

