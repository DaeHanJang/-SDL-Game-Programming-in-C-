#pragma once

#include <limits>

namespace Math {
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.f;
	const float PiOver2 = Pi / 2.f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegInfinity = -std::numeric_limits<float>::infinity();

	inline float ToRadians(float degrees) { return degrees * Pi / 180.0f; }

	inline float ToDegrees(float radians) { return radians * 180.0f / Pi; }

	inline bool NearZero(float val, float epsilon = 0.001f) {
		if (fabs(val) <= epsilon) return true;
		else return false;
	}

	template <typename T>
	T Max(const T& a, const T& b) { return (a < b ? b : a); }

	template <typename T>
	T Min(const T& a, const T& b) { return (a < b ? a : b); }

	template <typename T>
	T Clamp(const T& value, const T& lower, const T& upper) {
		return Min(upper, Max(lower, value));
	}

	inline float Abs(float value) { return fabs(value); }

	inline float Cos(float angle) { return cosf(angle); }

	inline float Sin(float angle) { return sinf(angle); }

	inline float Tan(float angle) { return tanf(angle); }

	inline float Acos(float value) { return acosf(value); }

	inline float Atan2(float y, float x) { return atan2f(y, x); }

	inline float Cot(float angle) { return 1.0f / Tan(angle); }

	inline float Lerp(float a, float b, float f) { return a + f * (b - a); }

	inline float Sqrt(float value) { return sqrtf(value); }

	inline float Fmod(float numer, float denom) { return fmod(numer, denom); }
}

//2D Vector
class Vector2 {
public:
	float x;
	float y;

	Vector2() :x(0.0f), y(0.0f) {}

	explicit Vector2(float inX, float inY) :x(inX), y(inY) {}

	void Set(float inX, float inY) {
		x = inX;
		y = inY;
	}

	// Vector addition (a + b)
	friend Vector2 operator+(const Vector2& a, const Vector2& b) {
		return Vector2(a.x + b.x, a.y + b.y);
	}

	// Vector subtraction (a - b)
	friend Vector2 operator-(const Vector2& a, const Vector2& b) {
		return Vector2(a.x - b.x, a.y - b.y);
	}

	// Component-wise multiplication
	// (a.x * b.x, ...)
	friend Vector2 operator*(const Vector2& a, const Vector2& b) {
		return Vector2(a.x * b.x, a.y * b.y);
	}

	// Scalar multiplication
	friend Vector2 operator*(const Vector2& vec, float scalar) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	// Scalar multiplication
	friend Vector2 operator*(float scalar, const Vector2& vec) {
		return Vector2(vec.x * scalar, vec.y * scalar);
	}

	// Scalar *=
	Vector2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	// Vector +=
	Vector2& operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;
		return *this;
	}

	// Vector -=
	Vector2& operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}

	//벡터의 길이 제곱
	float LengthSq() const { return (x * x + y * y); }
	//벡터 길이
	float Length() const { return (Math::Sqrt(LengthSq())); }
	//벡터 정규화
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
	}
	//정규화된 벡터 제공
	static Vector2 Normalize(const Vector2& vec) {
		Vector2 temp = vec;
		temp.Normalize();
		return temp;
	}
	//두 벡터 사이의 내적(a dot b)
	static float Dot(const Vector2& a, const Vector2& b) {
		return (a.x * b.x + a.y * b.y);
	}
	//a에서 b로 보간 계수/비율 f로 선형 보간
	static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
		return Vector2(a + f * (b - a));
	}
	//(정규화된)n(반사면의 법선 벡터)를 기준으로 v(입사 벡터)를 반사
	static Vector2 Reflect(const Vector2& v, const Vector2& n) {
		return v - 2.f * Vector2::Dot(v, n) * n;
	}
	//벡터를 행렬로 변환
	static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.f);

	static const Vector2 Zero;
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 NegUnitX;
	static const Vector2 NegUnitY;
};

