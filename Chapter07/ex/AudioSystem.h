#pragma once

#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

//FMOD 헤더를 포함하지 않도록 전방 선언
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

	//뱅크 로드/언로드
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	//위치 오디오의 경우
	void SetListener(const Matrix4& viewMatrix);
	//버스 제어
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	//이벤트 인스턴스에 사용할 다음 ID를 추척
	static unsigned int sNextID;

	class Game* mGame;
	//로드된 뱅크를 관리하는 맵
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	//이벤트 이름과 EventDescription 맵
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	//이벤트 ID를 EventInstance로 매핑
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	//버스 맵
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	//FMOD 스튜디오 시스템
	FMOD::Studio::System* mSystem;
	//FMOD 저수준 시스템(거의 접근할 일없음)
	FMOD::System* mLowLevelSystem;
};
