#include "CameraActor.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"

CameraActor::CameraActor(Game* game) :Actor(game) {
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);
}

void CameraActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//�̵� ���̰� �ֱٿ� �̵����� ���� ��� �߰��� ���
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f) {
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	//���Ϳ� ���� ���ο� ī�޶� ����
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
	GetGame()->GetAudioSystem()->SetListener(view);
}

void CameraActor::ActorInput(const uint8_t* keys) {
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;
	//wasd ������
	if (keys[SDL_SCANCODE_W]) forwardSpeed += 300.f;
	if (keys[SDL_SCANCODE_S]) forwardSpeed -= 300.f;
	if (keys[SDL_SCANCODE_A]) angularSpeed -= Math::TwoPi;
	if (keys[SDL_SCANCODE_D]) angularSpeed += Math::TwoPi;

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}

void CameraActor::SetFootstepSurface(float value) {
	//FMOD���� �Ű������� �����ϴ� ��� ������ ���⼭ �Ͻ� ����
	//������ �߰��� �Ҹ� ���
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}
