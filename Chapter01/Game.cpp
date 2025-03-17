#include "Game.h"

const int thickness = 15;
const int paddleH = 100.f;

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDir(0) {}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); //SDL �ʱ�ȭ
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//SDL ������ ����
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", //������ ����
		100, //�������� ���� ��� x��ǥ
		100, //�������� ���� ��� y��ǥ
		1024, //������ �ʺ�
		768, //������ ����
		0 //�÷���(0�� ��� �÷��׵� �������� ������ �ǹ�)
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//SDL ������ ����
	mRenderer = SDL_CreateRenderer(
		mWindow, //�������� ���� ������ ������
		-1, //�Ϲ������� -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddlePos.x = 10.f;
	mPaddlePos.y = 768.f / 2.f;
	mBallPos.x = 1024.f / 2.f;
	mBallPos.y = 768.f / 2.f;
	mBallVel.x = -200.f;
	mBallVel.y = 235.f;

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

	//ť�� ������ �̺�Ʈ�� ���� �ִ� ����
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			//���� ������ �̺�Ʈ�� �ٷ�
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL); //Ű������ ���� ���

	//�̽������� Ű�� �����ٸ� ���� ����
	if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;

	//W/SŰ�� ������� �е� ���� ����
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S]) mPaddleDir += 1;
}

void Game::UpdateGame() {
	//������ ������ ���ķ� 16ms�� ����� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ��(���� �ʴ����� ��ȯ)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;

	//�ִ� ��Ÿ �ð������� ����
	if (deltaTime > 0.05f) deltaTime = 0.05f;

	mTicksCount = SDL_GetTicks(); //���� �������� ���� ƽ���� ����

	//������ ������� �е� ��ġ ����
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 300.f * deltaTime;
		//�е��� ȭ�� ������ ������� ����
		if (mPaddlePos.y < (paddleH / 2.f + thickness)) mPaddlePos.y = paddleH / 2.f + thickness;
		else if (mPaddlePos.y > (768.f - paddleH / 2.f - thickness)) mPaddlePos.y = 768.f - paddleH / 2.f - thickness;
	}

	//�� �ӵ��� ������� �� ��ġ ����
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//���� �浹�� ���
	//�е�� ������ ���
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.f) ? diff : -diff; //���� ���� ����
	//���� ���� ����� �۰� �ùٸ� x��ǥ �ȿ� �ְ� ���� �������� �����δٸ�
	if (diff <= paddleH / 2.f && mBallPos.x >= 20.f && mBallPos.x <= 25.f && mBallVel.x < 0.f) mBallVel.x *= -1.f;
	//���� ��ũ�� ������ ���� ���(���� ����)
	else if (mBallPos.x <= 0.f) mIsRunning = false;
	//������ ���� �浹
	else if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.f) mBallVel.x *= -1.f;
	//���� ���� �浹
	if (mBallPos.y <= thickness && mBallVel.y < 0.f) mBallVel.y *= -1.f;
	//�Ʒ��� ���� �浹
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.f) mBallVel.y *= -1.f;
}

void Game::GenerateOutput() {
	//�Ķ����� �׸����� ����
	SDL_SetRenderDrawColor(
		mRenderer,
		0, //R
		0, //G
		255, //B
		255 //A
	);
	SDL_RenderClear(mRenderer); //���� ���� ���

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); //���� �׸���
	//�� �� �׸���
	SDL_Rect wall{
		0, //���� ��� x
		0, //���� ��� y
		1024, //�ʺ�
		thickness //����
	};
	SDL_RenderFillRect(mRenderer, &wall);
	//�Ʒ� �� �׸���
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	//������ �� �׸���
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 768;
	SDL_RenderFillRect(mRenderer, &wall);

	//�е� �׸���
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	//�� �׸���
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); //���� ���ۿ� �ĸ� ���� ��ü
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
