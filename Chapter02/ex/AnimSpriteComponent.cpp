#include "AnimSpriteComponent.h"
#include "Math.h"
#include <vector>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder), mCurrFrame(0.f), mAnimFPS(24.f), 
	mAnimMove(false), mAnimJump(false), mAnimPunch(false) {}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimPunch) {
		if (GetAnimMove() && mCurrFrame != 0.f) {
			SetAnimMove(false);
			mCurrFrame = 0.f;
		}
		if (mPunchTextures.first.size() > 0) {
			mCurrFrame += mAnimFPS * deltaTime;
			while (mCurrFrame >= mPunchTextures.first.size()) {
				if (mPunchTextures.second) mCurrFrame -= mPunchTextures.first.size();
				else {
					mCurrFrame = mPunchTextures.first.size() - 1;
					SetAnimPunch(false);
				}
			}
			SetTexture(mPunchTextures.first[static_cast<int>(mCurrFrame)]);
		}
	}
	else if (mAnimJump) {
		if (GetAnimMove() && mCurrFrame != 0.f) {
			SetAnimMove(false);
			mCurrFrame = 0.f;
		}
		if (mJumpTextures.first.size() > 0) {
			mCurrFrame += mAnimFPS * deltaTime;
			while (mCurrFrame >= mJumpTextures.first.size()) {
				if (mJumpTextures.second) mCurrFrame -= mJumpTextures.first.size();
				else {
					mCurrFrame = mJumpTextures.first.size() - 1;
					SetAnimJump(false);
				}
			}
			SetTexture(mJumpTextures.first[static_cast<int>(mCurrFrame)]);
		}
	}
	else if (mAnimMove) {
		if (mMoveTextures.first.size() > 0) {
			mCurrFrame += mAnimFPS * deltaTime;
			while (mCurrFrame >= mMoveTextures.first.size()) {
				if (mMoveTextures.second) mCurrFrame -= mMoveTextures.first.size();
				else {
					mCurrFrame = mMoveTextures.first.size() - 1;
					SetAnimMove(false);
				}
			}
			SetTexture(mMoveTextures.first[static_cast<int>(mCurrFrame)]);
		}
	}
	else {
		mCurrFrame = 0.f;
		SetTexture(mPunchTextures.first[0]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
	mMoveTextures = make_pair(std::vector<SDL_Texture*>(textures.begin(), textures.begin() + 6), true);
	mJumpTextures = make_pair(std::vector<SDL_Texture*>(textures.begin() + 6, textures.begin() + 15), false);
	mPunchTextures = make_pair(std::vector<SDL_Texture*>(textures.begin() + 15, textures.end()), false);

	if (mMoveTextures.first.size() > 0) {
		mCurrFrame = 0.f;
		SetTexture(mPunchTextures.first[0]);
	}
}
