#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game) : mState(EActive), mPosition(Vector3::Zero), mRotation(Quaternion::Identity), 
mScale(1.0f), mGame(game), mRecomputeWorldTransform(true) {
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);
	//컴포넌트 제거를 제거해야 함
	//컴포넌트 소멸자가 RemoveComponent 함수를 호출하기 때문에 다른 스타일의 반복문 필요
	while (!mComponents.empty()) delete mComponents.back();
}

void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (auto comp : mComponents) comp->Update(deltaTime);
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::ProcessInput(const uint8_t* keyState) {
	if (mState == EActive) {
		//컴포넌트에 대한 첫 번째 입력 처리
		for (auto comp : mComponents) comp->ProcessInput(keyState);

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState) {}

void Actor::ComputeWorldTransform() {
	if (mRecomputeWorldTransform) {
		mRecomputeWorldTransform = false;
		//스케일, 회전, 이동
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// Inform components world transform updated
		//컴포넌트 세계 변환 업데이트 알림
		for (auto comp : mComponents) comp->OnUpdateWorldTransform();
	}
}

void Actor::AddComponent(Component* component) {
	//정렬된 벡터에서 삽입 지점을 찾음
	//(나보다 순위가 높은 첫 번째 요소)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) break;
	}

	//반복자의 전 위치에 요소 삽입
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) mComponents.erase(iter);
}
