#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "SDL.h"

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

	class Texture* GetTexture(const std::string& fileName);

	//게임(운석 추가/제거)
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, class Texture*> mTextures; //불러온 텍스처 맵
	
	std::vector<class Actor*> mActors; //게임 안 모든 액터
	std::vector<class Actor*> mPendingActors; //모든 보류 중인 액터

	std::vector<class SpriteComponent*> mSprites; //그려질 모든 스프라이트 컴포넌트

	class Shader* mSpriteShader; //스프라이트 셰이더
	class VertexArray* mSpriteVerts; //스프라이트 버텍스 배열

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //지금 액터를 갱신하고 있는지 추적

	//게임
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;
};
