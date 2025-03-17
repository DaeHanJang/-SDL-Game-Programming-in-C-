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

struct PointLight {
	Vector3 mPosition; //��ġ
	Vector3 mDiffuseColor; //���ݻ� ����
	Vector3 mSpecColor; //���ݻ� ����
	Vector3 mSpecPower; //���ݻ� ����
	float mRange; //����
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

	void AddMeshComp(class MeshComponent* mesh, int n);
	void RemoveMeshComp(class MeshComponent* mesh, int n);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }
	PointLight& GetPointLight(int n) { return mPotLight[n]; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, class Texture*> mTextures; //�ҷ��� �ؽ�ó ��
	std::unordered_map<std::string, class Mesh*> mMeshes; //�ҷ��� �޽� ��

	std::vector<class SpriteComponent*> mSprites; //�׷��� ��� ��������Ʈ ������Ʈ

	//�׷��� ��� �޽� ������Ʈ
	std::vector<class MeshComponent*> mBasicMeshComps;
	std::vector<class MeshComponent*> mPhongMeshComps;

	class Game* mGame; //����

	class Shader* mSpriteShader; //��������Ʈ ���̴�
	class VertexArray* mSpriteVerts; //���ؽ� �迭

	//�޽� ���̴�
	class Shader* mBasicMeshShader;
	class Shader* mPhongMeshShader;

	//3D ���̴��� ���� ��/���� ���
	Matrix4 mView;
	Matrix4 mProjection;
	//ȭ���� �ʺ�/����
	float mScreenWidth;
	float mScreenHeight;

	//���� ������
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;
	PointLight mPotLight[4];

	SDL_Window* mWindow; //������
	SDL_GLContext mContext; //OpenGL ���ؽ�Ʈ
};