//3D Vector
class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
	explicit Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

	//상수 float 포인터로 형변환
	const float* GetAsFloatPtr() const { return reinterpret_cast<const float*>(&x); }
	void Set(float inX, float inY, float inZ) {
		x = inX;
		y = inY;
		z = inZ;
	}

	// Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right) {
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	// Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar multiplication
	friend Vector3 operator*(float scalar, const Vector3& vec) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar *=
	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// Vector +=
	Vector3& operator+=(const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	// Vector -=
	Vector3& operator-=(const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	//벡터의 길이 제곱
	float LengthSq() const { return (x * x + y * y + z * z); }
	//벡터 길이
	float Length() const { return (Math::Sqrt(LengthSq())); }
	//벡터 정규화
	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}
	//정규화된 벡터 제공
	static Vector3 Normalize(const Vector3& vec) {
		Vector3 temp = vec;
		temp.Normalize();
		return temp;
	}
	//두 벡터 사이의 내적(a dot b)
	static float Dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	//두 벡터 사이의 외적(a cross b)
	static Vector3 Cross(const Vector3& a, const Vector3& b) {
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}
	//a에서 b로 보간 계수/비율 f로 선형 보간
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f) {
		return Vector3(a + f * (b - a));
	}
	//(정규화된)n(반사면의 법선 벡터)를 기준으로 v(입사 벡터)를 반사
	static Vector3 Reflect(const Vector3& v, const Vector3& n) {
		return v - 2.f * Vector3::Dot(v, n) * n;
	}
	//벡터를 행렬로 변환
	static Vector3 Transform(const Vector3& vec, const class Matrix4& mat, float w = 1.f);
	//벡터를 행렬로 변환하고 w 성분으로 정규화
	static Vector3 TransformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.f);

	//3D 벡터를 쿼터니언으로 변환
	static Vector3 Transform(const Vector3& v, const class Quaternion& q);

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegUnitX;
	static const Vector3 NegUnitY;
	static const Vector3 NegUnitZ;
	static const Vector3 Infinity;
	static const Vector3 NegInfinity;
};

//3*3 Matrix
class Matrix3 {
public:
	float mat[3][3];

	Matrix3() { *this = Matrix3::Identity; }

	explicit Matrix3(float inMat[3][3]) { memcpy(mat, inMat, 9 * sizeof(float)); }

	//상수 float 포인터로 형변환
	const float* GetAsFloatPtr() const { return reinterpret_cast<const float*>(&mat[0][0]); }

	// Matrix multiplication
	friend Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
		Matrix3 retVal;
		// row 0
		retVal.mat[0][0] =
			left.mat[0][0] * right.mat[0][0] +
			left.mat[0][1] * right.mat[1][0] +
			left.mat[0][2] * right.mat[2][0];

		retVal.mat[0][1] =
			left.mat[0][0] * right.mat[0][1] +
			left.mat[0][1] * right.mat[1][1] +
			left.mat[0][2] * right.mat[2][1];

		retVal.mat[0][2] =
			left.mat[0][0] * right.mat[0][2] +
			left.mat[0][1] * right.mat[1][2] +
			left.mat[0][2] * right.mat[2][2];

		// row 1
		retVal.mat[1][0] =
			left.mat[1][0] * right.mat[0][0] +
			left.mat[1][1] * right.mat[1][0] +
			left.mat[1][2] * right.mat[2][0];

		retVal.mat[1][1] =
			left.mat[1][0] * right.mat[0][1] +
			left.mat[1][1] * right.mat[1][1] +
			left.mat[1][2] * right.mat[2][1];

		retVal.mat[1][2] =
			left.mat[1][0] * right.mat[0][2] +
			left.mat[1][1] * right.mat[1][2] +
			left.mat[1][2] * right.mat[2][2];

		// row 2
		retVal.mat[2][0] =
			left.mat[2][0] * right.mat[0][0] +
			left.mat[2][1] * right.mat[1][0] +
			left.mat[2][2] * right.mat[2][0];

		retVal.mat[2][1] =
			left.mat[2][0] * right.mat[0][1] +
			left.mat[2][1] * right.mat[1][1] +
			left.mat[2][2] * right.mat[2][1];

		retVal.mat[2][2] =
			left.mat[2][0] * right.mat[0][2] +
			left.mat[2][1] * right.mat[1][2] +
			left.mat[2][2] * right.mat[2][2];

