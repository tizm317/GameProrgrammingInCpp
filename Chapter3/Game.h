#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "CircleComponent.h"


class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	// ���� ���� �Լ�
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// �̹��� ���� �Լ�
	SDL_Texture* LoadTexture(const char* fileName);
	SDL_Texture* GetTexture(const std::string& fileName);

	void LoadData();

	// Sprite ���� �Լ�
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*> GetAsteroids() { return mAsteroids; }

private:
	// ���� ������ ���� 3�ܰ� ���� �Լ�
	// �Է� ó�� / ���� / ��� ����
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL �� ������ ������
	SDL_Window* mWindow;

	// ������ ��� ����ž� �ϴ� ���� �Ǵ�
	bool mIsRunning;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	// ����
	std::vector<Actor*> mActors;			// Ȱ��
	std::vector<Actor*> mPendingActors;		// ��Ȱ��

	// ���� ���� ����
	bool mUpdatingActors;

	// �̹��� �� (����, SDL_Texture������)
	// ���� �ʿ� X
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Sprite
	std::vector<SpriteComponent*> mSprites;

	class Ship* mShip;

	std::vector<Asteroid*> mAsteroids;
};