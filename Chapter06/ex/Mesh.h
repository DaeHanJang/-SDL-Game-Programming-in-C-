#pragma once

#include <string>
#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh();
	//메시 로드/언로드
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//이 메시에 해당하는 버텍스 배열을 얻음
	class VertexArray* GetVertexArray() { return mVertexArray; }
	class Texture* GetTexture(size_t index); //특정 인덱스에 해당하는 텍스처를 얻음
	//셰이더의 이름을 얻음
	const std::string& GetShaderName() const { return mShaderName; }
	//오브젝트 공간 바운딩 구체 반지름을 얻음
	float GetRadius() const { return mRadius; }
	//정반사 지수을 얻음
	float GetSpecPower() const { return mSpecPower; }
private:
	std::vector<class Texture*> mTextures; //이 메시에 사용되는 텍스처들
	class VertexArray* mVertexArray; //메시의 버텍스 배열
	std::string mShaderName; //메시가 지정한 셰이더 이름
	float mRadius; //오브젝트 공간 바운딩 구체의 반지름값 저장
	float mSpecPower; //정반사 지수
};
