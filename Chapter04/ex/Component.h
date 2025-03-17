#pragma once

#include <cstdint>

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100); //������Ʈ �������� ���� ���� ������Ʈ�� �� ���� ����
	virtual ~Component();
	virtual void Update(float deltaTime); //��Ÿ �ð����� �� ������Ʈ�� ������Ʈ
	virtual void ProcessInput(const uint8_t* keyState) {} //�� ������Ʈ�� ���� �Է� ó��

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; //������ ����
	int mUpdateOrder; //������Ʈ�� ������Ʈ ����
};
