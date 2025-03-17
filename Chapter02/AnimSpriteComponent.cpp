#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) 
	: SpriteComponent(owner, drawOrder), mCurrFrame(0.f), mAnimFPS(24.f) {}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		//������ ����Ʈ�� ��Ÿ �ð��� ������� ���� ������ ����
		mCurrFrame += mAnimFPS * deltaTime;
		//�ִϸ��̼� �ؽ�ó ���� �ʰ��ϸ� ���� �������� ����
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
