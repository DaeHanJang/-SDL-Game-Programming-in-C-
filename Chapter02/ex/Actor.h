#pragma once

#include <vector>
#include "Math.h"

class Actor {
public:
	//액터의 상태를 추적하는데 사용
	enum State {
		EActive, 
		EPaused, 
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime); //Game에서 호출하는 Update 함수
	void UpdateComponents(float deltaTime); //액터에 부착된 모든 컴포넌트를 업데이트
	virtual void UpdateActor(float deltaTime); //특정 액터에 특화된 업데이트 코드

	//Getter/Setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	//컴포넌트 추가/제거
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State mState; //액터의 상태

	//변환
	Vector2 mPosition; //액터의 중심점
	float mScale; //액터의 배율(100%의 경우 1.f)
	float mRotation; //회전 각도(라디안)

	std::vector<class Component*> mComponents; //이 액터가 보유한 컴포넌트들
	class Game* mGame;
};
