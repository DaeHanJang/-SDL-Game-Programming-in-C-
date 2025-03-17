#pragma once

#include "AIState.h"

class EnemyStatePatrol : public AIState {
public:
	EnemyStatePatrol(class AIComponent* owner, class Enemy* enemy) : AIState(owner), mEnemy(enemy) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "EnemyPatrol"; }
private:
	class Enemy* mEnemy;
};

class EnemyStateDeath : public AIState {
public:
	EnemyStateDeath(class AIComponent* owner, class Enemy* enemy) : AIState(owner), mEnemy(enemy) {}

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override { return "EnemyDeath"; }
private:
	class Enemy* mEnemy;
};
