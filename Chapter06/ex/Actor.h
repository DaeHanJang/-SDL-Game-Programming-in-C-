#pragma once

#include "Math.h"
#include <cstdint>
#include <vector>

class Actor {
public:
	enum State {
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime); //게임에서 호출하는 갱신 함수(오버라이드 불가)
	//액터에 붙어있는 모든 컴포넌트 갱신(오버라이드 불가)
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime); //액터별 갱신 코드(오버라이드 가능)

	//게임에서 호울하는 입력처리 함수(오버라이드 불가)
	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState); //액터별 입력 코드(오버라이드 가능)

	//Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	//컴포넌트 추가/제거
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State mState; //액터의 상태

	//변환
	Matrix4 mWorldTransform;
	Vector3 mPosition;
	Quaternion mRotation;
	float mScale;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
