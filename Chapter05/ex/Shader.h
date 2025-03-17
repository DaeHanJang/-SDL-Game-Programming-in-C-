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
	void SetActive(); //�� ���̴��� Ȱ��ȭ�� ���̴� ���α׷����� ����
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	//������ ���̴��� ������
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader); //���̴��� ���������� �����ϵƴ��� Ȯ��
	bool IsValidProgram(); //���ؽ�/�����׸�Ʈ ���α׷��� ����ƴ��� Ȯ��

	//���̴� ������Ʈ ID�� ����
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
