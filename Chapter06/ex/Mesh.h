#pragma once

#include <string>
#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh();
	//�޽� �ε�/��ε�
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//�� �޽ÿ� �ش��ϴ� ���ؽ� �迭�� ����
	class VertexArray* GetVertexArray() { return mVertexArray; }
	class Texture* GetTexture(size_t index); //Ư�� �ε����� �ش��ϴ� �ؽ�ó�� ����
	//���̴��� �̸��� ����
	const std::string& GetShaderName() const { return mShaderName; }
	//������Ʈ ���� �ٿ�� ��ü �������� ����
	float GetRadius() const { return mRadius; }
	//���ݻ� ������ ����
	float GetSpecPower() const { return mSpecPower; }
private:
	std::vector<class Texture*> mTextures; //�� �޽ÿ� ���Ǵ� �ؽ�ó��
	class VertexArray* mVertexArray; //�޽��� ���ؽ� �迭
	std::string mShaderName; //�޽ð� ������ ���̴� �̸�
	float mRadius; //������Ʈ ���� �ٿ�� ��ü�� �������� ����
	float mSpecPower; //���ݻ� ����
};
