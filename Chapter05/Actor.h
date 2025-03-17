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
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }

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
	Vector2 mPosition;
	float mScale;
	float mRotation;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
