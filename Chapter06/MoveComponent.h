#pragma once

#include "Component.h"

class MoveComponent : public Component {
public:
	//먼저 업데이트되도록 갱신 순서값을 낮춤
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
private:
	float mAngularSpeed;
	float mForwardSpeed;
};
