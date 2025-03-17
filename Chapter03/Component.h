#pragma once

#include <cstdint>

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100); //업데이트 순서값이 작을 수록 컴포넌트는 더 빨리 갱신
	virtual ~Component();
	virtual void Update(float deltaTime); //델타 시간으로 이 컴포넌트를 업데이트
	virtual void ProcessInput(const uint8_t* keyState) {} //이 컴포넌트를 위한 입력 처리

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; //소유자 액터
	int mUpdateOrder; //컴포넌트의 업데이트 순서
};
