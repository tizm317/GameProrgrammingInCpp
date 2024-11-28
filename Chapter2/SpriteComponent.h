#pragma once
#include <SDL.h>
#include "Component.h"

class SpriteComponent : public Component
{
public:
	// 생성자/소멸자
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	// getters
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

protected:
	// 그릴 텍스쳐
	SDL_Texture* mTexture;

	// 그리기 순서 (작을수록 먼저 그림 = 뒤)
	int mDrawOrder;

	// 텍스쳐 너비/높이
	int mTexWidth;
	int mTexHeight;
};

