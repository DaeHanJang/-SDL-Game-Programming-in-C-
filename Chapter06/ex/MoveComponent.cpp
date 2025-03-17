#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mAngularSpeed(0.f), mForwardSpeed(0.f) {}

void MoveComponent::Update(float deltaTime) {
	if (!Math::NearZero(mAngularSpeed)) {
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		//회전 증가분을 위한 쿼터니언 생성(상향축을 기준으로 회전)
		Quaternion inc(Vector3::UnitZ, angle);
		rot = Quaternion::Concatenate(rot, inc); //이전 쿼터니언과 새 쿼터니언을 연결
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mForwardSpeed)) {
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
