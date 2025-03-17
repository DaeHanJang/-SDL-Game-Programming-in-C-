#pragma once

#include "Component.h"
#include <string>
#include <unordered_map>

class AIComponent : public Component {
public:
	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);

	void RegisterState(class AIState* state); //새 상태를 맵에 추가
private:
	//상태의 이름과 AIState 인스턴스를 매핑
	std::unordered_map<std::string, class AIState*> mStateMap;
	class AIState* mCurrentState; //AI의 현재 상태
};
