#pragma once

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100); //������Ʈ �������� ���� ���� ������Ʈ�� �� ���� ����
	virtual ~Component();
	virtual void Update(float deltaTime); //��Ÿ �ð����� �� ������Ʈ�� ������Ʈ

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; //������ ����
	int mUpdateOrder; //������Ʈ�� ������Ʈ ����
};
