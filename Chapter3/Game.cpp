#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "Math.h"

// �ػ�
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
		"Game Programming in C++ (Chapter 3)",
		100,
		30, //100,
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

	// SDL Image �ʱ�ȭ
	IMG_Init(IMG_INIT_PNG);

	// ������ �ε�
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
	// ������ ����
	SDL_DestroyWindow(mWindow);

	// ������ ����
	SDL_DestroyRenderer(mRenderer);

	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// ���� ���� ���̸� ��� ���Ϳ� �߰�
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
	// Ȱ��ȭ�� ����
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

	// ��� ���� ����
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
	// ���Ϸκ��� �ε�
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	// �ؽ��� ����
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
		// �ؽ�ó �ε�
		//return LoadTexture(textrueName.c_str());

		tex = LoadTexture(fileName.c_str());
		mTextures.insert({ fileName, tex });
	}

	return tex;
}

void Game::LoadData()
{
	// ���� ���� �� ��� ���� ���� ���

	// ���ּ�
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);

	// �
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
		new Asteroid(this);

	//AddAsteroid(new Asteroid(this));
	//mAsteroids.emplace_back(new Asteroid(this));


}

void Game::AddSprite(SpriteComponent* sprite)
{
	// ���� ���� ����
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

	// ��� ���Ϳ� ���� ProcessInput ȣ��
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
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

	// ��� ���� ����
	mUpdatingActors = true;
	for (auto actor : mActors)
		actor->Update(deltaTime);
	mUpdatingActors = false;

	// ��� ���� -> Ȱ�� ���� �̵�
	for (auto pendingActor : mPendingActors)
		mActors.push_back(pendingActor);
	// ��� ���� ����
	mPendingActors.clear();

	// ���� ���� ó��
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
	for (auto sp : mSprites)
		sp->Draw(mRenderer);

	// 3. ���� ����, �ĸ� ���� ��ȯ
	SDL_RenderPresent(mRenderer);
}