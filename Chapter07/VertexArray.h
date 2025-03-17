#pragma once
class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	//버텍스 버퍼에 얼마나 버텍스가 있는지
	unsigned int mNumVerts;
	//인덱스 버퍼에 얼마나 인덱스가 있는지
	unsigned int mNumIndices;
	//버텍스 버퍼의 OpenGL ID
	unsigned int mVertexBuffer;
	//인덱스 버퍼의 OpenGL ID
	unsigned int mIndexBuffer;
	//버텍스 배열 객체의 OpenGL ID
	unsigned int mVertexArray;
};
