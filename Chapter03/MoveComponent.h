#pragma once

#include "Component.h"

class MoveComponent : public Component {
public:
	MoveComponent(class Actor* owner, int updateOrder = 10); //먼저 업데이트되도록 갱신 순서값을 낮춤

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
private:
	float mAngularSpeed; //회전을 제어(초당 라디안)
	float mForwardSpeed; //전방 이동을 제어(초당 단위)
};
