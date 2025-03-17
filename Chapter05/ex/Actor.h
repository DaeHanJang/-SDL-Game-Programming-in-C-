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

	void Update(float deltaTime); //���ӿ��� ȣ���ϴ� ���� �Լ�(�������̵� �Ұ�)
	//���Ϳ� �پ��ִ� ��� ������Ʈ ����(�������̵� �Ұ�)
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime); //���ͺ� ���� �ڵ�(�������̵� ����)

	//���ӿ��� ȣ���ϴ� �Է�ó�� �Լ�(�������̵� �Ұ�)
	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState); //���ͺ� �Է� �ڵ�(�������̵� ����)

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

	//������Ʈ �߰�/����
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State mState; //������ ����

	//��ȯ
	Matrix4 mWorldTransform;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
