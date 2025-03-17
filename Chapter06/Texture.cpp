#include "Texture.h"
#include "SOIL2.h"
#include "SDL.h"
#include "glew.h"

Texture::Texture() :mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string& fileName) {
	int channels = 0;
	unsigned char* image = SOIL_load_image(
		fileName.c_str(), //파일이름
		&mWidth, //너비 저장
		&mHeight, //높이 저장
		&channels, //채널 수 저장
		SOIL_LOAD_AUTO //이미지 파일 타입 또는 자동
	);
	if (image == nullptr) {
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4) format = GL_RGBA;
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(
		GL_TEXTURE_2D, //텍스처 타깃
		0, //LOD
		format, //OpenGL이 상용해야 되는 색상 포맷
		mWidth, //텍스처의 너비
		mHeight, //텍스처의 높이
		0, //보더
		format, //입력 데이터의 색상 포맷
		GL_UNSIGNED_BYTE, //입력 데이터의 비트 깊이
		image //이미지 데이터의 포인터
	);
	SOIL_free_image_data(image);
	//이중 선형 필터링 활성화
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() { glDeleteTextures(1, &mTextureID); }

void Texture::SetActive() { glBindTexture(GL_TEXTURE_2D, mTextureID); }
