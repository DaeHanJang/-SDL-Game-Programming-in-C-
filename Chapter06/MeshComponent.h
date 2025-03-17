#pragma once

#include "Component.h"

class MeshComponent : public Component {
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	//제공된 셰이더로 이 메시 컴포넌트를 그림
	virtual void Draw(class Shader* shader);
	//메시 컴포넌트가 사용하는 메시/텍스처를 설정
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
private:
	class Mesh* mMesh;
	size_t mTextureIndex;
};
