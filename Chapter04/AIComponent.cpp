#include "AIComponent.h"
#include "AIState.h"
#include "SDL.h"

AIComponent::AIComponent(class Actor* owner) : Component(owner), mCurrentState(nullptr) {}

void AIComponent::Update(float deltaTime) {
	if (mCurrentState) mCurrentState->Update(deltaTime);
}

void AIComponent::ChangeState(const std::string& name) {
	//���� ���� ���¸� ���� ����
	if (mCurrentState) mCurrentState->OnExit();
	//�ʿ��� ���ο� ���¸� ã��
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end()) {
		mCurrentState = iter->second;
		mCurrentState->OnEnter(); //���ο� ���·� ����
	}
	else {
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state) {
	mStateMap.emplace(state->GetName(), state);
}
