#include "Laser.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game) : Actor(game), mDeathTimer(1.f) {
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.f);
}

void Laser::UpdateActor(float deltaTime) {
	//레이저가 죽고 나서의 시간
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.f) SetState(EDead);
	else {
		//운석과 교차할 경우
		for (auto ast : GetGame()->GetAsteroid()) {
			if (Intersect(*mCircle, *(ast->GetCircle()))) {
				//이 레이저가 운석과 교차한다면 레이저와 운석은 죽었다고 설정
				SetState(EDead);
				ast->SetState(EDead);
				break;
			}
		}
	}
}
