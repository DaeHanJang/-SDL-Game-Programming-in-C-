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

	class Renderer* GetRenderer() { return mRenderer; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors; //게임 안 모든 액터
	std::vector<class Actor*> mPendingActors; //모든 보류 중인 액터

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //지금 액터를 갱신하고 있는지 추적

	//게임
	class CameraActor* mCameraActor;
};
