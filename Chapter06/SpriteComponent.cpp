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
		//텍스처의 너비와 높이로 사각형을 스케일
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.f);
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		//모든 스프라이트가 동일한 셰이더/버텍스를 사용하기 때문에 
		//게임은 먼저 그려질 스프라이트를 활성화해야 함
		shader->SetMatrixUniform("uWorldTransform", world); //세계 변환 행렬을 설정
		mTexture->SetActive(); //현재 텍스처 활성화
		//사각형을 그림
		glDrawElements(
			GL_TRIANGLES, //그려야 할 폴리곤/프리미티브의 타입
			6, //인덱스 버퍼에 있는 인덱스의 수
			GL_UNSIGNED_INT, //각 인덱스의 타입
			nullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture) {
	mTexture = texture;
	//너비/높이 설정
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
