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

	//����(� �߰�/����)
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

	std::unordered_map<std::string, class Texture*> mTextures; //�ҷ��� �ؽ�ó ��
	
	std::vector<class Actor*> mActors; //���� �� ��� ����
	std::vector<class Actor*> mPendingActors; //��� ���� ���� ����

	std::vector<class SpriteComponent*> mSprites; //�׷��� ��� ��������Ʈ ������Ʈ

	class Shader* mSpriteShader; //��������Ʈ ���̴�
	class VertexArray* mSpriteVerts; //��������Ʈ ���ؽ� �迭

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors; //���� ���͸� �����ϰ� �ִ��� ����

	//����
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;
};
