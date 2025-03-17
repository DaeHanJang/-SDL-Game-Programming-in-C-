#include "AIComponent.h"
#include "AIState.h"
#include "SDL.h"

AIComponent::AIComponent(class Actor* owner) : Component(owner), mCurrentState(nullptr) {}

void AIComponent::Update(float deltaTime) {
	if (mCurrentState) mCurrentState->Update(deltaTime);
}

void AIComponent::ChangeState(const std::string& name) {
	//먼저 현재 상태를 빠져 나옴
	if (mCurrentState) mCurrentState->OnExit();
	//맵에서 새로운 상태를 찾음
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end()) {
		mCurrentState = iter->second;
		mCurrentState->OnEnter(); //새로운 상태로 진입
	}
	else {
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state) {
	mStateMap.emplace(state->GetName(), state);
}
