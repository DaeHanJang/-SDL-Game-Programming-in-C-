#pragma once

#include "SDL.h"

//x/y��ǥ�� �����ϴ� ����ü
struct Vector2 {
	float x;
	float y;
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
	Vector2 mBallPos; //�� ��ġ
	Vector2 mBallVel; //�� �ӵ�
};
