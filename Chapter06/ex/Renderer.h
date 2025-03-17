#pragma once

#include <string>
#include "Math.h"
#include <unordered_map>
#include "SDL.h"

struct DirectionalLight {
	Vector3 mDirection; //방향광
	Vector3 mDiffuseColor; //난반사 색상
	Vector3 mSpecColor; //정반사 색상
};

struct PointLight {
	Vector3 mPosition; //위치
	Vector3 mDiffuseColor; //난반사 색상
	Vector3 mSpecColor; //정반사 색상
	Vector3 mSpecPower; //정반사 지수
	float mRange; //범위
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

	std::unordered_map<std::string, class Texture*> mTextures; //불러온 텍스처 맵
	std::unordered_map<std::string, class Mesh*> mMeshes; //불러온 메시 맵

	std::vector<class SpriteComponent*> mSprites; //그려질 모든 스프라이트 컴포넌트

	//그려질 모든 메시 컴포넌트
	std::vector<class MeshComponent*> mBasicMeshComps;
	std::vector<class MeshComponent*> mPhongMeshComps;

	class Game* mGame; //게임

	class Shader* mSpriteShader; //스프라이트 셰이더
	class VertexArray* mSpriteVerts; //버텍스 배열

	//메시 셰이더
	class Shader* mBasicMeshShader;
	class Shader* mPhongMeshShader;

	//3D 셰이더를 위한 뷰/투영 행렬
	Matrix4 mView;
	Matrix4 mProjection;
	//화면의 너비/높이
	float mScreenWidth;
	float mScreenHeight;

	//광원 데이터
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;
	PointLight mPotLight[4];

	SDL_Window* mWindow; //윈도우
	SDL_GLContext mContext; //OpenGL 컨텍스트
};
