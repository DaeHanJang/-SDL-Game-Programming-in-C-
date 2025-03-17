#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "glew.h"

Renderer::Renderer(Game* game) : mGame(game), mSpriteShader(nullptr), mMeshShader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeight) {
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	//OpenGL 속성 설정
	//코어 OpenGL 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//버전 3.3 지정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA 채널 당 8비트의 색상 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//OpenGL이 하드웨어 가속을 사용하도록 강제
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 7)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//OpenGL 컨텍스트 생성
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	//일부 플래폼에서 GLEW 에러 코드 생성
	//그래서 지움
	glGetError();

	//셰이더를 생성/컴파일할 수 있는지 확인
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	//스프라이트로 그릴 사각형 생성
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown() {
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData() {
	//텍스처 삭제
	for (auto i : mTextures) {
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	//메시 삭제
	for (auto i : mMeshes) {
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw() {
	//클리어 색상을 밝은 회색으로 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//색상 버퍼 지움
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//메시 컴포넌트 그림
	//깊이 버퍼 활성화/알파 블렌딩 비활성화
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//메시 셰이더 활성화
	mMeshShader->SetActive();
	//뷰-투영 행렬 갱신
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//광원 uniform 갱신
	SetLightUniforms(mMeshShader);
	for (auto mc : mMeshComps) mc->Draw(mMeshShader);

	//모든 스프라이트 컴포넌트 그림
	//깊이 버퍼 비활성화
	glDisable(GL_DEPTH_TEST);
	//색상 버퍼의 알파 블렌딩 활성화
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//셰이더/버텍스 배열 객체 활성화
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites) sprite->Draw(mSpriteShader);

	//버퍼 교체
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	//정렬된 벡터에서 삽입 지점을 찾음
	//(나보다 순위가 높은 첫 번째 요소)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	//반복자의 전 위치에 요소 삽입
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh) { mMeshComps.emplace_back(mesh); }

void Renderer::RemoveMeshComp(MeshComponent* mesh) {
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName) {
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) tex = iter->second;
	else {
		tex = new Texture();
		if (tex->Load(fileName)) mTextures.emplace(fileName, tex);
		else {
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName) {
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end()) m = iter->second;
	else {
		m = new Mesh();
		if (m->Load(fileName, this)) mMeshes.emplace(fileName, m);
		else {
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders() {
	//스프라이트 셰이더 생성
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) return false;

	mSpriteShader->SetActive();
	//뷰-투영 행렬 설정
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	//basic 메시 셰이더 생성
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) return false;

	mMeshShader->SetActive();
	//뷰-투영 행렬 설정
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.f),
		mScreenWidth, mScreenHeight, 25.f, 10000.f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, //상단 왼쪽
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, //상단 오른쪽
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, //하단 오른쪽
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  //하단 왼쪽
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader) {
	//카메라 위치은 뷰의 역행렬
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	//주변광
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//방향광
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}
