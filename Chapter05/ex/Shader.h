#pragma once

#include <string>
#include "Math.h"
#include <glew.h>

class Shader {
public:
	Shader();
	~Shader();
	//주어진 이름으로 버텍스/프래그먼트 셰이더 로드
	bool Load(const std::string& vertName, const std::string& fragNam);
	void Unload();
	void SetActive(); //이 셰이더를 활성화된 셰이더 프로그램으로 설정
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	//지정된 셰이더를 컴파일
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader); //셰이더가 성공적으로 컴파일됐는지 확인
	bool IsValidProgram(); //버텍스/프래그먼트 프로그램이 연결됐는지 확인

	//셰이더 오브젝트 ID를 저장
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
