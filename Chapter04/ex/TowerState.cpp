#include "TowerState.h"
#include "SDL.h"
#include "Tower.h"
#include "MoveComponent.h"
#include "Bullet.h"

void TowerStatePatrol::Update(float deltaTime) { SDL_Log("Updating %s state", GetName()); }

void TowerStatePatrol::OnEnter() {
	SDL_Log("Entering %s state", GetName());
	mTower->GetMoveComponent()->SetAngularSpeed(Math::Pi);
}

void TowerStatePatrol::OnExit() {
	SDL_Log("Exiting %s state", GetName());
	mTower->GetMoveComponent()->SetAngularSpeed(0.f);
}

void TowerStateAttack::Update(float deltaTime) {
	SDL_Log("Updating %s state", GetName());
}

void TowerStateAttack::OnEnter() {
	SDL_Log("Entering %s state", GetName());
	//타워 위치부터 적 방향 총알 생성
	Bullet* b = new Bullet(mTower->GetGame());
	b->SetPosition(mTower->GetPosition());
	b->SetRotation(mTower->GetRotation());
}

void TowerStateAttack::OnExit() { SDL_Log("Exiting %s state", GetName()); }