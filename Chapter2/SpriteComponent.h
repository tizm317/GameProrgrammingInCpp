#pragma once
#include <SDL.h>
#include "Component.h"

class SpriteComponent : public Component
{
public:
	// ������/�Ҹ���
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	// getters
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

protected:
	// �׸� �ؽ���
	SDL_Texture* mTexture;

	// �׸��� ���� (�������� ���� �׸� = ��)
	int mDrawOrder;

	// �ؽ��� �ʺ�/����
	int mTexWidth;
	int mTexHeight;
};

