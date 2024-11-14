#pragma once

#include <vector>
#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Update(float deltaTime) override;

	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);

	// �ִϸ��̼� FPS ��ų� �����ϴ� �Լ�
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	// �ִϸ��̼ǿ� ���Ǵ� �ؽ��ĵ�
	std::vector<SDL_Texture*> mAnimTextures;

	// ���� ������
	float mCurrFrame;

	// �ִϸ��̼� ������ ����Ʈ
	float mAnimFPS;
};

