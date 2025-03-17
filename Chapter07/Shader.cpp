#include "Shader.h"
#include "Texture.h"
#include "SDL.h"
#include <fstream>
#include <sstream>

Shader::Shader() : mShaderProgram(0), mVertexShader(0), mFragShader(0) {}

Shader::~Shader() {}

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
	//버텍스/픽셀 셰이더 컴파일
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) || 
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) return false;
	
	//버텍스/프래그먼트 셰이더를 연결하는 
	//셰이더 프로그램 생성
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	
	//프로그램이 성공적으로 연결되었는지 확인
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
	//활성화된 프로그램으로 설정
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//unifom으로 행렬 데이터 전송
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector) {
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//벡터 데이터 전송
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value) {
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	//float 데이터 전송
	glUniform1f(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
	//파일 열기
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) {
		//문자열에 들어있는 모든 텍스트 읽기
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		
		//지정된 형식의 셰이더 생성
		outShader = glCreateShader(shaderType);
		//소스 문자열 설정 및 컴파일 시도
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
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
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
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	
	return true;
}
