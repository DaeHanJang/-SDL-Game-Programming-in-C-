#include "Actor.h"
#include "Game.h"
#include "Component.h"

Actor::Actor(Game* game)
	:mState(EActive), mPosition(Vector2::Zero), 
	mScale(1.0f), mRotation(0.0f), mGame(game), mRecomputeWorldTransform(true) {
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);
	//컴포넌트들의 삭제가 필요
	//소멸자에서 RomveComponent를 호출하기 때문에 다른 스타일의 반복문 필요
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
		//스케일, 회전, 이동 행렬순으로 결합해서 세계 변환 행렬 구함
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateRotationZ(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0.f));

		//컴포넌트에 세계 변환이 갱신됐다고 통지
		for (auto comp : mComponents) comp->OnUpdateWorldTransform();
	}
}

void Actor::AddComponent(Component* component) {
	//정렬된 벡터에서 삽입 지점을 찾음(첫 앞 요소는 더 높은 우선순위를 갖음)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) break;
	}

	mComponents.insert(iter, component); //반복자의 전 위치에 요소 삽입
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) mComponents.erase(iter);
}
