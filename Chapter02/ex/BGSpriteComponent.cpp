#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder), mScrollSpeed(0.f) {}

void BGSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures) {
		bg.mOffset.x += mScrollSpeed * deltaTime; //x 오프셋 값을 갱신
		//이 텍스처가 화면을 완전히 벗어나면 오프셋 값을 마지막 배경 텍스처의 오른쪽 위치로 초기화
		if (bg.mOffset.x < -mScreenSize.x) bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	for (auto& bg : mBGTextures) {
		SDL_Rect r;
		//화면 크기 치수를 가정
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		//소유자의 위치를 중심으로 사각형을 배치
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);
		SDL_RenderCopy(renderer, bg.mTexture, nullptr, &r); //배경을 그림
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures) {
	int count = 0;
	for (auto tex : textures) {
		BGTexture temp;
		temp.mTexture = tex;
		//각 텍스처의 오프셋은 화면의 너비 * count
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}
