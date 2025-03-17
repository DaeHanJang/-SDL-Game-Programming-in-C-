#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 768, 0);
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

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
	mShip->ProcessKeyboard(state); //쉽 입력 프로세스
}

void Game::UpdateGame() {
	//델타 시간 계산(마지막 프레임 이후로 16ms가 경과할 때까지 대기)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	//모든 액터를 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;
	//대기 중인 액터를 mActors로 이동
	for (auto pending : mPendingActors) mActors.emplace_back(pending);
	mPendingActors.clear();

	//죽은 액터를 임시 벡터에 추가
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}
	//죽은 액터 제거(mActors에서 추려낸 액터들)
	for (auto actor : deadActors) delete actor;
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	//모든 스프라이트 컴포넌트들 그리기
	for (auto sprite : mSprites) sprite->Draw(mRenderer);

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
	//플레이어 쉽 생성
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.f, 384.f));
	mShip->SetScale(1.5f);

	
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.f, 384.f));
	//더 먼 배경 액터 생성
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.f, 768.f));
	std::vector<SDL_Texture*> bgtexs = { GetTexture("Assets/Farback01.png"), GetTexture("Assets/Farback02.png") };
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.f);
	//가까운 배경 액터 생성
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.f, 768.f));
	bgtexs = { GetTexture("Assets/Stars.png"), GetTexture("Assets/Stars.png") };
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.f);
}

void Game::UnloadData() {
	//액터 제거
	//~Actor 함수가 RemoveActor를 호출하므로 다른 스타일의 루프를 사용
	while (!mActors.empty()) delete mActors.back();
	//텍스처 제거
	for (auto i : mTexture) SDL_DestroyTexture(i.second);
	mTexture.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	//맵에 이미 텍스처가 있는지 확인
	auto iter = mTexture.find(fileName);
	if (iter != mTexture.end()) tex = iter->second;
	else {
		//파일 불러오기
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName);
			return nullptr;
		}
		//surface로 부터 텍스처 생성
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
	//액터를 갱신 중이라면 대기 벡터에 액터를 추가
	if (mUpdatingActors) mPendingActors.emplace_back(actor);
	else mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//대기 액터
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		//벡터의 마지막으로 교환하고 뺌(복사본 제거 방지)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	//활성화된 액터
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		//벡터의 마지막으로 교환하고 뺌(복사본 제거 방지)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	//정렬된 벡터에서 삽입해야 할 위치를 찾음(자신보다 그리기 순서값이 큰 최초 요소)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}
	mSprites.insert(iter, sprite); //반복자 위치 앞에 요소를 삽입
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	//순서를 망치기 때문에 교환할 수 없음
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
