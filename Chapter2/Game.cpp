#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

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

	// SDL �ݱ�
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// ���� ���� ���̸� ��� ���Ϳ� �߰�
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
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
		}
	}
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;

	// 1. ���� �ؽ�ó �ʿ� �ִ��� Ȯ��
	auto it = mTextures.find(fileName);
	if (it != mTextures.end())
	{
		tex = it->second;
	}
	else
	{
		// 2. �ؽ�ó �ʿ� ���� ���, �ؽ�ó �ε� �� ����
		tex = LoadTexture(fileName.c_str());
		mTextures.insert({ fileName, tex });

		//return LoadTexture(textrueName.c_str()); 
	}

	return tex;
}

void Game::LoadData()
{
	// ���� ���� �� ��� ���� ���� ���

	// 1. �÷��̾� Ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, HEIGHT / 2.0f));
	mShip->SetScale(1.5f);

	// 2. ���

	// ��� ��� Actor ����
	Actor* actor = new Actor(this);
	actor->SetPosition(Vector2(WIDTH / 2, HEIGHT / 2));

	// ��� ������Ʈ 1
	BGSpriteComponent* bg = new BGSpriteComponent(actor);
	bg->SetScreenSize(Vector2(WIDTH, HEIGHT));
	std::vector<SDL_Texture*> bgTexs =
	{
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png"),
	};

	bg->SetBGTextures(bgTexs);
	bg->SetScrollSpeed(100.0f);

	// ��� ������Ʈ 2
		// drawOrder �ٸ��� ����
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
	// ���� ���� ����
	const int order = sprite->GetDrawOrder();
	auto it = mSprites.begin();
	for (; it != mSprites.end(); it++)
	{
		if (order < (*it)->GetDrawOrder())
		{
			break;
		}
	}

	// ��������Ʈ ���Ϳ� �ֱ�
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

	// �÷��̾� Ship �Է� ó��
	mShip->ProcessKeyboard(state);
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
		mActors.emplace_back(pendingActor);
	// ��� ���� ����
	mPendingActors.clear();

	// ���� ���� ó��
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
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

SDL_Texture* Game::LoadTexture(const char* fileName)
{
	// 1. ���Ϸκ��� �ε�
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	// 2. �ؽ��� ����
	SDL_Texture* text = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!text)
	{
		SDL_Log("Failed to convert surface to texture for %s", fileName);
		return nullptr;
	}

	return text;
}
