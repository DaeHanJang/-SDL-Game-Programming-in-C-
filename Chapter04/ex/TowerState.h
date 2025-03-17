#pragma once

#include "AIState.h"

class TowerStatePatrol : public AIState {
public:
	TowerStatePatrol(class AIComponent* owner, class Tower* tower) : AIState(owner), mTower(tower) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "TowerPatrol"; }
private:
	class Tower* mTower;
};

class TowerStateAttack : public AIState {
public:
	TowerStateAttack(class AIComponent* owner, class Tower* tower) : AIState(owner), mTower(tower) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "TowerAttack"; }
private:
	class Tower* mTower;
};
