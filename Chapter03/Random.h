#pragma once

#include <random>
#include "Math.h"

class Random {
public:
	static void Init();

	//지정된 int값을 사용하는 시드 생성기(일반적으로 수동으로 사용할 필요 없음)
	static void Seed(unsigned int seed);

	static float GetFloat(); //0.f와 1.f사이의 float값 얻음
	static float GetFloatRange(float min, float max); //지정된 범위에서 float값 얻음
	static int GetIntRange(int min, int max); //지정된 범위에서 int값 얻음
	//주어진 최소, 최댓값 범위로 랜덤 벡터를 가져옴
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
	static Vector3 GetVector(const Vector3& min, const Vector3& max);
private:
	static std::mt19937 sGenerator;
};
