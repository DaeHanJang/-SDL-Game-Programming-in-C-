#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner) 
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), mClockwiseKey(0), mCounterClockwiseKey(0) {

}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	//MoveComponent를 위한 전방 속도 계산
	if (keyState[mForwardKey]) AddForce(mMaxForwardSpeed);
	if (keyState[mBackKey]) AddForce(-mMaxForwardSpeed);

	//MoveComponent를 위한 각 속도 계산
	float angularSpeed = 0.f;
	if (keyState[mClockwiseKey]) angularSpeed += mMaxAngularSpeed;
	if (keyState[mCounterClockwiseKey]) angularSpeed -= mMaxAngularSpeed;
	SetAngularSpeed(angularSpeed);
}
