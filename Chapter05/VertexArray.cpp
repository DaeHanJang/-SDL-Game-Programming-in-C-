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
	glBufferData(
		GL_ARRAY_BUFFER, //데이터를 쓸 버퍼의 버퍼 타입
		numVerts * 5 * sizeof(float), //복사할 바이트 크기
		verts, //복사할 소스(포인터)
		GL_STATIC_DRAW //이 데이터를 어떻게 사용할지
	);

	//인덱스 버퍼 생성
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, //인덱스 버퍼
		numIndices * sizeof(unsigned int), //데이터의 크기
		indices, 
		GL_STATIC_DRAW
	);

	//버텍스 속성 지정(지금은 하나의 버텍스 형식을 가정)
	//위치는 오프셋 0부터 시작하여 3개의 float
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, //속성 인덱스(첫 번째 ㅂ텍스 속성 인덱스는 0)
		3, //요소의 수
		GL_FLOAT, //요소의 타입
		GL_FALSE, //(정수형 타입에서만 사용)
		sizeof(float) * 5, //간격(일반적으로 각 버텍스의 크기)
		0 //버텍스의 시작에서 이 속성까지의 오프셋
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, //버텍스 속성 인덱스
		2, //컴포넌트 수(UV, 2)
		GL_FLOAT, //각 컴포넌트의 타입
		GL_FALSE, //GL_FLOAT 에서는 사용되지 않음
		sizeof(float) * 5, //간격(간격은 항상 버텍스의 크기)
		reinterpret_cast<void*>(sizeof(float) * 3) //오프셋 포인터(offsetof 매크로 유용)
	);
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() { glBindVertexArray(mVertexArray); }
