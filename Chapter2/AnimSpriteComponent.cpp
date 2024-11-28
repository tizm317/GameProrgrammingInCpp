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
		// ���� ������ ���� : ������ ����Ʈ, ��Ÿ �ð� ���
		mCurrFrame += mAnimFPS * deltaTime;

		// �ִϸ��̼� �ؽ�ó �� �ʰ��ϸ� ���� ������ ������
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		// ���� �ؽ�ó�� ����
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	// �ؽ�ó ���� ����
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0)
	{
		// ���� ������ 0���� �ʱ�ȭ
		mCurrFrame = 0.0f;

		// ù��° ������ ����
		SetTexture(mAnimTextures[0]);
	}
}
