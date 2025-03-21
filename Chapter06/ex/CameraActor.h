#pragma once

#include "Actor.h"

class CameraActor : public Actor {
public:
	CameraActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* key) override;
private:
	class MoveComponent* mMoveComp;
};
