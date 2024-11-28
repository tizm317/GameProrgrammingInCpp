#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Actor.h"


class BGSpriteComponent : public SpriteComponent
{
public:
	// ������
		// ����� �ٸ� �̹������� ���� �ڿ� ��ġ�ϹǷ� 10���� �۰� ���� (���� �׸� = ��)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	// ��� �ؽ�ó ����
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	// ��ũ�� ������ ����
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }

	// ��ũ�� �ӵ� ���� / ���
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	// ��� �̹����� ������ ���� ĸ��ȭ�� ����ü
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};

	// ��� �ؽ�ó
	std::vector<BGTexture> mBGTextures;

	// ��� ������
	Vector2 mScreenSize;

	// ��ũ�� �ӵ�
	float mScrollSpeed;
};

