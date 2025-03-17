#pragma once

#include "glew.h"
#include <string>
#include "Math.h"

class Shader {
public:
	Shader();
	~Shader();
	//지정된 이름으로 버텍스/프래그먼트 셰이더 로드
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//셰이더 프로그램 활성화
	void SetActive();
	//행렬 uniform 설정
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	//3D 벡터 uniform 설정
	void SetVectorUniform(const char* name, const Vector3& vector);
	//float uniform 설정
	void SetFloatUniform(const char* name, float value);
private:
	//지정된 셰이더로 컴파일 시도
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	
	//셰이더가 성공적으로 컴파일되었는지 테스트
	bool IsCompiled(GLuint shader);
	//버텍스/프래그먼트 프로그램 링크외었는지 테스트
	bool IsValidProgram();
private:
	//셰이더 ID 저장
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
