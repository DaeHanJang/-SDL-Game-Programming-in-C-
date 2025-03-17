#pragma once

#include "SDL.h"
#include <unordered_map>
#include <vector>
#include <string>

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
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroid() { return mAsteroids; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

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
	class Ship* mShip; //플레이어 쉽
	std::vector<class Asteroid*> mAsteroids;
};
