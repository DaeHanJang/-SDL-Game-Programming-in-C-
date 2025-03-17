#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner) : Component(owner), mMesh(nullptr), mTextureIndex(0) {
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() { mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this); }

void MeshComponent::Draw(Shader* shader) {
	if (mMesh) {
		//월드 세계 변화 설정
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		//정반사 지수 설정
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//활성화 텍스처 설정
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) t->SetActive();
		//메시의 버텍스 배열을 활성화
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//그리기
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
