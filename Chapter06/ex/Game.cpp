#include "Game.h"
#include "Renderer.h"
#include "glew.h"
#include "Actor.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "CameraActor.h"
#include "SpriteComponent.h"
#include "Texture.h"

Game::Game() : mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//렌더러 생성
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.f, 768.f)) {
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
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

	for (auto  actor : mActors) actor->ProcessInput(keyState);
}

void Game::UpdateGame() {
	//델타 시간 계산
	//마지막 프레임 이후 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	//모든 액터 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;
	//대기 액터를 mActors로 이동
	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();
	//모든 죽은 액터를 임시 벡터에 추가
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}

	//죽은 액터 제거(mActors에서도 제거됨)
	for (auto actor : deadActors) delete actor;
}

void Game::GenerateOutput() {
	mRenderer->Draw();
}

void Game::LoadData() {
	//액터 생성
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.f, 75.f, 0.f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a, 0);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.f, -75.f, 0.f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a, 1);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	//바닥 설정
	const float start = -1250.f;
	const float size = 250.f;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.f));
		}
	}

	//왼쪽/오른쪽 벽
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	//정방/후방 벽
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.f));
		a->SetRotation(q);
	}

	//광원 설정
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	for (int i = 0; i < 4; i++) {
		PointLight& dir2 = mRenderer->GetPointLight(i);
		dir2.mPosition = Vector3(0.f, 0.f, 0.2f);
		dir2.mDiffuseColor = Vector3(1.f, 0.f, 0.f);
		dir2.mSpecColor = Vector3(1.f, 0.f, 0.f);
		dir2.mSpecPower = Vector3(0.8f, 0.8f, 0.8f);
		dir2.mRange = 1000.f;
	}

	//카메라 액터
	mCameraActor = new CameraActor(this);

	//UI 요소
	a = new Actor(this);
	a->SetPosition(Vector3(-350.f, -350.f, 0.f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3(375.f, -275.f, 0.f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
}

void Game::UnloadData() {
	//액터 제거
	//액터 소멸자에서 RemvoeActor을 호출하기 때문에 다른 스타일의 반복문 사용
	while (!mActors.empty()) delete mActors.back();

	if (mRenderer) mRenderer->UnloadData();
}

void Game::Shutdown() {
	UnloadData();
	if (mRenderer) mRenderer->Shutdown();
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	//갱신해야 할 액터가 없다면 대기 액터에 추가
	if (mUpdatingActors) mPendingActors.emplace_back(actor);
	else mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//대기 액터가 있을 때
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		//벡터의 끝과 교환 후 뺌(복사된 것이 지워지는 것을 피함)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//활성화된 액터가 있을 때
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		//벡터의 끝과 교환 후 뺌(복사된 것이 지워지는 것을 피함)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
