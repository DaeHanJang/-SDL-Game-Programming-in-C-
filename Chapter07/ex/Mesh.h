#pragma once

#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	//로드/언로드 메시
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	//메시에 연결된 버텍스 배열 얻기
	class VertexArray* GetVertexArray() { return mVertexArray; }
	//지정된 인덱스로 텍스처 얻기
	class Texture* GetTexture(size_t index);
	//셰이더 이름 얻기
	const std::string& GetShaderName() const { return mShaderName; }
	//오브젝트 공간 경계 구 반지름 얻기
	float GetRadius() const { return mRadius; }
	//메시의 정반사 지수 얻기
	float GetSpecPower() const { return mSpecPower; }
private:
	//메시에 연결된 텍스처
	std::vector<class Texture*> mTextures;
	//메시에 연결된 버텍스 배열
	class VertexArray* mVertexArray;
	//메시에 지정된 셰이더 이름
	std::string mShaderName;
	//오브젝트 공간 경계 구 반지름 저장
	float mRadius;
	//표면의 정반사 지수
	float mSpecPower;
};
