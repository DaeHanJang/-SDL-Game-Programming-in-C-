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
		//텍스터의 너비/높이로 사각형 스케일
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);
		
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();
		
		//모든 스프라이트가 같은 셰이더/버텍스를 사용하기 때문에
		//게임은 먼저 스프라이트를 그리기 전에 활성화
		
		//세계 변화 설정
		shader->SetMatrixUniform("uWorldTransform", world);
		//현제 텍스처 설정
		mTexture->SetActive();
		//사각형 그리기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture) {
	mTexture = texture;
	//너비/높이 설정
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
