#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "MoveComponent.h"

Game::Game()
	: mRenderer(nullptr), mAudioSystem(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//렌더러 생성
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	//오디오 시스템 생성
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize()) {
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
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
			//키를 처음 누를 때 발생
			case SDL_KEYDOWN:
				if (!event.key.repeat) HandleKeyPress(event.key.keysym.sym);
				break;
			default: break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;

	for (auto actor : mActors) actor->ProcessInput(state);
}

void Game::HandleKeyPress(int key) {
	switch (key) {
		case '-': {
			//마스터 볼륨을 낮임
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = Math::Max(0.0f, volume - 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		case '=': {
			//마스터 볼륨을 높임
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = Math::Min(1.0f, volume + 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		case 'e':
			//폭발 소리 재생
			mAudioSystem->PlayEvent("event:/Explosion2D");
			break;
		case 'm':
			//음악 일시 정시 상태 토글
			mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
			break;
		case 'r':
			//리벌브 스냅샷 정지 또는 활성화
			if (!mReverbSnap.IsValid()) mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
			else mReverbSnap.Stop();
			break;
		case '1':
			//기본 표면 발걸음 설정
			mCameraActor->SetFootstepSurface(0.0f);
			break;
		case '2':
			//잔디 표면 발걸음 설정
			mCameraActor->SetFootstepSurface(0.5f);
			break;
		default:
			break;
	}
}

void Game::UpdateGame() {
	//델타 시간 계산
	//마지막 프레임 이후 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	//모든 액터 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;

	//대기 중인 액터 mActors에 이동
	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//죽은 액터를 임시 벡터에 추가
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}

	//죽은 액터 삭제(mActors에서도 제거 됨)
	for (auto actor : deadActors) delete actor;

	//오디오 시스템 갱신
	mAudioSystem->Update(deltaTime);
}

void Game::GenerateOutput() { mRenderer->Draw(); }

void Game::LoadData() {
	//액터 생성
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.f, 75.f, 0.f));
	a->SetScale(100.f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.f, -75.f, 0.f));
	a->SetScale(3.f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	//바닥 설정
	const float start = -1250.f;
	const float size = 250.0;
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
	//앞/뒤 벽
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

	//오디오 컴포넌트로 다양한 소리를 재생하는 구체 생성
	a = new Actor(this);
	a->SetPosition(Vector3(500.f, -75.f, 0.f));
	a->SetScale(1.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");
	MoveComponent* mvc = new MoveComponent(a);
	mvc->SetForwardSpeed(-100.f);


	//음악 재생
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
}

void Game::UnloadData() {
	//액터 삭제
	//액터 소멸자가 RemoveActor을 호출하기 때문에 다른 스타일의 반복문을 사용
	while (!mActors.empty()) delete mActors.back();

	if (mRenderer) mRenderer->UnloadData();
}

void Game::Shutdown() {
	UnloadData();
	if (mRenderer) mRenderer->Shutdown();
	if (mAudioSystem) mAudioSystem->Shutdown();
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	//액터 갱신 중이라면 대기 중인 액터에 추가
	if (mUpdatingActors) mPendingActors.emplace_back(actor);
	else mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//대기중인 액터에 있다면
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		//벡터 끝과 교환 후 뺌(복사본 삭제 방지)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//활성화된 액터에 있다면
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		//벡터 끝과 교환 후 뺌(복사본 삭제 방지)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
