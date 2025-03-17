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

	//����
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

	std::vector<class Actor*> mActors; //���Ӿ� ���͵�
	std::vector<class Actor*> mPendingActors; //������� ���͵�
	std::vector<class SpriteComponent*> mSprites; //�׷��� ��� ��������Ʈ ������Ʈ��

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //���� ���� ����

	//����
	class Ship* mShip; //�÷��̾� ��
	std::vector<class Asteroid*> mAsteroids;
};
