#pragma once

#include <cstdint>

class Component {
public:
	//������
	//���� ������Ʈ �����ϼ��� ���� ������Ʈ ������ ��
	Component(class Actor* owner, int updateOrder = 100);
	//�Ҹ���
	virtual ~Component();
	//��Ÿ �ð��� ���� ������Ʈ ����
	virtual void Update(float deltaTime);
	//������Ʈ�� �Է� ó��
	virtual void ProcessInput(const uint8_t* keyState) {}
	//���� ��ȯ�� �ٲ� �� ȣ��
	virtual void OnUpdateWorldTransform() { }

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	//������ ����
	class Actor* mOwner;
	//������Ʈ�� ���� ����
	int mUpdateOrder;
};