		return retVal;
	}

	Matrix3& operator*=(const Matrix3& right) {
		*this = *this * right;
		return *this;
	}

	//x, y 비율로 스케일 행렬 생성
	static Matrix3 CreateScale(float xScale, float yScale) {
		float temp[3][3] = {
			{ xScale, 0.f, 0.f },
			{ 0.f, yScale, 0.f },
			{ 0.f, 0.f, 1.f },
		};
		return Matrix3(temp);
	}
	static Matrix3 CreateScale(const Vector2& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y);
	}
	//단일 값으로 스케일 행렬 생성
	static Matrix3 CreateScale(float scale) { return CreateScale(scale, scale); }
	//z축을 기준으로 회전 행렬 생성(theta는 라다안)
	static Matrix3 CreateRotation(float theta) {
		float temp[3][3] = {
			{ Math::Cos(theta), Math::Sin(theta), 0.f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.f },
			{ 0.f, 0.f, 1.f },
		};
		return Matrix3(temp);
	}
	//평행 이동 행렬 생성(xy평면에서)
	static Matrix3 CreateTranslation(const Vector2& trans) {
		float temp[3][3] = {
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ trans.x, trans.y, 1.f },
		};
		return Matrix3(temp);
	}

	static const Matrix3 Identity;
};

//4*4 Matrix
class Matrix4 {
public:
	float mat[4][4];

	Matrix4() { *this = Matrix4::Identity; }

	explicit Matrix4(float inMat[4][4]) { memcpy(mat, inMat, 16 * sizeof(float)); }

	//상수 float 포인터로 형변환
	const float* GetAsFloatPtr() const { return reinterpret_cast<const float*>(&mat[0][0]); }

	// Matrix multiplication (a * b)
	friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
		Matrix4 retVal;
		// row 0
		retVal.mat[0][0] =
			a.mat[0][0] * b.mat[0][0] +
			a.mat[0][1] * b.mat[1][0] +
			a.mat[0][2] * b.mat[2][0] +
			a.mat[0][3] * b.mat[3][0];

		retVal.mat[0][1] =
			a.mat[0][0] * b.mat[0][1] +
			a.mat[0][1] * b.mat[1][1] +
			a.mat[0][2] * b.mat[2][1] +
			a.mat[0][3] * b.mat[3][1];

		retVal.mat[0][2] =
			a.mat[0][0] * b.mat[0][2] +
			a.mat[0][1] * b.mat[1][2] +
			a.mat[0][2] * b.mat[2][2] +
			a.mat[0][3] * b.mat[3][2];

		retVal.mat[0][3] =
			a.mat[0][0] * b.mat[0][3] +
			a.mat[0][1] * b.mat[1][3] +
			a.mat[0][2] * b.mat[2][3] +
			a.mat[0][3] * b.mat[3][3];

		// row 1
		retVal.mat[1][0] =
			a.mat[1][0] * b.mat[0][0] +
			a.mat[1][1] * b.mat[1][0] +
			a.mat[1][2] * b.mat[2][0] +
			a.mat[1][3] * b.mat[3][0];

		retVal.mat[1][1] =
			a.mat[1][0] * b.mat[0][1] +
			a.mat[1][1] * b.mat[1][1] +
			a.mat[1][2] * b.mat[2][1] +
			a.mat[1][3] * b.mat[3][1];

		retVal.mat[1][2] =
			a.mat[1][0] * b.mat[0][2] +
			a.mat[1][1] * b.mat[1][2] +
			a.mat[1][2] * b.mat[2][2] +
			a.mat[1][3] * b.mat[3][2];

		retVal.mat[1][3] =
			a.mat[1][0] * b.mat[0][3] +
			a.mat[1][1] * b.mat[1][3] +
			a.mat[1][2] * b.mat[2][3] +
			a.mat[1][3] * b.mat[3][3];

		// row 2
		retVal.mat[2][0] =
			a.mat[2][0] * b.mat[0][0] +
			a.mat[2][1] * b.mat[1][0] +
			a.mat[2][2] * b.mat[2][0] +
			a.mat[2][3] * b.mat[3][0];

		retVal.mat[2][1] =
			a.mat[2][0] * b.mat[0][1] +
			a.mat[2][1] * b.mat[1][1] +
			a.mat[2][2] * b.mat[2][1] +
			a.mat[2][3] * b.mat[3][1];

		retVal.mat[2][2] =
			a.mat[2][0] * b.mat[0][2] +
			a.mat[2][1] * b.mat[1][2] +
			a.mat[2][2] * b.mat[2][2] +
			a.mat[2][3] * b.mat[3][2];

		retVal.mat[2][3] =
			a.mat[2][0] * b.mat[0][3] +
			a.mat[2][1] * b.mat[1][3] +
			a.mat[2][2] * b.mat[2][3] +
			a.mat[2][3] * b.mat[3][3];

