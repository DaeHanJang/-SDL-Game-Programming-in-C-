#pragma once

#include <vector>
#include "Math.h"

class Actor {
public:
	//������ ���¸� �����ϴµ� ���
	enum State {
		EActive, 
		EPaused, 
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime); //Game���� ȣ���ϴ� Update �Լ�
	void UpdateComponents(float deltaTime); //���Ϳ� ������ ��� ������Ʈ�� ������Ʈ
	virtual void UpdateActor(float deltaTime); //Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ�

	void ProcessInput(const uint8_t* keyState); //������ ȣ���ϴ� ProcessInput �Լ�(�������̵� �Ұ�)
	virtual void ActorInput(const uint8_t* keyState); //Ư�� ���͸� ���� �Է� �ڵ�(�������̵� ����)

	//Getter/Setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	//������Ʈ �߰�/����
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State mState; //������ ����

	//��ȯ
	Vector2 mPosition;
	float mScale;
	float mRotation;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
