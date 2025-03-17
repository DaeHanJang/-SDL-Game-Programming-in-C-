#include "Game.h"

const int thickness = 15;
const int paddleH = 100.f;

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDir(0) {}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); //SDL 초기화
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//SDL 윈도우 생성
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", //윈도우 제목
		100, //윈도우의 좌측 상단 x좌표
		100, //윈도우의 좌측 상단 y좌표
		1024, //윈도우 너비
		768, //윈도우 높이
		0 //플래그(0은 어떠한 플래그도 설정되지 않음을 의미)
	);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//SDL 렌더러 생성
	mRenderer = SDL_CreateRenderer(
		mWindow, //렌더링을 위해 생성한 윈도우
		-1, //일반적으로 -1
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

	//큐에 여전히 이벤트가 남아 있는 동안
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			//여러 유형의 이벤트를 다룸
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL); //키보드의 상태 얻기

	//이스케이프 키를 눌렀다면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;

	//W/S키를 기반으로 패들 방향 갱신
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S]) mPaddleDir += 1;
}

void Game::UpdateGame() {
	//마지막 프레임 이후로 16ms가 경과할 때까지 대기
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차(차를 초단위로 변환)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;

	//최대 델타 시간값으로 고정
	if (deltaTime > 0.05f) deltaTime = 0.05f;

	mTicksCount = SDL_GetTicks(); //다음 프레임을 위해 틱값을 갱신

	//방향을 기반으로 패들 위치 갱신
	if (mPaddleDir != 0) {
		mPaddlePos.y += mPaddleDir * 300.f * deltaTime;
		//패들이 화면 영역을 벗어나는지 검증
		if (mPaddlePos.y < (paddleH / 2.f + thickness)) mPaddlePos.y = paddleH / 2.f + thickness;
		else if (mPaddlePos.y > (768.f - paddleH / 2.f - thickness)) mPaddlePos.y = 768.f - paddleH / 2.f - thickness;
	}

	//공 속도를 기반으로 공 위치 갱신
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//공이 충돌할 경우
	//패들과 교차할 경우
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.f) ? diff : -diff; //차이 값의 절댓값
	//차이 값이 충분히 작고 올바른 x좌표 안에 있고 공이 왼쪽으로 움직인다면
	if (diff <= paddleH / 2.f && mBallPos.x >= 20.f && mBallPos.x <= 25.f && mBallVel.x < 0.f) mBallVel.x *= -1.f;
	//공이 스크린 밖으로 나갈 경우(게임 종료)
	else if (mBallPos.x <= 0.f) mIsRunning = false;
	//오른쪽 벽과 충돌
	else if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.f) mBallVel.x *= -1.f;
	//위쪽 벽과 충돌
	if (mBallPos.y <= thickness && mBallVel.y < 0.f) mBallVel.y *= -1.f;
	//아래쪽 벽과 충돌
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.f) mBallVel.y *= -1.f;
}

void Game::GenerateOutput() {
	//파란색을 그리도록 설정
	SDL_SetRenderDrawColor(
		mRenderer,
		0, //R
		0, //G
		255, //B
		255 //A
	);
	SDL_RenderClear(mRenderer); //색상 버퍼 비움

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); //벽들 그리기
	//위 벽 그리기
	SDL_Rect wall{
		0, //왼쪽 상단 x
		0, //왼쪽 상단 y
		1024, //너비
		thickness //높이
	};
	SDL_RenderFillRect(mRenderer, &wall);
	//아래 벽 그리기
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	//오른쪽 벽 그리기
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 768;
	SDL_RenderFillRect(mRenderer, &wall);

	//패들 그리기
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	//공 그리기
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer); //전면 버퍼와 후면 버퍼 교체
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
