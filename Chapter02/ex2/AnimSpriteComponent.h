#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override; //�ִϸ��̼��� �����Ӹ��� ����(Comoponent�� Update �޼ҵ带 �������̵�)
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures); //�ִϸ��̼ǿ� ���Ǵ� �ؽ�ó ����

	//�ִϸ��̼� FPS�� ��ų� �����ϴ� �Լ�
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	std::vector<SDL_Texture*> mAnimTextures; //�ִϸ��̼ǿ� ���Ǵ� ��� �ؽ�ó
	float mCurrFrame; //���� ������
	float mAnimFPS; //�ִϸ��̼� ������ ����Ʈ
};
