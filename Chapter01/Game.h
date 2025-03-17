#pragma once

#include "SDL.h"

//x/y좌표를 저장하는 구조체
struct Vector2 {
	float x;
	float y;
};

//게임 클래스
class Game {
public:
	Game();

	bool Initialize(); //게임 초기화
	void RunLoop(); //게임이 끝나기 전까지 게임 루프를 실행
	void Shutdown(); //게임 종료
private:
	//게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow; //SDL이 생성한 윈도우
	SDL_Renderer* mRenderer; //2D 렌더러
	Uint32 mTicksCount; //게임 시작부터 경과된 틱 수
	bool mIsRunning; //게임이 계속 실행돼야 하는지를 판단

	//퐁
	int mPaddleDir; //패들 방향
	Vector2 mPaddlePos; //패들 위치
	Vector2 mBallPos; //공 위치
	Vector2 mBallVel; //공 속도
};
