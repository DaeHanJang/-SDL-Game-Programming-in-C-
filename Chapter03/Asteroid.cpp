#include "Asteroid.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game) : Actor(game), mCircle(nullptr) {
	//�����ϰ� ��ġ�� ������ �ʱ�ȭ
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.f, 768.f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.f, Math::TwoPi));

	//��������Ʈ ������Ʈ�� �����ϰ� �ؽ�ó�� ����
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//�̵� ������Ʈ�� �����ϰ� ���� �ӵ��� ����
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	//CircleComponent ����(�浹 ����)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.f);

	game->AddAsteroid(this); //���� mAsteroids�� �߰�
}

Asteroid::~Asteroid() { GetGame()->RemoveAsteroid(this); }
