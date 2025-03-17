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

	//이동 중이고 최근에 이동하지 않은 경우 발걸음 재생
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f) {
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	//액터에 따른 새로운 카메라 연산
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
	//wasd 움직임
	if (keys[SDL_SCANCODE_W]) forwardSpeed += 300.f;
	if (keys[SDL_SCANCODE_S]) forwardSpeed -= 300.f;
	if (keys[SDL_SCANCODE_A]) angularSpeed -= Math::TwoPi;
	if (keys[SDL_SCANCODE_D]) angularSpeed += Math::TwoPi;

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}

void CameraActor::SetFootstepSurface(float value) {
	//FMOD에서 매개변수를 설정하는 방식 때문에 여기서 일시 중지
	//변경한 발걸음 소리 재생
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}
