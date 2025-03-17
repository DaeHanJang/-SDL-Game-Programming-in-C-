#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override; //�ִϸ��̼��� �����Ӹ��� ����(Comoponent�� Update �޼ҵ带 �������̵�)
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures); //�ִϸ��̼ǿ� ���Ǵ� �ؽ�ó ����
	void PlayAnim(int index);

	//�ִϸ��̼� FPS�� ��ų� �����ϴ� �Լ�
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	bool GetAnimMove() const { return mAnimMove; }
	void SetAnimMove(bool b) { mAnimMove = b; }
	bool GetAnimJump() const { return mAnimJump; }
	void SetAnimJump(bool b) { mAnimJump = b; }
	bool GetAnimPunch() const { return mAnimPunch; }
	void SetAnimPunch(bool b) { mAnimPunch = b; }
private:
	std::pair<std::vector<SDL_Texture*>, bool> mMoveTextures;
	std::pair<std::vector<SDL_Texture*>, bool> mJumpTextures;
	std::pair<std::vector<SDL_Texture*>, bool> mPunchTextures;
	float mCurrFrame; //���� ������
	float mAnimFPS; //�ִϸ��̼� ������ ����Ʈ
	bool mAnimMove;
	bool mAnimJump;
	bool mAnimPunch;
};
