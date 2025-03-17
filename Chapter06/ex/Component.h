#pragma once

#include <cstdint>

class Component {
public:
	//순위가 낮을수록 먼저 컴포넌트 갱신
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime); //델타 시간으로 컴포넌트 갱싱
	virtual void ProcessInput(const uint8_t* keyState) {} //컴포넌트의 입력 처리
	virtual void OnUpdateWorldTransform() {} //world transform 변화 시 호출

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; //소유자 액터
	int mUpdateOrder; //커포넌트의 갱신 순위
};
