#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "NavComponent.h"
#include "CircleComponent.h"
#include "AIComponent.h"
#include "EnemyState.h"

Enemy::Enemy(class Game* game) : Actor(game) {
	game->GetEnemies().emplace_back(this); //�� ���Ϳ� �߰�

	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	//���� Ÿ�Ͽ� ��ġ ����
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	//���� �浹 ������Ʈ ����
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.f);

	mAI = new AIComponent(this);
	mAI->RegisterState(new EnemyStatePatrol(mAI, this));
	mAI->RegisterState(new EnemyStateDeath(mAI, this));
	mAI->ChangeState("EnemyPatrol");
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
	if (Math::NearZero(diff.Length(), 10.f)) mAI->ChangeState("EnemyDeath");
}
