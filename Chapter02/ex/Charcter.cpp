#include "Character.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Character::Character(Game* game) : Actor(game), mRightSpeed(0.f), mDownSpeed(0.f), mJump(false), mPunch(false) {
	asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Character01.png"), 
		game->GetTexture("Assets/Character02.png"), 
		game->GetTexture("Assets/Character03.png"), 
		game->GetTexture("Assets/Character04.png"), 
		game->GetTexture("Assets/Character05.png"), 
		game->GetTexture("Assets/Character06.png"), 
		game->GetTexture("Assets/Character07.png"), 
		game->GetTexture("Assets/Character08.png"), 
		game->GetTexture("Assets/Character09.png"), 
		game->GetTexture("Assets/Character10.png"), 
		game->GetTexture("Assets/Character11.png"), 
		game->GetTexture("Assets/Character12.png"), 
		game->GetTexture("Assets/Character13.png"), 
		game->GetTexture("Assets/Character14.png"), 
		game->GetTexture("Assets/Character15.png"), 
		game->GetTexture("Assets/Character16.png"), 
		game->GetTexture("Assets/Character17.png"), 
		game->GetTexture("Assets/Character18.png") 
	};
	asc->SetAnimTextures(anims);
}

void Character::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	if (pos.x < 25.f) pos.x = 25.f;
	else if (pos.x > 999.f) pos.x = 999.f;
	if (pos.y < 25.f) pos.y = 25.f;
	else if (pos.y > 743.f) pos.y = 743.f;
	SetPosition(pos);
	if (mRightSpeed == 0.f && mDownSpeed == 0.f) asc->SetAnimMove(false);
}

void Character::ProcessKeyboard(const uint8_t* state) {
	mRightSpeed = 0.f;
	mDownSpeed = 0.f;
	if (state[SDL_SCANCODE_D]) {
		mRightSpeed += 250.f;
		if (!asc->GetAnimJump() && !asc->GetAnimPunch()) asc->SetAnimMove(true);
	}
	if (state[SDL_SCANCODE_A]) {
		mRightSpeed -= 250.f;
		if (!asc->GetAnimJump() && !asc->GetAnimPunch()) asc->SetAnimMove(true);
	}
	if (state[SDL_SCANCODE_S]) {
		mDownSpeed += 300.f;
		if (!asc->GetAnimJump() && !asc->GetAnimPunch()) asc->SetAnimMove(true);
	}
	if (state[SDL_SCANCODE_W]) {
		mDownSpeed -= 300.f;
		if (!asc->GetAnimJump() && !asc->GetAnimPunch()) asc->SetAnimMove(true);
	}
	if (state[SDL_SCANCODE_Z]) {
		if (!asc->GetAnimJump() && !asc->GetAnimPunch()) asc->SetAnimJump(true);
	}
	if (state[SDL_SCANCODE_X]) {
		if (!asc->GetAnimPunch() && !asc->GetAnimJump()) asc->SetAnimPunch(true);
	}
}
