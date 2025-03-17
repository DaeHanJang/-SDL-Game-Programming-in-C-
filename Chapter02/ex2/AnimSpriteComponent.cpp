#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) 
	: SpriteComponent(owner, drawOrder), mCurrFrame(0.f), mAnimFPS(24.f) {}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		//프레임 레이트와 델타 시간을 기반으로 현재 프레임 갱신
		mCurrFrame += mAnimFPS * deltaTime;
		//애니메이션 텍스처 수를 초과하면 현재 프레임을 래핑
		while (mCurrFrame >= mAnimTextures.size()) mCurrFrame -= mAnimTextures.size();
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0) {
		mCurrFrame = 0.f;
		SetTexture(mAnimTextures[0]);
	}
}
