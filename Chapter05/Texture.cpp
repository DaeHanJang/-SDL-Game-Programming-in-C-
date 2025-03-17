#include "Texture.h"
#include "SOIL2.h"
#include "SDL.h"
#include "glew.h"

Texture::Texture() :mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string& fileName) {
	int channels = 0;
	unsigned char* image = SOIL_load_image(
		fileName.c_str(), //�����̸�
		&mWidth, //�ʺ� ����
		&mHeight, //���� ����
		&channels, //ä�� �� ����
		SOIL_LOAD_AUTO //�̹��� ���� Ÿ�� �Ǵ� �ڵ�
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
		GL_TEXTURE_2D, //�ؽ�ó Ÿ��
		0, //LOD
		format, //OpenGL�� ����ؾ� �Ǵ� ���� ����
		mWidth, //�ؽ�ó�� �ʺ�
		mHeight, //�ؽ�ó�� ����
		0, //����
		format, //�Է� �������� ���� ����
		GL_UNSIGNED_BYTE, //�Է� �������� ��Ʈ ����
		image //�̹��� �������� ������
	);
	SOIL_free_image_data(image);
	//���� ���� ���͸� Ȱ��ȭ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() { glDeleteTextures(1, &mTextureID); }

void Texture::SetActive() { glBindTexture(GL_TEXTURE_2D, mTextureID); }
