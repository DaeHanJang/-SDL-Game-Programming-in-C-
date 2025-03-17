#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner) 
	: MoveComponent(owner), mForwardKey(0), mBackKey(0), mClockwiseKey(0), mCounterClockwiseKey(0) {

}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	//MoveComponent�� ���� ���� �ӵ� ���
	if (keyState[mForwardKey]) AddForce(mMaxForwardSpeed);
	if (keyState[mBackKey]) AddForce(-mMaxForwardSpeed);

	//MoveComponent�� ���� �� �ӵ� ���
	float angularSpeed = 0.f;
	if (keyState[mClockwiseKey]) angularSpeed += mMaxAngularSpeed;
	if (keyState[mCounterClockwiseKey]) angularSpeed -= mMaxAngularSpeed;
	SetAngularSpeed(angularSpeed);
}