		// row 3
		retVal.mat[3][0] =
			a.mat[3][0] * b.mat[0][0] +
			a.mat[3][1] * b.mat[1][0] +
			a.mat[3][2] * b.mat[2][0] +
			a.mat[3][3] * b.mat[3][0];

		retVal.mat[3][1] =
			a.mat[3][0] * b.mat[0][1] +
			a.mat[3][1] * b.mat[1][1] +
			a.mat[3][2] * b.mat[2][1] +
			a.mat[3][3] * b.mat[3][1];

		retVal.mat[3][2] =
			a.mat[3][0] * b.mat[0][2] +
			a.mat[3][1] * b.mat[1][2] +
			a.mat[3][2] * b.mat[2][2] +
			a.mat[3][3] * b.mat[3][2];

		retVal.mat[3][3] =
			a.mat[3][0] * b.mat[0][3] +
			a.mat[3][1] * b.mat[1][3] +
			a.mat[3][2] * b.mat[2][3] +
			a.mat[3][3] * b.mat[3][3];

		return retVal;
	}

	Matrix4& operator*=(const Matrix4& right) {
		*this = *this * right;
		return *this;
	}

	void Invert(); //역행렬(매우 느림)
	//행렬에서 이동 성분 추출
	Vector3 GetTranslation() const { return Vector3(mat[3][0], mat[3][1], mat[3][2]); }
	//행렬의 x축 추출(앞)
	Vector3 GetXAxis() const { return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2])); }
	//행렬의 y축 추출(왼쪽)
	Vector3 GetYAxis() const { return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2])); }
	//행렬의 z축 추출(위)
	Vector3 GetZAxis() const { return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2])); }
	//행렬에서 스케일 성분 추출
	Vector3 GetScale() const {
		Vector3 retVal;
		retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
		retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
		retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
		return retVal;
	}
	//x, y, z 비율로 스케일 행렬 생성
	static Matrix4 CreateScale(float xScale, float yScale, float zScale) {
		float temp[4][4] = {
			{ xScale, 0.f, 0.f, 0.f },
			{ 0.f, yScale, 0.f, 0.f },
			{ 0.f, 0.f, zScale, 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateScale(const Vector3& scaleVector) {
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}
	//단일 값으로 스케일 행렬 생성
	static Matrix4 CreateScale(float scale) {
		return CreateScale(scale, scale, scale);
	}
	//x축 기준 회전
	static Matrix4 CreateRotationX(float theta) {
		float temp[4][4] = {
			{ 1.f, 0.f, 0.f , 0.f },
			{ 0.f, Math::Cos(theta), Math::Sin(theta), 0.f },
			{ 0.f, -Math::Sin(theta), Math::Cos(theta), 0.f },
			{ 0.f, 0.f, 0.f, 1.f },
		};
		return Matrix4(temp);
	}
	//y축 기준 회전
	static Matrix4 CreateRotationY(float theta) {
		float temp[4][4] = {
			{ Math::Cos(theta), 0.f, -Math::Sin(theta), 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ Math::Sin(theta), 0.f, Math::Cos(theta), 0.f },
			{ 0.f, 0.f, 0.f, 1.f },
		};
		return Matrix4(temp);
	}
	//z축 기준 회전
	static Matrix4 CreateRotationZ(float theta) {
		float temp[4][4] = {
			{ Math::Cos(theta), Math::Sin(theta), 0.f, 0.f },
			{ -Math::Sin(theta), Math::Cos(theta), 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, 1.f },
		};
		return Matrix4(temp);
	}
	//쿼터니언에서 4*4행렬 생성
	static Matrix4 CreateFromQuaternion(const class Quaternion& q);

	static Matrix4 CreateTranslation(const Vector3& trans) {
		float temp[4][4] = {
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ trans.x, trans.y, trans.z, 1.f }
		};
		return Matrix4(temp);
	}
	static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
		Vector3 zaxis = Vector3::Normalize(target - eye);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
		Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
		Vector3 trans;
		trans.x = -Vector3::Dot(xaxis, eye);
		trans.y = -Vector3::Dot(yaxis, eye);
		trans.z = -Vector3::Dot(zaxis, eye);

		float temp[4][4] = {
			{ xaxis.x, yaxis.x, zaxis.x, 0.f },
			{ xaxis.y, yaxis.y, zaxis.y, 0.f },
			{ xaxis.z, yaxis.z, zaxis.z, 0.f },
			{ trans.x, trans.y, trans.z, 1.f }
		};
		return Matrix4(temp);
	}

	static Matrix4 CreateOrtho(float width, float height, float near, float far) {
		float temp[4][4] = {
			{ 2.f / width, 0.f, 0.f, 0.f },
			{ 0.f, 2.f / height, 0.f, 0.f },
			{ 0.f, 0.f, 1.f / (far - near), 0.f },
			{ 0.f, 0.f, near / (near - far), 1.f }
		};
		return Matrix4(temp);
	}
	
	static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far) {
		float yScale = Math::Cot(fovY / 2.f);
		float xScale = yScale * height / width;
		float temp[4][4] = {
			{ xScale, 0.f, 0.f, 0.f },
			{ 0.f, yScale, 0.f, 0.f },
			{ 0.f, 0.f, far / (far - near), 1.f },
			{ 0.f, 0.f, -near * far / (far - near), 0.f }
		};
		return Matrix4(temp);
	}
	//6장 간단한 뷰-투영 행렬 생성
	static Matrix4 CreateSimpleViewProj(float width, float height) {
		float temp[4][4] = {
			{ 2.f / width, 0.f, 0.f, 0.f },
			{ 0.f, 2.f / height, 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 1.f, 1.f }
		};
		return Matrix4(temp);
	}

	static const Matrix4 Identity;
};

