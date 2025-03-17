#pragma once

#include "Component.h"
#include "Math.h"

class MoveComponent : public Component {
public:
	MoveComponent(class Actor* owner, int updateOrder = 10); //먼저 업데이트되도록 갱신 순서값을 낮춤

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

	float GetMass() const { return mass; }
	Vector2 GetSumOfForces() const { return sumOfForces; }
	Vector2 GetVelocity() const { return velocity; }
	void SetMass(float _mass) { mass = _mass; }
	void SetSumOfForces(const Vector2& _sumOfForces) { sumOfForces = _sumOfForces; }
	void SetVelocity(const Vector2& _velocity) { velocity = _velocity; }

	void AddForce(float force);
private:
	float mAngularSpeed; //회전을 제어(초당 라디안)
	float mForwardSpeed; //전방 이동을 제어(초당 단위)

	float mass;
	Vector2 sumOfForces;
	Vector2 velocity;
};
