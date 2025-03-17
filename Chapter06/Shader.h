#pragma once

#include <string>
#include "Math.h"
#include <glew.h>

class Shader {
public:
	Shader();
	~Shader();
	//�־��� �̸����� ���ؽ�/�����׸�Ʈ ���̴� �ε�
	bool Load(const std::string& vertName, const std::string& fragNam);
	void Unload();
	//Ȱ�� ���̴� ���α׷����� ����
	void SetActive();
	//���Ÿ���� uniform ����
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	//����Ÿ���� uniform ����
	void SetVectorUniform(const char* name, const Vector3& vector);
	//floatŸ���� uniform ����
	void SetFloatUniform(const char* name, float value);
private:
	//������ ���̴��� ������
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	//���̴��� ���������� �����ϵƴ��� Ȯ��
	bool IsCompiled(GLuint shader);
	//���ؽ�/�����׸�Ʈ ���α׷��� ����ƴ��� Ȯ��
	bool IsValidProgram();

	//���̴� ������Ʈ ID�� ����
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
