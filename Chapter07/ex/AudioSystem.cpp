#include "AudioSystem.h"
#include "SDL_log.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	: mGame(game), mSystem(nullptr), mLowLevelSystem(nullptr) {}

AudioSystem::~AudioSystem() {}

bool AudioSystem::Initialize() {
	//����� �α� �ʱ�ȭ
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, //������ ��쿡�� �α׸� ����
		FMOD_DEBUG_MODE_TTY //stdout���� �α� ���
	);

	//FMOD ������ �ý��� �ν��Ͻ� ����
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK) {
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//FMOD ��Ʃ��� �ý��� �ʱ�ȭ
	result = mSystem->initialize(
		512, //���ÿ� ����� �� �ִ� ������ �ִ� ����
		FMOD_STUDIO_INIT_NORMAL, //�⺻ ���� ���
		FMOD_INIT_NORMAL, //�⺻ ����
		nullptr //��κ� nullptr
	);
	if (result != FMOD_OK) {
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	//������ �ý��� ������ ����
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	//������ ��ũ �ε�(Matster Bank.strings.bank�� ���� �ε�)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown() {
	//��� ��ũ ��ε�
	UnloadAllBanks();
	//FMOD �ý��� ����
	if (mSystem) mSystem->release();
}

void AudioSystem::LoadBank(const std::string& name) {
	//�� �� �ε����� �ʰ� ��
	if (mBanks.find(name) != mBanks.end()) return;

	//��ũ �ε�
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(), //��ũ�� ���� �̸�
		FMOD_STUDIO_LOAD_BANK_NORMAL, //�Ϲ����� ������� �ε�
		&bank //��ũ ������ ����
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK) {
		//��ũ�� �ʿ� �߰�
		mBanks.emplace(name, bank);
		//��Ʈ���� ������ �ƴ� ��� ���� �����͸� �ε�
		bank->loadSampleData();
		//��ũ�� �̺�Ʈ ���� ����
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0) {
			//��ũ���� �̺�Ʈ ��ũ���� ����Ʈ�� ����
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++) {
				FMOD::Studio::EventDescription* e = events[i];
				//�̺�Ʈ�� ��θ� ��´�(�� event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);
				//�̺�Ʈ�� �ʿ� �߰�
				mEvents.emplace(eventName, e);
			}
		}
		//��ũ�� ���� ���� ����
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0) {
			//��ũ���� ���� ����Ʈ�� ����
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++) {
				FMOD::Studio::Bus* bus = buses[i];
				//������ ��θ� ��´�(�� bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				//���� �ʿ� �߰�
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name) {
	//�ε���� �ʾҴٸ� ����
	auto iter = mBanks.find(name);
	if (iter == mBanks.end()) return;

	//���� ��ũ���� ��� �̺�Ʈ ���� �ʿ�
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0) {
		//��ũ���� �̺�Ʈ ��ũ���� ����
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//�̺�Ʈ ����Ʈ�� ����
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++) {
			FMOD::Studio::EventDescription* e = events[i];
			//�̺�Ʈ�� ��θ� ����
			e->getPath(eventName, 512, nullptr);
			//�̺�Ʈ ����
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end()) mEvents.erase(eventi);
		}
	}
	//��ũ���� ���� ���� ����
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0) {
		//��ũ���� ���� ����Ʈ ����
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++) {
			FMOD::Studio::Bus* bus = buses[i];
			//���� ��� ����(�� bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			//���� ����
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end()) mBuses.erase(busi);
		}
	}

	//���� �����Ϳ� ��ũ ��ε�
	bank->unloadSampleData();
	bank->unload();
	//��ũ �ʿ��� ����
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks() {
	for (auto& iter : mBanks) {
		//��ũ �ڽ��� ���� ������ ��ε�
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	//��ũ�� ���ٴ� ���� �̺�Ʈ�� ���ٴ� ��
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name) {
	unsigned int retID = 0;
	//�̺�Ʈ�� �����ϴ��� Ȯ��
	auto iter = mEvents.find(name);
	if (iter != mEvents.end()) {
		//�̺�Ʈ�� �ν��Ͻ��� ����
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event) {
			//�̺�Ʈ �ν��Ͻ��� ����
			event->start();
			//�� ���̵� ��� �ʿ� �߰�
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime) {
	//������ �̺�Ʈ�� ã��
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances) {
		FMOD::Studio::EventInstance* e = iter.second;
		//�̺�Ʈ�� ���� ���� ����
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
			//�̺�Ʈ�� �����ϰ� id�� done �ʿ� �߰�
			e->release();
			done.emplace_back(iter.first);
		}
	}
	
	//�Ϸ�� �̺�Ʈ �ν��Ͻ��� �ʿ��� ����
	for (auto id : done) mEventInstances.erase(id);

	//FMOD ����
	mSystem->update();
}

namespace {
	FMOD_VECTOR VecToFMOD(const Vector3& in) {
		//���� ��ǥ(+x ����, +y ������, +z ����)�� 
		//FMOD ��ǥ(+z ����, +x ������, +y ����)�� ��ȯ
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix) {
	//�� ����� ������� ���ؼ� �ʿ�� �ϴ� ���� ������ ���͸� ����
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	//��ġ�� ���� ����, ���� ���͸� ����
	listener.position = VecToFMOD(invView.GetTranslation());
	//�� ����� ����Ŀ��� �� ��° ���� ���� ����
	listener.forward = VecToFMOD(invView.GetZAxis());
	//�� ����� ����Ŀ��� �� ��° ���� ���� ����
	listener.up = VecToFMOD(invView.GetYAxis());
	//�ӵ��� 0���� ����(���÷� ȿ���� ����ҽ� ����)
	listener.velocity = {0.0f, 0.0f, 0.0f};
	//FMOD�� ����(0 = ������ �ϳ�)
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const {
	float retVal = 0.f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end()) iter->second->getVolume(&retVal);
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string & name) const {
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end()) iter->second->getPaused(&retVal);
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume) {
	auto iter = mBuses.find(name);
	if (iter != mBuses.end()) iter->second->setVolume(volume);
}

void AudioSystem::SetBusPaused(const std::string & name, bool pause) {
	auto iter = mBuses.find(name);
	if (iter != mBuses.end()) iter->second->setPaused(pause);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id) {
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end()) event = iter->second;
	return event;
}
