#pragma once

#include "glew.h"
#include <string>
#include "Math.h"

class Shader {
public:
	Shader();
	~Shader();
	//������ �̸����� ���ؽ�/�����׸�Ʈ ���̴� �ε�
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//���̴� ���α׷� Ȱ��ȭ
	void SetActive();
	//��� uniform ����
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	//3D ���� uniform ����
	void SetVectorUniform(const char* name, const Vector3& vector);
	//float uniform ����
	void SetFloatUniform(const char* name, float value);
private:
	//������ ���̴��� ������ �õ�
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	
	//���̴��� ���������� �����ϵǾ����� �׽�Ʈ
	bool IsCompiled(GLuint shader);
	//���ؽ�/�����׸�Ʈ ���α׷� ��ũ�ܾ����� �׽�Ʈ
	bool IsValidProgram();
private:
	//���̴� ID ����
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
