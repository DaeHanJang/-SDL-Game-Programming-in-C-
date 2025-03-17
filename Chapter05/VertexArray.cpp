#include "VertexArray.h"
#include "glew.h"

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices) {
	//���ؽ� �迭 ��ü ����
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	//���ؽ� ���� ����
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER, //�����͸� �� ������ ���� Ÿ��
		numVerts * 5 * sizeof(float), //������ ����Ʈ ũ��
		verts, //������ �ҽ�(������)
		GL_STATIC_DRAW //�� �����͸� ��� �������
	);

	//�ε��� ���� ����
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, //�ε��� ����
		numIndices * sizeof(unsigned int), //�������� ũ��
		indices, 
		GL_STATIC_DRAW
	);

	//���ؽ� �Ӽ� ����(������ �ϳ��� ���ؽ� ������ ����)
	//��ġ�� ������ 0���� �����Ͽ� 3���� float
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, //�Ӽ� �ε���(ù ��° ���ؽ� �Ӽ� �ε����� 0)
		3, //����� ��
		GL_FLOAT, //����� Ÿ��
		GL_FALSE, //(������ Ÿ�Կ����� ���)
		sizeof(float) * 5, //����(�Ϲ������� �� ���ؽ��� ũ��)
		0 //���ؽ��� ���ۿ��� �� �Ӽ������� ������
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, //���ؽ� �Ӽ� �ε���
		2, //������Ʈ ��(UV, 2)
		GL_FLOAT, //�� ������Ʈ�� Ÿ��
		GL_FALSE, //GL_FLOAT ������ ������ ����
		sizeof(float) * 5, //����(������ �׻� ���ؽ��� ũ��)
		reinterpret_cast<void*>(sizeof(float) * 3) //������ ������(offsetof ��ũ�� ����)
	);
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() { glBindVertexArray(mVertexArray); }
