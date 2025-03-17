#pragma once

#include "MoveComponent.h"
#include <cstdint>

class  InputComponent : public MoveComponent {
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	//getter, setter
	float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
	float GetMaxAngularSpeed() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
private:
	//최대 전방, 각 속도
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	//전, 후진을 위한 키
	int mForwardKey;
	int mBackKey;
	//각 운동을 위한 키
	int mClockwiseKey;
	int mCounterClockwiseKey;
};
