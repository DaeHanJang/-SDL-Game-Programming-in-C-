#pragma once

#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	//�޽� ������Ʈ �׸�
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	//�޽� ����/�޽� ������Ʈ�� �ε����� ����Ͽ� �ؽ�ó ����
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};
