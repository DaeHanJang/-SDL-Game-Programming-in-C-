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

	std::vector<class Actor*> mActors; //���� �� ��� ����
	std::vector<class Actor*> mPendingActors; //��� ���� ���� ����

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //���� ���͸� �����ϰ� �ִ��� ����

	//����
	class CameraActor* mCameraActor;
};
