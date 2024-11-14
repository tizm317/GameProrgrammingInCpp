#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "SpriteComponent.h"

struct Vector2
{
	float x;
	float y;

	// ������
	Vector2(float x_in = 0, float y_in = 0)
	{
		x = x_in;
		y = y_in;
	}

	Vector2 operator * (Vector2 v)
	{
		Vector2 ret = { this->x * v.x , this->y * v.y };
		return ret;
	}

	Vector2 operator * (float f)
	{
		Vector2 ret = { this->x * f , this->y * f };
		return ret;
	}
};

/*
struct Ball
{
	Vector2 pos;
	Vector2 vel;
};
*/

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

	/* ���� ---------------
	// ��
	//Vector2 mBallPos;
	//Vector2 mBallVel;
	std::vector<Ball*> mBalls;

	// �е�
	Vector2 mPaddlePos;
	int mPaddleDir;

	Vector2 mPaddle2Pos;
	int mPaddle2Dir;
	*/

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
};