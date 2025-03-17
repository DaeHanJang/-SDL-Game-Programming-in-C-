#include "CameraActor.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"

CameraActor::CameraActor(Game* game) : Actor(game) { mMoveComp = new MoveComponent(this); }

void CameraActor::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	//액터의 새 카메라 계산
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys) {
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	//wasd 움직임
	if (keys[SDL_SCANCODE_W]) forwardSpeed += 300.0f;
	if (keys[SDL_SCANCODE_S]) forwardSpeed -= 300.0f;
	if (keys[SDL_SCANCODE_A]) angularSpeed -= Math::TwoPi;
	if (keys[SDL_SCANCODE_D]) angularSpeed += Math::TwoPi;

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}
