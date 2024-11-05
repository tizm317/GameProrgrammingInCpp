#include "Game.h"

// �ػ�
const float WIDTH = 1024.0f;
const float HEIGHT = 768.0f;

const int thickness = 15;
const int paddleH = thickness * 5;

Game::Game()
	: mWindow(nullptr),
	mIsRunning(true),
	mRenderer(nullptr),
	mTicksCount(0),
	mPaddleDir(0),
	mPaddle2Dir(0)
{

}

bool Game::Initialize()
{
	// 1. SDL ���̺귯�� �ʱ�ȭ
		// ���� ����ý��� �ʱ�ȭ
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		// SDL ���̺귯�� �ʱ�ȭ ����
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	// 2. ������ ����
	mWindow = SDL_CreateWindow
	(
		"Game Programming in C++ (Chapter 1)",
		100,
		100,
		WIDTH,
		HEIGHT,
		0
	);

	if (!mWindow)
	{
		// ������ ���� ����
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// 3. ������ ����
	mRenderer = SDL_CreateRenderer
	(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		// ������ ���� ����
		SDL_Log("Failed to creaet renderer : %s", SDL_GetError());
		return false;
	}

	// ��, �е� �ʱ�ȭ
	mPaddlePos = { 10.0f , HEIGHT / 2 };
	mPaddle2Pos = { WIDTH - 10.0f , HEIGHT / 2 };
	mBallPos = { WIDTH / 2 , HEIGHT / 2 };
	mBallVel = { -200.0f, 235.0f };

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	// ������ ����
	SDL_DestroyWindow(mWindow);

	// ������ ����
	SDL_DestroyRenderer(mRenderer);

	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	// ť�� �����ϴ� ��� �̺�Ʈ ��ȸ
	while (SDL_PollEvent(&event))
	{
		// �̺�Ʈ�� ���� ���� ó��
		switch (event.type)
		{
		// [X] ��ư Ŭ�� �� ���� ����
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Ű���� ���� ���
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	
	// Escape Ű ������ ���� ����
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// �е� ����
	{
		// �е� 1 ����
		mPaddleDir = 0;
		if (state[SDL_SCANCODE_W])
		{
			mPaddleDir -= 1;
		}
		if (state[SDL_SCANCODE_S])
		{
			mPaddleDir += 1;
		}

		// �е� 2 ����
		mPaddle2Dir = 0;
		if (state[SDL_SCANCODE_I])
		{
			mPaddle2Dir -= 1;
		}
		if (state[SDL_SCANCODE_K])
		{
			mPaddle2Dir += 1;
		}
	}
}

void Game::UpdateGame()
{
	// ������ ������ ���ķ� 16ms �� ����� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// ��Ÿ �ð� = ���� ������ ƽ �� - ���� ������ ƽ �� // �� ������ ��ȯ
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// ƽ �� ����
	mTicksCount = SDL_GetTicks();

	// �ִ� ��Ÿ �ð������� ����
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// �е� ��ġ ����
	{
		// �е�1 ��ġ ����
		if (mPaddleDir != 0)
		{
			mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

			// �е��� ȭ�� ���� ������� ����
			const float tmp = paddleH / 2.0f + thickness;
			if (mPaddlePos.y < tmp)
			{
				mPaddlePos.y = tmp;
			}
			else if (mPaddlePos.y > HEIGHT - tmp)
			{
				mPaddlePos.y = HEIGHT - tmp;
			}
		}

		// �е�2 ��ġ ����
		if (mPaddle2Dir != 0)
		{
			mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;

			// �е��� ȭ�� ���� ������� ����
			const float tmp = paddleH / 2.0f + thickness;
			if (mPaddle2Pos.y < tmp)
			{
				mPaddle2Pos.y = tmp;
			}
			else if (mPaddle2Pos.y > HEIGHT - tmp)
			{
				mPaddle2Pos.y = HEIGHT - tmp;
			}
		}
	}
	

	// �� ��ġ ����
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// �� �浹
	{
		// ���
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1;
		}

		// �ϴ�
		if (mBallPos.y >= HEIGHT - thickness && mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1;
		}

		//// ����
		//if (mBallPos.x >= WIDTH - thickness && mBallVel.x > 0.0f)
		//{
		//	mBallVel.x *= -1;
		//}
	}
	
	// ���� �е� �浹
	{
		// �е� 1
		float diff = fabsf(mBallPos.y - mPaddlePos.y);
		if (diff <= paddleH / 2.0f &&						// y ���� ����� �۰�
			/*mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&*/
			// ���� �е�� ������ (�ùٸ� x��)
			mBallPos.x >= mPaddlePos.x - thickness / 2 &&
			mBallPos.x <= mPaddlePos.x + thickness / 2 &&
			mBallVel.x < 0.0f)								// ���� �������� �̵�
		{
			mBallVel.x *= -1.0f;
		}

		// �е� 2
		diff = fabsf(mBallPos.y - mPaddle2Pos.y);
		if (diff <= paddleH / 2.0f &&						// y ���� ����� �۰�
			/*mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&*/
			// ���� �е�� ������ (�ùٸ� x��)
			mBallPos.x >= mPaddle2Pos.x - thickness / 2 &&
			mBallPos.x <= mPaddle2Pos.x + thickness / 2 &&
			mBallVel.x > 0.0f)								// ���� �������� �̵�
		{
			mBallVel.x *= -1.0f;
		}
	}
}

void Game::GenerateOutput()
{
	// �� ���� (Blue)
	SDL_SetRenderDrawColor
	(
		mRenderer,
		0,
		0,
		255,
		255
	);

	// 1. �ĸ� ���� �����
	SDL_RenderClear(mRenderer);

	// 2. ��ü ���� ��� �׸���

	// �� �׸���
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // ������� ����

		// ��� ��
		SDL_Rect wall
		{
			0,
			0,
			1024,
			thickness
		};

		SDL_RenderFillRect(mRenderer, &wall);

		// �ϴ� ��
		wall.y = HEIGHT - thickness;
		SDL_RenderFillRect(mRenderer, &wall);

		//// ���� ��
		//wall =
		//{
		//	int(WIDTH) - thickness,
		//	0,
		//	thickness,
		//	int(HEIGHT)
		//};
		//SDL_RenderFillRect(mRenderer, &wall);
	}
	

	// �� �׸���
	SDL_Rect ball
	{
		// ��ġ ����
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// �е� �׸���
	{
		// �е� 1
		SDL_Rect paddle
		{
			// ��ġ ����
			static_cast<int>(mPaddlePos.x - thickness / 2),
			static_cast<int>(mPaddlePos.y - paddleH / 2),
			thickness,
			paddleH // thickness * 5
		};
		SDL_RenderFillRect(mRenderer, &paddle);

		// �е� 2 
		/*
		SDL_Rect paddle2
		{
			// ��ġ ����
			static_cast<int>(mPaddle2Pos.x - thickness / 2),
			static_cast<int>(mPaddle2Pos.y - paddleH / 2),
			thickness,
			paddleH // thickness * 5
		};
		*/

		paddle.x = static_cast<int>(mPaddle2Pos.x - thickness / 2);
		paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
		SDL_RenderFillRect(mRenderer, &paddle);
	}

	// 3. ���� ����, �ĸ� ���� ��ȯ
	SDL_RenderPresent(mRenderer);
}