#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner, int shaderIndex)
	: Component(owner), mMesh(nullptr), mTextureIndex(0), ShaderIndex(shaderIndex) {
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this, ShaderIndex);
}

MeshComponent::~MeshComponent() {
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this, ShaderIndex);
}

void MeshComponent::Draw(Shader* shader) {
	if (mMesh) {
		//���� ��ȯ ����
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		//���ݻ� ���� ����
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		//Ȱ��ȭ�� �ؽ�ó ����
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) t->SetActive();
		//Ȱ��ȭ�� �޽��� ���ؽ� �迭 ����
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		//�޽ø� �׸�
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
