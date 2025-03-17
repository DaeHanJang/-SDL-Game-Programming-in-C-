#pragma once

#include "SDL.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"

class Game {
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	//게임
	class Grid* GetGrid() { return mGrid; }
	std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
	class Enemy* GetNearestEnemy(const Vector2& pos);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//텍스처들을 불러오는 맵
	std::unordered_map<std::string, SDL_Texture*> mTexture;

	std::vector<class Actor*> mActors; //게임안 액터들
	std::vector<class Actor*> mPendingActors; //대기중인 액터들
	std::vector<class SpriteComponent*> mSprites; //그려질 모든 스프라이트 컴포넌트들

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //액터 갱신 여부

	//게임
	std::vector<class Enemy*> mEnemies;
	class Grid* mGrid;
	float mNextenemy;
};
