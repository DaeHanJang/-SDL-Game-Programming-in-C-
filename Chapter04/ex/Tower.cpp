#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AIComponent.h"
#include "TowerState.h"

Tower::Tower(class Game* game) : Actor(game) {
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));

	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);

	mNextAttack = AttackTime;

	mAI = new AIComponent(this);
	mAI->RegisterState(new TowerStatePatrol(mAI, this));
	mAI->RegisterState(new TowerStateAttack(mAI, this));
	mAI->ChangeState("TowerPatrol");
}

void Tower::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	mNextAttack -= deltaTime;
	if (mNextAttack <= 0.f) {
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		if (e != nullptr) {
			Vector2 dir = e->GetPosition() - GetPosition(); //나부터 적으로의 벡터
			float dist = dir.Length();
			if (dist < AttackRange) {
				SetRotation(Math::Atan2(-dir.y, dir.x)); //적 방향으로 회전값
				mAI->ChangeState("TowerAttack");
			}
			else mAI->ChangeState("TowerPatrol");
		}
		mNextAttack += AttackTime;
	}
}
