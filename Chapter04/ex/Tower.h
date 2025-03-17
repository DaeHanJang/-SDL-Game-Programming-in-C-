#pragma once

#include "Actor.h"

class Tower : public Actor {
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

	class MoveComponent* GetMoveComponent() const { return mMove; }
private:
	class MoveComponent* mMove;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.f;

	class AIComponent* mAI;
};
