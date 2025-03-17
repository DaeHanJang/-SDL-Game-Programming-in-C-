#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship(Game* game) : Actor(game), mRightSpeed(0.f), mDownSpeed(0.f) {
	//애니메이션 스프라이트 컴포넌트 생성
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),  
		game->GetTexture("Assets/Ship02.png"), 
		game->GetTexture("Assets/Ship03.png"), 
		game->GetTexture("Assets/Ship04.png")
	};
	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	//속도와 델타 시간을 기반으로 위치 갱신
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	//화면 왼쪽 반으로 위치 제한
	if (pos.x < 25.f) pos.x = 25.f;
	else if (pos.x > 500.f) pos.x = 500.f;
	if (pos.y < 25.f) pos.y = 25.f;
	else if (pos.y > 743.f) pos.y = 743.f;
	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state) {
	mRightSpeed = 0.f;
	mDownSpeed = 0.f;
	//우좌
	if (state[SDL_SCANCODE_D]) mRightSpeed += 250.f;
	if (state[SDL_SCANCODE_A]) mRightSpeed -= 250.f;
	//상하
	if (state[SDL_SCANCODE_S]) mDownSpeed += 300.f;
	if (state[SDL_SCANCODE_W]) mDownSpeed -= 300.f;
}
