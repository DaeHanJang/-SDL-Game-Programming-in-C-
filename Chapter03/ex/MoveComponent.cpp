#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	: Component(owner, updateOrder), mAngularSpeed(0), mForwardSpeed(0), 
	mass(0.f), sumOfForces(Vector2::Zero), velocity(Vector2::Zero) {}

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
		if (pos.x < 0.f) pos.x = 1022.f;
		else if (pos.x > 1024.f) pos.x = 2.f;
		if (pos.y < 0.f) pos.y = 766.f;
		else if (pos.y > 768.f) pos.y = 2.f;

		mOwner->SetPosition(pos);
	}
	else {
		Vector2 acceleration = sumOfForces / mass;
		velocity += acceleration * deltaTime;
		Vector2 pos = mOwner->GetPosition();
		pos += velocity * deltaTime;

		if (pos.x < 0.f) pos.x = 1022.f;
		else if (pos.x > 1024.f) pos.x = 2.f;
		if (pos.y < 0.f) pos.y = 766.f;
		else if (pos.y > 768.f) pos.y = 2.f;

		mOwner->SetPosition(pos);
		SetSumOfForces(Vector2::Zero);
	}
}

void MoveComponent::AddForce(float force) {
	sumOfForces += mOwner->GetForward() * force;
}