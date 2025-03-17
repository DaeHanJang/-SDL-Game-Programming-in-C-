#include "Actor.h"
#include "Game.h"
#include "Component.h"

Actor::Actor(Game* game) 
	: mState(EActive), mPosition(Vector2::Zero), mScale(1.f), mRotation(0.f), mGame(game) {
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);

	//컴포넌트들을 삭제해야 함, 소멸자에서 RemoveComponent을 콜하기 때문에 다른 스타일의 반복문이 필요함
	while (!mComponents.empty()) delete mComponents.back();
}

void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) comp->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::ProcessInput(const uint8_t* keyState) {
	if (mState == EActive) {
		for (auto comp : mComponents) comp->ProcessInput(keyState);
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState) {}

void Actor::AddComponent(Component* component) {
	//정렬된 벡터에서 삽입점을 찾음(나보다 순서가 높은 첫번째 요소)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) break;
	}
	mComponents.insert(iter, component); //반복자의 위치 앞으로 요소를 삽입
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) mComponents.erase(iter);
}
