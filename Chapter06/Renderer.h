#pragma once

#include <string>
#include "Math.h"
#include <unordered_map>
#include "SDL.h"

struct DirectionalLight {
	Vector3 mDirection; //���Ɽ
	Vector3 mDiffuseColor; //���ݻ� ����
	Vector3 mSpecColor; //���ݻ� ����
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

	std::unordered_map<std::string, class Texture*> mTextures; //�ҷ��� �ؽ�ó ��
	std::unordered_map<std::string, class Mesh*> mMeshes; //�ҷ��� �޽� ��

	std::vector<class SpriteComponent*> mSprites; //�׷��� ��� ��������Ʈ ������Ʈ

	std::vector<class MeshComponent*> mMeshComps; //�׷��� ��� �޽� ������Ʈ

	class Game* mGame; //����

	class Shader* mSpriteShader; //��������Ʈ ���̴�
	class VertexArray* mSpriteVerts; //���ؽ� �迭

	class Shader* mMeshShader; //�޽� ���̴�

	//3D ���̴��� ���� ��/���� ���
	Matrix4 mView;
	Matrix4 mProjection;
	//ȭ���� �ʺ�/����
	float mScreenWidth;
	float mScreenHeight;

	//���� ������
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	SDL_Window* mWindow; //������
	SDL_GLContext mContext; //OpenGL ���ؽ�Ʈ
};
