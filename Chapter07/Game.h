#pragma once

#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	//���Ӿ� ��� ����
	std::vector<class Actor*> mActors;
	//������� ����
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;

	Uint32 mTicksCount;
	bool mIsRunning;
	//���� ���͸� ������Ʈ�ϰ� �ִ��� ����
	bool mUpdatingActors;

	//���� ���� �ڵ�
	class CameraActor* mCameraActor;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;
};
