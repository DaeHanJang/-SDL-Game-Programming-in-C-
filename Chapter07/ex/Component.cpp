#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder) : mOwner(owner), mUpdateOrder(updateOrder) {
	//액터의 컴포넌트 벡터에 추가
	mOwner->AddComponent(this);
}

Component::~Component() { mOwner->RemoveComponent(this); }

void Component::Update(float deltaTime) {}
