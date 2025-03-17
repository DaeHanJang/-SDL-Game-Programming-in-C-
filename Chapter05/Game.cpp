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

	//OpenGL �Ӽ� ����
	//�ھ� OpenGL �������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//3.3 �������� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA ä�θ��� 8��Ʈ ũ���� ���� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //���� ���۸� Ȱ��ȭ
	//OpenGL�� �ϵ���� ������ ����ϵ��� ����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5)", 
		100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow); //OpenGL ���ؽ�Ʈ ����

	//GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed ot initialize GLEW.");
		return false;
	}
	glGetError(); //�Ϻ� �÷������� GLEW�� ���� �ڵ带 ���� �׷��Ƿ� �������� ����

	//���̴��� ����/�������� �� �ִ��� Ȯ��
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	CreateSpriteVerts(); //�׷��� ��������Ʈ�� ���� ���� ����

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

	//���� ����
	mUpdatingActors = true;
	for (auto actor : mActors) actor->Update(deltaTime);
	mUpdatingActors = false;
	//���� ���� ���͸� mActors�� �̵�
	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();
	//��� ���� ���͸� �ӽ� ���Ϳ� �߰�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}
	for (auto actor : deadActors) delete actor; //���� ���� ����(mActors���� ���ŵ�)
}

void Game::GenerateOutput() {
	glClearColor(0.86f, 0.86f, 0.86f, 1.f); //������ ȸ������ ����
	glClear(GL_COLOR_BUFFER_BIT); //���� ���� �ʱ�ȭ

	glEnable(GL_BLEND);
	glBlendFunc(
		GL_SRC_ALPHA,  //srcFactor = srcAlpha
		GL_ONE_MINUS_SRC_ALPHA //dstFactor = 1 - srcAlpha
	);

	//��������Ʈ ���̴��� ���ؽ� �迭 ��ü�� Ȱ��ȭ
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	//��� ��������Ʈ�� �׸�
	for (auto sprite : mSprites) sprite->Draw(mSpriteShader);

	SDL_GL_SwapWindow(mWindow); //���۸� �����ؼ� ����� ���
}

bool Game::LoadShaders() {
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) return false;

	mSpriteShader->SetActive();
	//��-���� ��� ����
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, //���� ���
		 0.5f,  0.5f, 0.f, 1.f, 0.f, //������ ���
		 0.5f, -0.5f, 0.f, 1.f, 1.f, //������ �ϴ�
		-0.5f, -0.5f, 0.f, 0.f, 1.f  //���� �ϴ�
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData() {
	mShip = new Ship(this); //�÷����� �� ����
	mShip->SetRotation(Math::PiOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++) new Asteroid(this);
}

void Game::UnloadData() {
	//���� ����
	//���� �Ҹ��ڿ��� RemvoeActor�� ȣ���ϱ� ������ �ٸ� ��Ÿ���� �ݺ��� ���
	while (!mActors.empty()) delete mActors.back();

	//�ؽ�ó ����
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
	//�����ؾ� �� ���Ͱ� ���ٸ� ��� ���Ϳ� �߰���
	if (mUpdatingActors) mPendingActors.emplace_back(actor);
	else mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//��� ���Ͱ� ���� ��
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		//������ ���� ��ȯ �� ��(����� ���� �������� ���� ����)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//Ȱ��ȭ�� ���Ͱ� ���� ��
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		//������ ���� ��ȯ �� ��(����� ���� �������� ���� ����)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	//���ĵ� ���Ϳ��� ���� ������ ã��(ù �� ��Ҵ� �� ���� �켱������ ����)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	mSprites.insert(iter, sprite); //�ݺ��� ��ġ �տ� ��� ����
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
