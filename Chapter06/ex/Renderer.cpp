#include "Renderer.h"
#include "glew.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "VertexArray.h"
#include "SpriteComponent.h"

Renderer::Renderer(Game* game) : mGame(game), mSpriteShader(nullptr), mBasicMeshShader(nullptr), mPhongMeshShader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeight) {
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	//OpenGL 속성 설정
	//코어 OpenGL 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//3.3 버전 설정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA 채널당 8비트 색상 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//OpenGL 하드웨어 가속화 사용 강제
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
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

	//일부 플랫폼에서 GLEW은 에러 코드를 보냄 그러므로 에러값을 제거
	glGetError();

	//셰이더 생성/컴파일 가능 확인
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	//스프라이트를 그리기 위한 사각형 생성
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown() {
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mBasicMeshShader->Unload();
	mPhongMeshShader->Unload();
	delete mBasicMeshShader;
	delete mPhongMeshShader;
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
	//밝은 회색으로 채우기
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//색상 버퍼 비우기
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//메시 컴포넌트 그리기
	//깊이 버퍼를 활성화하고 알파 블렌딩을 끔
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//기본 메시 셰이더 활성화
	mBasicMeshShader->SetActive();
	//뷰-투영 행렬 갱신
	mBasicMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//광원 uniforms 갱신
	SetLightUniforms(mBasicMeshShader);
	for (auto mc : mBasicMeshComps) mc->Draw(mBasicMeshShader);

	mPhongMeshShader->SetActive();
	mPhongMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	SetLightUniforms(mPhongMeshShader);
	for (auto mc : mPhongMeshComps) mc->Draw(mPhongMeshShader);

	//모든 스프라이트 컴포넌트 그리기
	//깊이 버퍼 비활성화
	glDisable(GL_DEPTH_TEST);
	//색상 버퍼 사용 시 알파 블렌딩 활성화
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//셰이더/버텍스 배열객체 활성화
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites) sprite->Draw(mSpriteShader);

	//버퍼 교체
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	//정렬된 벡터에서 삽입 지점을 찾음(첫 앞 요소는 더 높은 우선순위를 갖음)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	//반복자 위치 앞에 요소 삽입
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh, int n) { 
	if (n == 0) mBasicMeshComps.emplace_back(mesh);
	else mPhongMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh, int n) {
	if (n == 0) {
		auto iter = std::find(mBasicMeshComps.begin(), mBasicMeshComps.end(), mesh);
		mBasicMeshComps.erase(iter);
	}
	else {
		auto iter = std::find(mPhongMeshComps.begin(), mPhongMeshComps.end(), mesh);
		mPhongMeshComps.erase(iter);
	}
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

Mesh* Renderer::GetMesh(const std::string& fileName) {
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
	mBasicMeshShader = new Shader();
	if (!mBasicMeshShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag")) return false;

	mPhongMeshShader = new Shader();
	if (!mPhongMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) return false;

	mBasicMeshShader->SetActive();
	mPhongMeshShader->SetActive();
	//뷰-투영 행렬 설정
	mView = Matrix4::CreateLookAt(
		Vector3::Zero, //카메라 위치
		Vector3::UnitX, //타깃 위치
		Vector3::UnitZ //상향 벡터
	);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.f), //수평 FOV
		mScreenWidth, //뷰의 너비
		mScreenHeight, //뷰의 높이
		25.f, //가까운 평면과의 거리
		10000.f //먼 평면과의 거리
	);
	mBasicMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	mPhongMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, //상단 왼쪽
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, //상단 오른쪽
		0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, //하단 오른쪽
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  //하단 왼쪽
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader) {
	//카메라 위치는 인버트된 뷰 행렬(역행렬)에서 얻을 수 있음
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	//주변광
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//방향광
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);

	for (int i = 0; i < 4; i++) {
		std::string str1 = "uPotLight[";
		str1.append(std::to_string(i)) += "].mPosition";
		shader->SetVectorUniform(str1.c_str(), mPotLight[i].mPosition);
		std::string str2 = "uPotLight[";
		str1.append(std::to_string(i)) += "].mDiffuseColor";
		shader->SetVectorUniform(str2.c_str(), mPotLight[i].mDiffuseColor);
		std::string str3 = "uPotLight[";
		str1.append(std::to_string(i)) += "].mSpecColor";
		shader->SetVectorUniform(str3.c_str(), mPotLight[i].mSpecColor);
		std::string str4 = "uPotLight[";
		str1.append(std::to_string(i)) += "].mSpecPower";
		shader->SetVectorUniform(str4.c_str(), mPotLight[i].mSpecPower);
		std::string str5 = "uPotLight[";
		str1.append(std::to_string(i)) += "].mRange";
		shader->SetFloatUniform(str5.c_str(), mPotLight[i].mRange);
	}
}
