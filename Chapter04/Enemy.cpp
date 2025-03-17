#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "NavComponent.h"
#include "CircleComponent.h"

Enemy::Enemy(class Game* game) : Actor(game) {
	game->GetEnemies().emplace_back(this); //�� ���Ϳ� �߰�

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	//���� Ÿ�Ͽ� ��ġ ����
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	//�׺� ������Ʈ �� ���� Ÿ�� ���� 
	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(GetGame()->GetGrid()->GetStartTile());
	//���� �浹 ������Ʈ ����
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.f);
}

Enemy::~Enemy() {
	//�� ���ͷ� ���� ����
	auto iter = std::find(GetGame()->GetEnemies().begin(),
		GetGame()->GetEnemies().end(),
		this
	);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//��ǥ Ÿ�Ϸ� ����� ���� ��
	Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (Math::NearZero(diff.Length(), 10.f)) SetState(EDead);
}
