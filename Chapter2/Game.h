#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <unordered_map>
#include <string>

#include "SpriteComponent.h"
#include "Math.h"

class Game
{
public:
	// ������
	Game();

	bool Initialize();	// �ʱ�ȭ
	void RunLoop();		// ���� ���� ����
	void Shutdown();	// ���� ����

	// ���� ���� �Լ�
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// �̹��� ���� �Լ�
	SDL_Texture* GetTexture(const std::string& fileName);

	// ���� ���� �Լ�
	void LoadData();

	// Sprite ���� �Լ�
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	// �ػ� ��� �Լ�
	Vector2 GetResolution() const { return Vector2(WIDTH, HEIGHT); }

private:
	// ���� ������ ���� 3�ܰ� ���� �Լ�
	void ProcessInput();	// �Է� ó��
	void UpdateGame();		// ����
	void GenerateOutput();	// ��� ����

	// GetTexture ���ο��� ����
	SDL_Texture* LoadTexture(const char* fileName);

	// SDL �� ������ ������
	SDL_Window* mWindow;

	// ������ ��� ����ž� �ϴ� ���� �Ǵ�
	bool mIsRunning;

	// ������ ������
	SDL_Renderer* mRenderer;

	// ���� ������ ƽ ��
	Uint32 mTicksCount;

	// ���� ������ ���� 2��
	std::vector<Actor*> mActors;			// Ȱ��
	std::vector<Actor*> mPendingActors;		// ��Ȱ��

	// ���� ���� ����
	bool mUpdatingActors;

	// �̹��� �� (����, SDL_Texture������)
	// ���� �ʿ� X
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Sprite ������Ʈ ����
	std::vector<SpriteComponent*> mSprites;

	// �÷��̾� Ship ������
	class Ship* mShip;

	// �ػ�
	const float WIDTH = 1024.0f;
	const float HEIGHT = 768.0f;
};