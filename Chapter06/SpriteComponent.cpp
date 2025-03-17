#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0) {
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader) {
	if (mTexture) {
		//�ؽ�ó�� �ʺ�� ���̷� �簢���� ������
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.f);
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//��� ��������Ʈ�� ������ ���̴�/���ؽ��� ����ϱ� ������ 
		//������ ���� �׷��� ��������Ʈ�� Ȱ��ȭ�ؾ� ��
		shader->SetMatrixUniform("uWorldTransform", world); //���� ��ȯ ����� ����
		mTexture->SetActive(); //���� �ؽ�ó Ȱ��ȭ
		//�簢���� �׸�
		glDrawElements(
			GL_TRIANGLES, //�׷��� �� ������/������Ƽ���� Ÿ��
			6, //�ε��� ���ۿ� �ִ� �ε����� ��
			GL_UNSIGNED_INT, //�� �ε����� Ÿ��
			nullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture) {
	mTexture = texture;
	//�ʺ�/���� ����
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
