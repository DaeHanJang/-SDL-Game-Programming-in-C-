#include "Ship.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "InputComponent.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* game) : Actor(game), mLaserCooldown(0.f) {
	//��������Ʈ ������Ʈ ����
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	//�Է� ������Ʈ ���� �� Ű/�ӵ� ����
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.f);
	ic->SetMass(1.5f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.f);
}

void Ship::UpdateActor(float deltaTime) {
	mLaserCooldown -= deltaTime;
	for (auto ast : GetGame()->GetAsteroid()) {
		if (Intersect(*mCircle, *(ast->GetCircle()))) {
			SetState(EDead);
			break;
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.f) {
		//�������� �����ϰ� �������� ��ġ�� ȸ������ ����
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = 0.5f; //�������� ��ٿ� �� �缳��(0.5��)
	}
}
