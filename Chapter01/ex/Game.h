#pragma once

#include <vector>
#include "SDL.h"

//x/y��ǥ�� �����ϴ� ����ü
struct Vector2 {
	float x;
	float y;
};

//�� ����ü
struct Ball {
	Vector2 mBallPos; //�� ��ġ
	Vector2 mBallVel; //�� �ӵ�
};

//���� Ŭ����
class Game {
public:
	Game();

	bool Initialize(); //���� �ʱ�ȭ
	void RunLoop(); //������ ������ ������ ���� ������ ����
	void Shutdown(); //���� ����
private:
	//���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow; //SDL�� ������ ������
	SDL_Renderer* mRenderer; //2D ������
	Uint32 mTicksCount; //���� ���ۺ��� ����� ƽ ��
	bool mIsRunning; //������ ��� ����ž� �ϴ����� �Ǵ�

	//��
	int mPaddleDir; //�е� ����
	Vector2 mPaddlePos; //�е� ��ġ
	int mPaddle2Dir; //�е�2 ����
	Vector2 mPaddle2Pos; //�е�2 ��ġ
	std::vector<Ball> mBall; //��
};
