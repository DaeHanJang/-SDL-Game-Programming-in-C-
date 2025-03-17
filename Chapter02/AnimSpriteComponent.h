#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override; //애니메이션을 프레임마다 갱신(Comoponent의 Update 메소드를 오버라이드)
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures); //애니메이션에 사용되는 텍스처 설정

	//애니메이션 FPS를 얻거나 설정하는 함수
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	std::vector<SDL_Texture*> mAnimTextures; //애니메이션에 사용되는 모든 텍스처
	float mCurrFrame; //현재 프레임
	float mAnimFPS; //애니메이션 프레임 레이트
};
