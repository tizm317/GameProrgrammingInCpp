#pragma once

#include <SDL.h>


struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

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


	// 공
	Vector2 mBallPos;
	Vector2 mBallVel;

	// 패들
	Vector2 mPaddlePos;
	int mPaddleDir;

	Vector2 mPaddle2Pos;
	int mPaddle2Dir;
};