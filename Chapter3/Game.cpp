#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "Math.h"

// 해상도
const float WIDTH = 1024.0f;
const float HEIGHT = 768.0f;

Game::Game()
	: mWindow(nullptr),
	mIsRunning(true),
	mRenderer(nullptr),
	mTicksCount(0),
	mUpdatingActors(false),
	mShip(nullptr)
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
		"Game Programming in C++ (Chapter 3)",
		100,
		30, //100,
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
			break; // <<
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
			break; // <<
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

	// 우주선
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);

	// 운석
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
		new Asteroid(this);

	//AddAsteroid(new Asteroid(this));
	//mAsteroids.emplace_back(new Asteroid(this));


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

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
		mAsteroids.erase(iter);
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

	// 모든 액터에 대한 ProcessInput 호출
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
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

	// 3. 전면 버퍼, 후면 버퍼 교환
	SDL_RenderPresent(mRenderer);
}