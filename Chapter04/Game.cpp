#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "AIComponent.h"
#include "AIState.h"
#include "Grid.h"
#include "Enemy.h"

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 4)", 100, 100, 1024, 768, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
	if (keyState[SDL_SCANCODE_B]) mGrid->BuildTower();

	//���콺 ó��
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT) mGrid->ProcessClick(x, y);

	mUpdatingActors = true;
	for (auto actor : mActors) actor->ProcessInput(keyState);
	mUpdatingActors = false;
}

void Game::UpdateGame() {
	//��Ÿ �ð� ���(������ ������ ���ķ� 16ms�� ����� ������ ���)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	//��� ���͸� ����
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;
	//��� ���� ���͸� mActors�� �̵�
	for (auto pending : mPendingActors) mActors.emplace_back(pending);
	mPendingActors.clear();

	//���� ���͸� �ӽ� ���Ϳ� �߰�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}
	//���� ���� ����(mActors���� �߷��� ���͵�)
	for (auto actor : deadActors) delete actor;
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	SDL_RenderClear(mRenderer);

	//��� ��������Ʈ ������Ʈ�� �׸���
	for (auto sprite : mSprites) sprite->Draw(mRenderer);

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
	mGrid = new Grid(this);

	/*Actor* a = new Actor(this);
	AIComponent* aic = new AIComponent(a);
	aic->RegisterState(new AIPatrol(aic));
	aic->RegisterState(new AIDeath(aic));
	aic->RegisterState(new AIAttack(aic));
	aic->ChangeState("Patrol"); //���� ���� ����*/
}

void Game::UnloadData() {
	//���� ����
	//~Actor �Լ��� RemoveActor�� ȣ���ϹǷ� �ٸ� ��Ÿ���� ������ ���
	while (!mActors.empty()) delete mActors.back();
	//�ؽ�ó ����
	for (auto i : mTexture) SDL_DestroyTexture(i.second);
	mTexture.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	//�ʿ� �̹� �ؽ�ó�� �ִ��� Ȯ��
	auto iter = mTexture.find(fileName);
	if (iter != mTexture.end()) tex = iter->second;
	else {
		//���� �ҷ�����
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName);
			return nullptr;
		}
		//surface�� ���� �ؽ�ó ����
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName);
			return nullptr;
		}
	}
	return tex;
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	//���͸� ���� ���̶�� ��� ���Ϳ� ���͸� �߰�
	if (mUpdatingActors) mPendingActors.emplace_back(actor);
	else mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//��� ����
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		//������ ���������� ��ȯ�ϰ� ��(���纻 ���� ����)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	//Ȱ��ȭ�� ����
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		//������ ���������� ��ȯ�ϰ� ��(���纻 ���� ����)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	//���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã��(�ڽź��� �׸��� �������� ū ���� ���)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}
	mSprites.insert(iter, sprite); //�ݺ��� ��ġ �տ� ��Ҹ� ����
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	//������ ��ġ�� ������ ��ȯ�� �� ����
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

Enemy* Game::GetNearestEnemy(const Vector2& pos) {
	Enemy* best = nullptr;

	if (mEnemies.size() > 0) {
		best = mEnemies[0];
		float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++) {
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq) {
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}
	return best;
}
