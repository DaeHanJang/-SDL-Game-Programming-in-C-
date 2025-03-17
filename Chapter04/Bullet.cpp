#include "Bullet.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Enemy.h"

Bullet::Bullet(class Game* game) : Actor(game) {
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(5.f);

	mLiveTime = 1.f;
}

void Bullet::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//적들과 출돌 검사
	for (Enemy* e : GetGame()->GetEnemies()) {
		if (Intersect(*mCircle, *(e->GetCircle()))) {
			//출돌로 둘다 죽음
			e->SetState(EDead);
			SetState(EDead);
			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.f)
		SetState(EDead); //생존 시간
}
