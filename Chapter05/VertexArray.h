#pragma once

class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts, 
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive(); //이 버텍스 배열을 활성화(활성화를 하면 이 버텍스 배열을 그릴 수 있다)

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	unsigned int mNumVerts; //버텍스 버퍼에 존재하는 버텍스의 수
	unsigned int mNumIndices; //인덱스 버퍼에 존재하는 인덱수의 수
	unsigned int mVertexBuffer; //버텍스 버퍼의 OpenGL ID
	unsigned int mIndexBuffer; //인덱스 버퍼의 OpenGL ID
	unsigned int mVertexArray; //버텍스 배열 개체의 OpenGL ID
};
