#pragma once

#include <cstdint>

class Component {
public:
	//������ �������� ���� ������Ʈ ����
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime); //��Ÿ �ð����� ������Ʈ ����
	virtual void ProcessInput(const uint8_t* keyState) {} //������Ʈ�� �Է� ó��
	virtual void OnUpdateWorldTransform() {} //world transform ��ȭ �� ȣ��

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; //������ ����
	int mUpdateOrder; //Ŀ����Ʈ�� ���� ����
};
