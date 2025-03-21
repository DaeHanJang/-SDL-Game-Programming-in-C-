#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mAngularSpeed(0), mForwardSpeed(0) {}

void MoveComponent::Update(float deltaTime) {
	if (!Math::NearZero(mAngularSpeed)) {
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mForwardSpeed)) {
		Vector2 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		//asteroids만을 위한 screen wrapping 코드
		if (pos.x < -512.0f) { pos.x = 510.0f; }
		else if (pos.x > 512.0f) { pos.x = -510.0f; }
		if (pos.y < -384.0f) { pos.y = 382.0f; }
		else if (pos.y > 384.0f) { pos.y = -382.0f; }
		mOwner->SetPosition(pos);
	}
}
