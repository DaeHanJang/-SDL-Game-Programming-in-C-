#include "AIState.h"
#include "SDL.h"
#include "AIComponent.h"

void AIPatrol::Update(float deltaTime) {
	//일부 갱신 작업
	SDL_Log("Updating %s state", GetName());
	bool dead = true; //자신이 죽었는지 판단
	if (dead) {
		mOwner->ChangeState("Death"); //AI 컴포넌트에 상태를 변경하라고 알림
	}
}

void AIPatrol::OnEnter() { SDL_Log("Entering %s state", GetName()); }

void AIPatrol::OnExit() { SDL_Log("Exiting %s state", GetName()); }

void AIDeath::Update(float deltaTime) {SDL_Log("Updating %s state", GetName()); }

void AIDeath::OnEnter() { SDL_Log("Entering %s state", GetName()); }

void AIDeath::OnExit() { SDL_Log("Exiting %s state", GetName()); }

void AIAttack::Update(float deltaTime) { SDL_Log("Updating %s state", GetName()); }

void AIAttack::OnEnter() { SDL_Log("Entering %s state", GetName()); }

void AIAttack::OnExit() { SDL_Log("Exiting %s state", GetName()); }
