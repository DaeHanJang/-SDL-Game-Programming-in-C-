#pragma once

class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive(); //�� ���ؽ� �迭�� Ȱ��ȭ(Ȱ��ȭ�� �ϸ� �� ���ؽ� �迭�� �׸� �� �ִ�)

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	unsigned int mNumVerts; //���ؽ� ���ۿ� �����ϴ� ���ؽ��� ��
	unsigned int mNumIndices; //�ε��� ���ۿ� �����ϴ� �ε����� ��
	unsigned int mVertexBuffer; //���ؽ� ������ OpenGL ID
	unsigned int mIndexBuffer; //�ε��� ������ OpenGL ID
	unsigned int mVertexArray; //���ؽ� �迭 ��ü�� OpenGL ID
};
