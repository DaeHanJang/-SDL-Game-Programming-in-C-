#pragma once

#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	//메시 컴포넌트 그림
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	//메시 설정/메시 컴포넌트의 인덱스를 사용하여 텍스처 설정
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};
