#include "Game.h"
#include "glew.h"
#include "Actor.h"
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"

#include "Texture.h"


Game::Game()
	: mWindow(nullptr), mSpriteShader(nullptr), mIsRunning(true), mUpdatingActors(false) {}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//OpenGL 속성 설정
	//코어 OpenGL 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//3.3 버전으로 설정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA 채널마다 8비트 크기인 색상 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //더블 버퍼링 활성화
	//OpenGL이 하드웨어 가속을 사용하도록 강제
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5)", 
		100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow); //OpenGL 콘텍스트 생성

	//GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed ot initialize GLEW.");
		return false;
	}
	glGetError(); //일부 플랫폼에서 GLEW은 에러 코드를 보냄 그러므로 에러값을 제거

	//셰이더를 생성/컴파일할 수 있는지 확인
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	CreateSpriteVerts(); //그려질 스프라이트를 위한 쿼드 생성

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

	mUpdatingActors = true;
	for (auto  actor : mActors) actor->ProcessInput(keyState);
	mUpdatingActors = false;
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	mTicksCount = SDL_GetTicks();

	//액터 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;
	//보류 중인 액터를 mActors로 이동
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
	for (auto actor : deadActors) delete actor; //죽은 액터 제거(mActors에서 제거됨)
}

void Game::GenerateOutput() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.f); //색상을 회색으로 설정
	glClear(GL_COLOR_BUFFER_BIT); //색상 버퍼 초기화

	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,  //srcFactor = srcAlpha
		GL_ONE_MINUS_SRC_ALPHA //dstFactor = 1 - srcAlpha
	);

	//스프라이트 셰이더와 버텍스 배열 객체를 활성화
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	//모든 스프라이트를 그림
	for (auto sprite : mSprites) sprite->Draw(mSpriteShader);

	SDL_GL_SwapWindow(mWindow); //버퍼를 스왑해서 장면을 출력
}

bool Game::LoadShaders() {
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) return false;

	mSpriteShader->SetActive();
	//뷰-투영 행렬 설정
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, //왼쪽 상단
		 0.5f,  0.5f, 0.f, 1.f, 0.f, //오른쪽 상단
		 0.5f, -0.5f, 0.f, 1.f, 1.f, //오른쪽 하단
		-0.5f, -0.5f, 0.f, 0.f, 1.f  //왼쪽 하단
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData() {
	mShip = new Ship(this); //플레리어 쉽 생성
	mShip->SetRotation(Math::PiOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++) new Asteroid(this);
}

void Game::UnloadData() {
	//액터 삭제
	//액터 소멸자에서 RemvoeActor을 호출하기 때문에 다른 스타일의 반복문 사용
	while (!mActors.empty()) delete mActors.back();

	//텍스처 삭제
	for (auto i : mTextures) {
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName) {
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) tex = iter->second;
	else {
		tex = new Texture();
		if (tex->Load(fileName)) mTextures.emplace(fileName, tex);
		else {
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Game::AddAsteroid(Asteroid* ast) { mAsteroids.emplace_back(ast); }

void Game::RemoveAsteroid(Asteroid* ast) {
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end()) mAsteroids.erase(iter);
}

void Game::Shutdown() {
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	//갱신해야 할 액터가 없다면 대기 액터에 추가함
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

void Game::AddSprite(SpriteComponent* sprite) {
	//정렬된 벡터에서 삽입 지점을 찾음(첫 앞 요소는 더 높은 우선순위를 갖음)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	mSprites.insert(iter, sprite); //반복자 위치 앞에 요소 삽입
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