//Quaternion
class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() { *this = Quaternion::Identity; }
	//쿼터니언 요소를 직접 설정(축/각도 사용하지 않음)
	explicit Quaternion(float inX, float inY, float inZ, float inW) {
		Set(inX, inY, inZ, inW);
	}
	//축과 각도로부터 쿼터니언 생성(축은 정규화되었다고 가정, 각도는 라디안)
	explicit Quaternion(const Vector3& axis, float angle) {
		float scalar = Math::Sin(angle / 2.f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = Math::Cos(angle / 2.f);
	}

	//내부 요소를 직접 설정
	void Set(float inX, float inY, float inZ, float inW) {
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate() {
		x *= -1.f;
		y *= -1.f;
		z *= -1.f;
	}

	float LengthSq() const { return (x * x + y * y + z * z + w * w); }

	float Length() const { return Math::Sqrt(LengthSq()); }

	void Normalize() {
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}
	//정규화된 쿼터니언 제공
	static Quaternion Normalize(const Quaternion& q) {
		Quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	}
	//선형 보간
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f) {
		Quaternion retVal;
		retVal.x = Math::Lerp(a.x, b.x, f);
		retVal.y = Math::Lerp(a.y, b.y, f);
		retVal.z = Math::Lerp(a.z, b.z, f);
		retVal.w = Math::Lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}

	static float Dot(const Quaternion& a, const Quaternion& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	//구면 선형 보간
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f) {
		float rawCosm = Quaternion::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f) cosom = rawCosm;

		float scale0, scale1;

		if (cosom < 0.9999f) {
			const float omega = Math::Acos(cosom);
			const float invSin = 1.f / Math::Sin(omega);
			scale0 = Math::Sin((1.f - f) * omega) * invSin;
			scale1 = Math::Sin(f * omega) * invSin;
		}
		else {
			//쿼터니언이 선형인 경우 선형 보간 사용
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f) scale1 = -scale1;

		Quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.Normalize();
		return retVal;
	}
	//쿼터니언 곱셈 잇기(q로 회전한뒤 p의 회전은 그라스만 곱pq를 사용)
	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p) {
		Quaternion retVal;

		//벡터 성분: ps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		//스칼라 성분: ps * qs - pv . qv
		retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

		return retVal;
	}

	static const Quaternion Identity; //v = (0, 0, 0)/s = 1
};

namespace Color {
	static const Vector3 Black(0.f, 0.f, 0.f);
	static const Vector3 White(1.0, 1.f, 1.f);
	static const Vector3 Red(1.f, 0.f, 0.f);
	static const Vector3 Green(0.f, 1.f, 0.f);
	static const Vector3 Blue(0.f, 0.f, 1.f);
	static const Vector3 Yellow(1.f, 1.f, 0.f);
	static const Vector3 LightYellow(1.f, 1.f, 0.88f);
	static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
	static const Vector3 LightPink(1.f, 0.71f, 0.76f);
	static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
};
