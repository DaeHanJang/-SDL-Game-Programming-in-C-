#pragma once

#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	//�ε�/��ε� �޽�
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//�޽ÿ� ����� ���ؽ� �迭 ���
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//������ �ε����� �ؽ�ó ���
	class Texture* GetTexture(size_t index);
	//���̴� �̸� ���
	const std::string& GetShaderName() const { return mShaderName; }
	//������Ʈ ���� ��� �� ������ ���
	float GetRadius() const { return mRadius; }
	//�޽��� ���ݻ� ���� ���
	float GetSpecPower() const { return mSpecPower; }
private:
	//�޽ÿ� ����� �ؽ�ó
	std::vector<class Texture*> mTextures;
	//�޽ÿ� ����� ���ؽ� �迭
	class VertexArray* mVertexArray;
	//�޽ÿ� ������ ���̴� �̸�
	std::string mShaderName;
	//������Ʈ ���� ��� �� ������ ����
	float mRadius;
	//ǥ���� ���ݻ� ����
	float mSpecPower;
};
