#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "NavComponent.h"
#include "CircleComponent.h"

Enemy::Enemy(class Game* game) : Actor(game) {
	game->GetEnemies().emplace_back(this); //적 벡터에 추가

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	//시작 타일에 위치 설정
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	//네비 컴포넌트 및 시작 타일 설정 
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	//원형 충돌 컴포넌트 설정
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.f);
}

Enemy::~Enemy() {
	//적 벡터로 부터 제거
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(),
		this
	);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//목표 타일로 가까워 졌을 때
	Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (Math::NearZero(diff.Length(), 10.f)) SetState(EDead);
}
