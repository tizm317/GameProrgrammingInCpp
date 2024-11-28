#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <unordered_map>
#include <string>

#include "SpriteComponent.h"
#include "Math.h"

class Game
{
public:
	// 생성자
	Game();

	bool Initialize();	// 초기화
	void RunLoop();		// 게임 루프 실행
	void Shutdown();	// 게임 종료

	// 액터 관련 함수
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// 이미지 관련 함수
	SDL_Texture* GetTexture(const std::string& fileName);

	// 액터 생성 함수
	void LoadData();

	// Sprite 관련 함수
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	// 해상도 얻기 함수
	Vector2 GetResolution() const { return Vector2(WIDTH, HEIGHT); }

private:
	// 게임 루프를 위한 3단계 헬퍼 함수
	void ProcessInput();	// 입력 처리
	void UpdateGame();		// 갱신
	void GenerateOutput();	// 출력 생성

	// GetTexture 내부에서 실행
	SDL_Texture* LoadTexture(const char* fileName);

	// SDL 이 생성한 윈도우
	SDL_Window* mWindow;

	// 게임이 계속 실행돼야 하는 지를 판단
	bool mIsRunning;

	// 렌더러 포인터
	SDL_Renderer* mRenderer;

	// 이전 프레임 틱 값
	Uint32 mTicksCount;

	// 액터 포인터 벡터 2개
	std::vector<Actor*> mActors;			// 활성
	std::vector<Actor*> mPendingActors;		// 비활성

	// 액터 갱신 여부
	bool mUpdatingActors;

	// 이미지 맵 (파일, SDL_Texture포인터)
	// 정렬 필요 X
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// Sprite 컴포넌트 벡터
	std::vector<SpriteComponent*> mSprites;

	// 플레이어 Ship 포인터
	class Ship* mShip;

	// 해상도
	const float WIDTH = 1024.0f;
	const float HEIGHT = 768.0f;
};