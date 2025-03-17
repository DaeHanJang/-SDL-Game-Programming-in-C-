#include "VertexArray.h"
#include "glew.h"

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices) {
	//버텍스 배열 개체 생성
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	//버텍스 버퍼 생성
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

	//인덱스 버퍼 생성
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), 
		indices, GL_STATIC_DRAW);

	//버텍스 속성 지정(지금은 하나의 버텍스 형식을 가정)
	//위치는 3개의 float
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	//법선은 3개의 float
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
		reinterpret_cast<void*>(sizeof(float) * 3));
	//텍스처 좌표는 2개의 float
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
		reinterpret_cast<void*>(sizeof(float) * 6));

}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() { glBindVertexArray(mVertexArray); }
