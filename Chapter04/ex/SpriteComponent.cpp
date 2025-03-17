#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0) {
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() { mOwner->GetGame()->RemoveSprite(this); }

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r;
		//소유자의 스케일값으로 너비/높이를 스케일
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		//소유자의 위치를 중심으로 사각형을 배치
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		//스프라이트를 그림
		SDL_RenderCopyEx(
			renderer, 
			mTexture, 
			nullptr, 
			&r, 
			-Math::ToDegrees(mOwner->GetRotation()), 
			nullptr, 
			SDL_FLIP_NONE
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight); //텍스처의 너비와 높이 얻음
}
