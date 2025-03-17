#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0) {
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() { mOwner->GetGame()->GetRenderer()->RemoveSprite(this); }

void SpriteComponent::Draw(Shader* shader) {
	if (mTexture) {
		//�ؽ����� �ʺ�/���̷� �簢�� ������
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);
		
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		
		//��� ��������Ʈ�� ���� ���̴�/���ؽ��� ����ϱ� ������
		//������ ���� ��������Ʈ�� �׸��� ���� Ȱ��ȭ
		
		//���� ��ȭ ����
		shader->SetMatrixUniform("uWorldTransform", world);
		//���� �ؽ�ó ����
		mTexture->SetActive();
		//�簢�� �׸���
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture) {
	mTexture = texture;
	//�ʺ�/���� ����
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
