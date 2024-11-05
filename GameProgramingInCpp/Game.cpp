#include "Game.h"

// 해상도
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
	// 1. SDL 라이브러리 초기화
		// 비디오 서브시스템 초기화
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		// SDL 라이브러리 초기화 실패
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	// 2. 윈도우 생성
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
		// 윈도우 생성 실패
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// 3. 렌더러 생성
	mRenderer = SDL_CreateRenderer
	(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		// 렌더러 생성 실패
		SDL_Log("Failed to creaet renderer : %s", SDL_GetError());
		return false;
	}

	// 공, 패들 초기화
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
	// 윈도우 해제
	SDL_DestroyWindow(mWindow);

	// 렌더러 해제
	SDL_DestroyRenderer(mRenderer);

	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	// 큐에 존재하는 모든 이벤트 조회
	while (SDL_PollEvent(&event))
	{
		// 이벤트에 대한 응답 처리
		switch (event.type)
		{
		// [X] 버튼 클릭 시 루프 종료
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// 키보드 상태 얻기
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	
	// Escape 키 누르면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// 패들 제어
	{
		// 패들 1 제어
		mPaddleDir = 0;
		if (state[SDL_SCANCODE_W])
		{
			mPaddleDir -= 1;
		}
		if (state[SDL_SCANCODE_S])
		{
			mPaddleDir += 1;
		}

		// 패들 2 제어
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
	// 마지막 프레임 이후로 16ms 가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// 델타 시간 = 현재 프레임 틱 값 - 이전 프레임 틱 값 // 초 단위로 변환
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// 틱 값 갱신
	mTicksCount = SDL_GetTicks();

	// 최대 델타 시간값으로 고정
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// 패들 위치 갱신
	{
		// 패들1 위치 갱신
		if (mPaddleDir != 0)
		{
			mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

			// 패들이 화면 영역 벗어나는지 검증
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

		// 패들2 위치 갱신
		if (mPaddle2Dir != 0)
		{
			mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;

			// 패들이 화면 영역 벗어나는지 검증
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
	

	// 공 위치 갱신
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// 공 충돌
	{
		// 상단
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		{
			mBallVel.y *= -1;
		}

		// 하단
		if (mBallPos.y >= HEIGHT - thickness && mBallVel.y > 0.0f)
		{
			mBallVel.y *= -1;
		}

		//// 우측
		//if (mBallPos.x >= WIDTH - thickness && mBallVel.x > 0.0f)
		//{
		//	mBallVel.x *= -1;
		//}
	}
	
	// 공과 패들 충돌
	{
		// 패들 1
		float diff = fabsf(mBallPos.y - mPaddlePos.y);
		if (diff <= paddleH / 2.0f &&						// y 차가 충분히 작고
			/*mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&*/
			// 공이 패들과 나란히 (올바른 x값)
			mBallPos.x >= mPaddlePos.x - thickness / 2 &&
			mBallPos.x <= mPaddlePos.x + thickness / 2 &&
			mBallVel.x < 0.0f)								// 공이 왼쪽으로 이동
		{
			mBallVel.x *= -1.0f;
		}

		// 패들 2
		diff = fabsf(mBallPos.y - mPaddle2Pos.y);
		if (diff <= paddleH / 2.0f &&						// y 차가 충분히 작고
			/*mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&*/
			// 공이 패들과 나란히 (올바른 x값)
			mBallPos.x >= mPaddle2Pos.x - thickness / 2 &&
			mBallPos.x <= mPaddle2Pos.x + thickness / 2 &&
			mBallVel.x > 0.0f)								// 공이 왼쪽으로 이동
		{
			mBallVel.x *= -1.0f;
		}
	}
}

void Game::GenerateOutput()
{
	// 색 지정 (Blue)
	SDL_SetRenderDrawColor
	(
		mRenderer,
		0,
		0,
		255,
		255
	);

	// 1. 후면 버퍼 지우기
	SDL_RenderClear(mRenderer);

	// 2. 전체 게임 장면 그리기

	// 벽 그리기
	{
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // 흰색으로 변경

		// 상단 벽
		SDL_Rect wall
		{
			0,
			0,
			1024,
			thickness
		};

		SDL_RenderFillRect(mRenderer, &wall);

		// 하단 벽
		wall.y = HEIGHT - thickness;
		SDL_RenderFillRect(mRenderer, &wall);

		//// 우측 벽
		//wall =
		//{
		//	int(WIDTH) - thickness,
		//	0,
		//	thickness,
		//	int(HEIGHT)
		//};
		//SDL_RenderFillRect(mRenderer, &wall);
	}
	

	// 공 그리기
	SDL_Rect ball
	{
		// 위치 보정
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// 패들 그리기
	{
		// 패들 1
		SDL_Rect paddle
		{
			// 위치 보정
			static_cast<int>(mPaddlePos.x - thickness / 2),
			static_cast<int>(mPaddlePos.y - paddleH / 2),
			thickness,
			paddleH // thickness * 5
		};
		SDL_RenderFillRect(mRenderer, &paddle);

		// 패들 2 
		/*
		SDL_Rect paddle2
		{
			// 위치 보정
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

	// 3. 전면 버퍼, 후면 버퍼 교환
	SDL_RenderPresent(mRenderer);
}