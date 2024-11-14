#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

// �ػ�
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

	//// ��, �е� �ʱ�ȭ
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

	// 1. �÷��̾� Ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// 2. ���
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

	// �÷��̾� Ship 
	mShip->ProcessKeyboard(state);

	/*
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
	*/
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

	/* ���� ���� --------------------
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
	//mBallPos.x += mBallVel.x * deltaTime;
	//mBallPos.y += mBallVel.y * deltaTime;
	for (auto ball : mBalls)
	{
		ball->pos.x += ball->vel.x * deltaTime;
		ball->pos.y += ball->vel.y * deltaTime;

		{
			// ���
			if (ball->pos.y <= thickness && ball->vel.y < 0.0f)
			{
				ball->vel.y *= -1;
			}

			// �ϴ�
			if (ball->pos.y >= HEIGHT - thickness && ball->vel.y > 0.0f)
			{
				ball->vel.y *= -1;
			}
		}

		{
			// �е� 1
			float diff = fabsf(ball->pos.y - mPaddlePos.y);
			if (diff <= paddleH / 2.0f &&						// y ���� ����� �۰�
				// ���� �е�� ������ (�ùٸ� x��)
				ball->pos.x >= mPaddlePos.x - thickness / 2 &&
				ball->pos.x <= mPaddlePos.x + thickness / 2 &&
				ball->vel.x < 0.0f)								// ���� �������� �̵�
			{
				ball->vel.x *= -1.0f;
			}

			// �е� 2
			diff = fabsf(ball->pos.y - mPaddle2Pos.y);
			if (diff <= paddleH / 2.0f &&						// y ���� ����� �۰�
				// ���� �е�� ������ (�ùٸ� x��)
				ball->pos.x >= mPaddle2Pos.x - thickness / 2 &&
				ball->pos.x <= mPaddle2Pos.x + thickness / 2 &&
				ball->vel.x > 0.0f)								// ���� �������� �̵�
			{		
				ball->vel.x *= -1.0f;
			}
		}
	}
	*/
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

	/* ����
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
	for (auto ball : mBalls)
	{
		SDL_Rect ball_rect
		{
			// ��ġ ����
			static_cast<int>(ball->pos.x - thickness / 2),
			static_cast<int>(ball->pos.y - thickness / 2),
			thickness,
			thickness
		};

		SDL_RenderFillRect(mRenderer, &ball_rect);
	}

	//SDL_Rect ball
	//{
	//	// ��ġ ����
	//	static_cast<int>(mBallPos.x - thickness / 2),
	//	static_cast<int>(mBallPos.y - thickness / 2),
	//	thickness,
	//	thickness
	//};

	//SDL_RenderFillRect(mRenderer, &ball);

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
		

		paddle.x = static_cast<int>(mPaddle2Pos.x - thickness / 2);
		paddle.y = static_cast<int>(mPaddle2Pos.y - paddleH / 2);
		SDL_RenderFillRect(mRenderer, &paddle);
	}
	*/

	// 3. ���� ����, �ĸ� ���� ��ȯ
	SDL_RenderPresent(mRenderer);
}