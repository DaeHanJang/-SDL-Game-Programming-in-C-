#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "SDL.h"
#include "Math.h"

struct DirectionalLight {
	//���� ����
	Vector3 mDirection;
	//���ݻ� ����
	Vector3 mDiffuseColor;
	//���ݻ� ����
	Vector3 mSpecColor;
};

class Renderer {
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	//�ε�� �ؽ�ó�� ��
	std::unordered_map<std::string, class Texture*> mTextures;
	//�ε�� �޽��� ��
	std::unordered_map<std::string, class Mesh*> mMeshes;

	//�׷��� ��� ��������Ʈ ������Ʈ
	std::vector<class SpriteComponent*> mSprites;

	//�׷��� ��� �޽� ������Ʈ
	std::vector<class MeshComponent*> mMeshComps;

	//����
	class Game* mGame;

	//��������Ʈ ���̴�
	class Shader* mSpriteShader;
	//��������Ʈ ���ؽ� �迭
	class VertexArray* mSpriteVerts;

	//�޽� ���̴�
	class Shader* mMeshShader;

	//3D ���̴��� ���� ��/���� ���
	Matrix4 mView;
	Matrix4 mProjection;
	//��ũ���� �ʺ�/����
	float mScreenWidth;
	float mScreenHeight;

	//���� ������
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	//������
	SDL_Window* mWindow;
	//OpenGL ���ؽ�Ʈ
	SDL_GLContext mContext;
};
