#pragma once

#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

//FMOD ����� �������� �ʵ��� ���� ����
namespace FMOD {
	class System;

	namespace Studio {
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem {
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();

	//��ũ �ε�/��ε�
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	//��ġ ������� ���
	void SetListener(const Matrix4& viewMatrix);
	//���� ����
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	//�̺�Ʈ �ν��Ͻ��� ����� ���� ID�� ��ô
	static unsigned int sNextID;

	class Game* mGame;
	//�ε�� ��ũ�� �����ϴ� ��
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//�̺�Ʈ �̸��� EventDescription ��
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	//�̺�Ʈ ID�� EventInstance�� ����
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	//���� ��
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	//FMOD ��Ʃ��� �ý���
	FMOD::Studio::System* mSystem;
	//FMOD ������ �ý���(���� ������ �Ͼ���)
	FMOD::System* mLowLevelSystem;
};
