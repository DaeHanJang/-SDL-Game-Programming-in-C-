#include "EnemyState.h"
#include "SDL.h"
#include "NavComponent.h"
#include "Game.h"
#include "Grid.h"
#include "Tile.h"
#include "Enemy.h"
#include "Actor.h"

void EnemyStatePatrol::Update(float deltaTime) { SDL_Log("Updating %s state", GetName()); }

void EnemyStatePatrol::OnEnter() {
	SDL_Log("Entering %s state", GetName());
	//네비 컴포넌트 및 시작 타일 설정 
	NavComponent* nc = new NavComponent(mEnemy);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(mEnemy->GetGame()->GetGrid()->GetStartTile());
}

void EnemyStatePatrol::OnExit() { SDL_Log("Exiting %s state", GetName()); }

void EnemyStateDeath::Update(float deltaTime) { SDL_Log("Updating %s state", GetName()); }

void EnemyStateDeath::OnEnter() {
	SDL_Log("Entering %s state", GetName());
	mEnemy->SetState(Actor::EDead);
}

void EnemyStateDeath::OnExit() { SDL_Log("Exiting %s state", GetName()); }