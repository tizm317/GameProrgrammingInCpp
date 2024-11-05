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

	// ���� �е� ��ǥ
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	// �� �ӵ�
	Vector2 mBallVel;

	int mPaddleDir;
};