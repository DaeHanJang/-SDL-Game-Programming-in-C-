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

	//OpenGL �Ӽ� ����
	//�ھ� OpenGL �������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//3.3 ���� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//RGBA ä�δ� 8��Ʈ ���� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//���� ���۸� Ȱ��ȭ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//OpenGL �ϵ���� ����ȭ ��� ����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//OpenGL ���ؽ�Ʈ ����
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	//�Ϻ� �÷������� GLEW�� ���� �ڵ带 ���� �׷��Ƿ� �������� ����
	glGetError();

	//���̴� ����/������ ���� Ȯ��
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	//��������Ʈ�� �׸��� ���� �簢�� ����
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
	//�ؽ�ó ����
	for (auto i : mTextures) {
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	//�޽� ����
	for (auto i : mMeshes) {
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw() {
	//���� ȸ������ ä���
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//���� ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�޽� ������Ʈ �׸���
	//���� ���۸� Ȱ��ȭ�ϰ� ���� ������ ��
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//�⺻ �޽� ���̴� Ȱ��ȭ
	mBasicMeshShader->SetActive();
	//��-���� ��� ����
	mBasicMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	//���� uniforms ����
	SetLightUniforms(mBasicMeshShader);
	for (auto mc : mBasicMeshComps) mc->Draw(mBasicMeshShader);

	mPhongMeshShader->SetActive();
	mPhongMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	SetLightUniforms(mPhongMeshShader);
	for (auto mc : mPhongMeshComps) mc->Draw(mPhongMeshShader);

	//��� ��������Ʈ ������Ʈ �׸���
	//���� ���� ��Ȱ��ȭ
	glDisable(GL_DEPTH_TEST);
	//���� ���� ��� �� ���� ���� Ȱ��ȭ
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//���̴�/���ؽ� �迭��ü Ȱ��ȭ
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites) sprite->Draw(mSpriteShader);

	//���� ��ü
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	//���ĵ� ���Ϳ��� ���� ������ ã��(ù �� ��Ҵ� �� ���� �켱������ ����)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	//�ݺ��� ��ġ �տ� ��� ����
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
	//��������Ʈ ���̴� ����
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) return false;

	mSpriteShader->SetActive();
	//��-���� ��� ����
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	//basic �޽� ���̴� ����
	mBasicMeshShader = new Shader();
	if (!mBasicMeshShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag")) return false;

	mPhongMeshShader = new Shader();
	if (!mPhongMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) return false;

	mBasicMeshShader->SetActive();
	mPhongMeshShader->SetActive();
	//��-���� ��� ����
	mView = Matrix4::CreateLookAt(
		Vector3::Zero, //ī�޶� ��ġ
		Vector3::UnitX, //Ÿ�� ��ġ
		Vector3::UnitZ //���� ����
	);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.f), //���� FOV
		mScreenWidth, //���� �ʺ�
		mScreenHeight, //���� ����
		25.f, //����� ������ �Ÿ�
		10000.f //�� ������ �Ÿ�
	);
	mBasicMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	mPhongMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::CreateSpriteVerts() {
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, //��� ����
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, //��� ������
		0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, //�ϴ� ������
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  //�ϴ� ����
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader) {
	//ī�޶� ��ġ�� �ι�Ʈ�� �� ���(�����)���� ���� �� ����
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	//�ֺ���
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	//���Ɽ
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
