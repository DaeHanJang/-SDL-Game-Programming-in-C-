#pragma once

#include "Component.h"
#include <string>
#include <unordered_map>

class AIComponent : public Component {
public:
	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	void RegisterState(class AIState* state); //�� ���¸� �ʿ� �߰�
private:
	//������ �̸��� AIState �ν��Ͻ��� ����
	std::unordered_map<std::string, class AIState*> mStateMap;
	class AIState* mCurrentState; //AI�� ���� ����
};
