#include "Shader.h"
#include <fstream>
#include <sstream>
#include <SDL.h>
#include "Texture.h"

Shader::Shader() : mShaderProgram(0), mVertexShader(0), mFragShader(0) {}

Shader::~Shader() {}

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
	//버텍스와 프래그먼트 셰이더 컴파일
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) return false;

	//버텍스/프래그먼트 셰이더를서로 연결하는 셰이더 프로그램 생성
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	//프로그램이 성공적으로 연결됐는지 검증
	if (!IsValidProgram()) return false;
	return true;
}

void Shader::Unload() {
	//프로그램/셰이더 삭제
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive() {
	//프로그램 활성화
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) {
	//해당 이름의 uniform을 찾음
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//행렬 데이터를 uniform에 전송
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector) {
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//벡터 데이터를 uniform에 전송
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value) {
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//float 데이터를 uniform에 전송
	glUniform1f(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
	//피일 열기
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) {
		//모든 텍스트를 읽어 스트링에 넣음
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		//지정된 타입의 셰이더 생성
		outShader = glCreateShader(shaderType);
		//소스 문자열을 설정하고 컴파일 시도
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
		if (!IsCompiled(outShader)) {
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else {
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader) {
	GLint status;

	//컴파일 상태 질의
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed: \n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram() {
	GLint status;

	//링크 상태 질의
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Failed: \n%s", buffer);
		return false;
	}

	return true;
}
