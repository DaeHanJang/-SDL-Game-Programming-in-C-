#include "Asteroid.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game) : Actor(game), mCircle(nullptr) {
	//랜덤하게 위치와 방향을 초기화
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.f, 768.f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.f, Math::TwoPi));

	//스프라이트 컴포넌트를 생성하고 텍스처를 설정
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//이동 컴포넌트를 생성하고 전방 속도를 설정
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	//CircleComponent 생성(충돌 감지)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.f);

	game->AddAsteroid(this); //게임 mAsteroids에 추가
}

Asteroid::~Asteroid() { GetGame()->RemoveAsteroid(this); }
