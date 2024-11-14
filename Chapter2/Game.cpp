#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

// 해상도
const float WIDTH = 1024.0f;
const float HEIGHT = 768.0f;

//const int thickness = 15;
//const int paddleH = thickness * 5;

std::vector<Vector2> dir = 
{
	{1.0f, -1.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{-1.0f, 1.0f},
	{-1.0f, 0.0f},
	{-1.0f, -1.0f}
};

Game::Game()
	: mWindow(nullptr),
	mIsRunning(true),
	mRenderer(nullptr),
	mTicksCount(0),
	//mPaddleDir(0),
	//mPaddle2Dir(0),
	mUpdatingActors(false)
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
		"Game Programming in C++ (Chapter 2)",
		100,
		//100,
		30,
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

	// SDL Image 초기화
	IMG_Init(IMG_INIT_PNG);

	// 데이터 로드
	LoadData();

	//// 공, 패들 초기화
	//mPaddlePos = { 10.0f , HEIGHT / 2 };
	//mPaddle2Pos = { WIDTH - 10.0f , HEIGHT / 2 };
	//for (int i = 0; i < 6; i++)
	//{
	//	Vector2 pos = { WIDTH / 2 , HEIGHT / 2 };
	//	//Vector2 vel = { -200.0f + i * 10, 235.0f + i * 10};
	//	Vector2 vel = Vector2{ 100.0f - 10 * i, 100.0f - 10 * i } * dir[i];
	//	//Vector2 vel = Vector2{ 100.0f, 150.0f } * dir[i];
	//	mBalls.push_back(new Ball{ pos, vel });
	//}


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

void Game::AddActor(Actor* actor)
{
	// 액터 갱신 중이면 대기 벡터에 추가
	if (mUpdatingActors)
	{
		mPendingActors.push_back(actor);
	}
	else
	{
		mActors.push_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// 활성화된 액터
	for (auto it = mActors.begin(); it != mActors.end(); it++)
	{
		if (*it == actor)
		{
			//mActors.erase(it);

			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(it, mActors.end() - 1);
			mActors.pop_back();
		}
	}

	// 대기 중인 액터
	for (auto it = mPendingActors.begin(); it != mPendingActors.end(); it++)
	{
		if (*it == actor)
		{
			// mPendingActors.erase(it);
			
			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(it, mPendingActors.end() - 1);
			mPendingActors.pop_back();
		}
	}
}

SDL_Texture* Game::LoadTexture(const char* fileName)
{
	// 파일로부터 로딩
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	// 텍스쳐 생성
	SDL_Texture* text = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!text)
	{
		SDL_Log("Failed to convert surface to texture for %s", fileName);
		return nullptr;
	}

	return text;
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto it = mTextures.find(fileName);
	if (it != mTextures.end())
	{
		tex = it->second;
	}
	else
	{
		// 텍스처 로드
		//return LoadTexture(textrueName.c_str());

		tex = LoadTexture(fileName.c_str());
		mTextures.insert({ fileName, tex });
	}

	return tex;
}

void Game::LoadData()
{
	// 게임 월드 내 모든 액터 생성 담당

	// 1. 플레이어 Ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// 2. 배경
	Actor* actor = new Actor(this);
	actor->SetPosition(Vector2(WIDTH / 2, HEIGHT / 2));

	BGSpriteComponent* bg = new BGSpriteComponent(actor);
	bg->SetScreenSize(Vector2(WIDTH, HEIGHT));
	std::vector<SDL_Texture*> bgTexs =
	{
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png"),
	};

	bg->SetBGTextures(bgTexs);
	bg->SetScrollSpeed(100.0f);

	bg = new BGSpriteComponent(actor, 50);
	bg->SetScreenSize(Vector2(WIDTH, HEIGHT));
	bgTexs =
	{
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
	};
	
	bg->SetBGTextures(bgTexs);
	bg->SetScrollSpeed(150.0f);
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// 정렬 상태 유지
	int order = sprite->GetDrawOrder();
	auto it = mSprites.begin();
	for (; it != mSprites.end(); it++)
	{
		if (order < (*it)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(it, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto it = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (it != mSprites.end())
		mSprites.erase(it);
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

	// 플레이어 Ship 
	mShip->ProcessKeyboard(state);

	/*
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
	*/
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

	// 모든 액터 갱신
	mUpdatingActors = true;
	for (auto actor : mActors)
		actor->Update(deltaTime);
	mUpdatingActors = false;

	// 대기 액터 -> 활성 액터 이동
	for (auto pendingActor : mPendingActors)
		mActors.push_back(pendingActor);
	// 대기 액터 비우기
	mPendingActors.clear();

	// 죽은 액터 처리
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.push_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}

	/* 이전 내용 --------------------
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
	//mBallPos.x += mBallVel.x * deltaTime;
	//mBallPos.y += mBallVel.y * deltaTime;
	for (auto ball : mBalls)
	{
		ball->pos.x += ball->vel.x * deltaTime;
		ball->pos.y += ball->vel.y * deltaTime;

		{
			// 상단
			if (ball->pos.y <= thickness && ball->vel.y < 0.0f)
			{
				ball->vel.y *= -1;
			}

			// 하단
			if (ball->pos.y >= HEIGHT - thickness && ball->vel.y > 0.0f)
			{
				ball->vel.y *= -1;
			}
		}

		{
			// 패들 1
			float diff = fabsf(ball->pos.y - mPaddlePos.y);
			if (diff <= paddleH / 2.0f &&						// y 차가 충분히 작고
				// 공이 패들과 나란히 (올바른 x값)
				ball->pos.x >= mPaddlePos.x - thickness / 2 &&
				ball->pos.x <= mPaddlePos.x + thickness / 2 &&
				ball->vel.x < 0.0f)								// 공이 왼쪽으로 이동
			{
				ball->vel.x *= -1.0f;
			}

			// 패들 2
			diff = fabsf(ball->pos.y - mPaddle2Pos.y);
			if (diff <= paddleH / 2.0f &&						// y 차가 충분히 작고
				// 공이 패들과 나란히 (올바른 x값)
				ball->pos.x >= mPaddle2Pos.x - thickness / 2 &&
				ball->pos.x <= mPaddle2Pos.x + thickness / 2 &&
				ball->vel.x > 0.0f)								// 공이 왼쪽으로 이동
			{		
				ball->vel.x *= -1.0f;
			}
		}
	}
	*/
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

	for (auto sp : mSprites)
		sp->Draw(mRenderer);

	/* 이전
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
	for (auto ball : mBalls)
	{
		SDL_Rect ball_rect
		{
			// 위치 보정
			static_cast<int>(ball->pos.x - thickness / 2),
			static_cast<int>(ball->pos.y - thickness / 2),
			thickness,
			thickness
		};

		SDL_RenderFillRect(mRenderer, &ball_rect);
	}

	//SDL_Rect ball
	//{
	//	// 위치 보정
	//	static_cast<int>(mBallPos.x - thickness / 2),
	//	static_cast<int>(mBallPos.y - thickness / 2),
	//	thickness,
	//	thickness
	//};

	//SDL_RenderFillRect(mRenderer, &ball);

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
		

		paddle.x = static_cast<int>(mPaddle2Pos.x - thickness / 2);
		paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
		SDL_RenderFillRect(mRenderer, &paddle);
	}
	*/

	// 3. 전면 버퍼, 후면 버퍼 교환
	SDL_RenderPresent(mRenderer);
}