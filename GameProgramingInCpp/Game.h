#pragma once

#include <SDL.h>
#include <vector>


struct Vector2
{
	float x;
	float y;

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

struct Ball
{
	Vector2 pos;
	Vector2 vel;
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


	// ��
	//Vector2 mBallPos;
	//Vector2 mBallVel;
	std::vector<Ball*> mBalls;

	// �е�
	Vector2 mPaddlePos;
	int mPaddleDir;

	Vector2 mPaddle2Pos;
	int mPaddle2Dir;
};