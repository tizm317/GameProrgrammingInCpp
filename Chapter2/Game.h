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

	// 생성자
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

	// 액터 관련 함수
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// 이미지 관련 함수
	SDL_Texture* LoadTexture(const char* fileName);
	SDL_Texture* GetTexture(const std::string& fileName);

	void LoadData();

	// Sprite 관련 함수
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

private:
	// 게임 루프를 위한 3단계 헬퍼 함수
	// 입력 처리 / 갱신 / 출력 생성
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL 이 생성한 윈도우
	SDL_Window* mWindow;

	// 게임이 계속 실행돼야 하는 지를 판단
	bool mIsRunning;

	SDL_Renderer* mRenderer;

	Uint32 mTicksCount;

	/* 이전 ---------------
	// 공
	//Vector2 mBallPos;
	//Vector2 mBallVel;
	std::vector<Ball*> mBalls;

	// 패들
	Vector2 mPaddlePos;
	int mPaddleDir;

	Vector2 mPaddle2Pos;
	int mPaddle2Dir;
	*/

	// 액터
	std::vector<Actor*> mActors;			// 활성
	std::vector<Actor*> mPendingActors;		// 비활성

	// 액터 갱신 여부
	bool mUpdatingActors;

	// 이미지 맵 (파일, SDL_Texture포인터)
	// 정렬 필요 X
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Sprite
	std::vector<SpriteComponent*> mSprites;

	class Ship* mShip;
};