#pragma once

#include <cstdint>

class Component {
public:
	//생성자
	//낮은 업데이트 순서일수록 먼저 컴포넌트 갱신을 함
	Component(class Actor* owner, int updateOrder = 100);
	//소멸자
	virtual ~Component();
	//델타 시간에 따른 컴포넌트 갱신
	virtual void Update(float deltaTime);
	//컴포넌트의 입력 처리
	virtual void ProcessInput(const uint8_t* keyState) {}
	//세계 변환이 바뀔 때 호출
	virtual void OnUpdateWorldTransform() { }

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	//소유자 액터
	class Actor* mOwner;
	//컴포넌트의 갱신 순서
	int mUpdateOrder;
};
